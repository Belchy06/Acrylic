#pragma once

#include "Core/Logging.h"
#include "Core/Containers/String.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

namespace Acrylic
{
	DECLARE_LOG_CATEGORY_EXTERN(LogInstrumentor, Log);

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		String Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id			  ThreadID;
	};

	struct InstrumentationSession
	{
		String Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const String& Name, const String& Path = "Results.JSON")
		{
			std::lock_guard lock(Mutex);
			if (CurrentSession)
			{
				AC_LOG(LogInstrumentor, Error, "Instrumentor::BeginSession('{0}') when session '{1}' already open.", Name, CurrentSession->Name);
				InternalEndSession();
			}
			OutputStream.open(Path);

			if (OutputStream.is_open())
			{
				CurrentSession = new InstrumentationSession({ Name });
				WriteHeader();
			}
			else
			{
				AC_LOG(LogInstrumentor, Error, "Instrumentor could not open Results file '{0}'.", Path);
			}
		}

		void EndSession()
		{
			std::lock_guard lock(Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& Result)
		{
			std::stringstream JSON;

			JSON << std::setprecision(3) << std::fixed;
			JSON << ",{";
			JSON << "\"cat\":\"function\",";
			JSON << "\"dur\":" << (Result.ElapsedTime.count()) << ',';
			JSON << "\"Name\":\"" << Result.Name << "\",";
			JSON << "\"ph\":\"X\",";
			JSON << "\"pid\":0,";
			JSON << "\"tid\":" << Result.ThreadID << ",";
			JSON << "\"ts\":" << Result.Start.count();
			JSON << "}";

			std::lock_guard lock(Mutex);
			if (CurrentSession)
			{
				OutputStream << JSON.str();
				OutputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor Instance;
			return Instance;
		}

	private:
		Instrumentor()
			: CurrentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			OutputStream.flush();
		}

		void WriteFooter()
		{
			OutputStream << "]}";
			OutputStream.flush();
		}

		// Note: you must already own lock on Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (CurrentSession)
			{
				WriteFooter();
				OutputStream.close();
				delete CurrentSession;
				CurrentSession = nullptr;
			}
		}

	private:
		std::mutex				Mutex;
		InstrumentationSession* CurrentSession;
		std::ofstream			OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* InName)
			: Name(InName)
			, Stopped(false)
		{
			StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!Stopped)
			{
				Stop();
			}
		}

		void Stop()
		{
			auto EndTimepoint = std::chrono::steady_clock::now();
			auto HighResStart = FloatingPointMicroseconds{ StartTimepoint.time_since_epoch() };
			auto ElapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(EndTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ Name, HighResStart, ElapsedTime, std::this_thread::get_id() });

			Stopped = true;
		}

	private:
		const char*										   Name;
		std::chrono::time_point<std::chrono::steady_clock> StartTimepoint;
		bool											   Stopped;
	};

	namespace InstrumentorUtils
	{
		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
		{
			ChangeResult<N> Result = {};

			size_t SrcIndex = 0;
			size_t DstIndex = 0;
			while (SrcIndex < N)
			{
				size_t MatchIndex = 0;
				while (MatchIndex < K - 1 && SrcIndex + MatchIndex < N - 1 && expr[SrcIndex + MatchIndex] == remove[MatchIndex])
				{
					MatchIndex++;
				}
				if (MatchIndex == K - 1)
				{
					SrcIndex += MatchIndex;
				}
				Result.Data[DstIndex++] = expr[SrcIndex] == '"' ? '\'' : expr[SrcIndex];
				SrcIndex++;
			}
			return Result;
		}
	} // Namespace InstrumentorUtils
} // namespace Acrylic