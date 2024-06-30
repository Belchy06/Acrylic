#include "acpch.h"

#include "StructuredLog.h"
#include "Core/Globals.h"

namespace Acrylic
{
	namespace Logging
	{
		LogTemplate* LogTemplate::Create(const char* Format, const LogField* Fields, const int32_t FieldCount)
		{
			const std::vector<LogField> FieldsView(Fields, Fields + FieldCount);
			const bool					bFindFields = !!Fields;
			const bool					bPositional = !FieldCount || std::none_of(FieldsView.begin(), FieldsView.end(), [](const LogField& Field) -> bool { return Field.Name != nullptr; });

			std::vector<LogTemplateOp> Ops;

			int32_t		FormatFieldCount = 0;
			int32_t		BracketSearchOffset = 0;
			size_t		FormatLength = std::string(Format).length();
			for (const char* TextStart = Format;;)
			{
				std::string Brackets = "{}";
				size_t		End = std::string(TextStart + BracketSearchOffset).find_first_of(Brackets);
				const char* const TextEnd = End == std::string::npos ? &Format[FormatLength] : TextStart + End;
				BracketSearchOffset = 0;

				if (TextEnd[0] == '{' && TextEnd[1] == '{' || TextEnd[0] == '}' && TextEnd[1] == '}')
				{
					// Only "{{" or "}}"
					if (TextStart == TextEnd)
					{
						Ops.push_back({ LogTemplateOp::OpSkip, 1 });
						TextStart = TextEnd + 1;
						BracketSearchOffset = 1;
					}
					// Text and "{{" or "}}"
					else
					{
						Ops.push_back({ LogTemplateOp::OpText, PTRDIFF_TO_INT32(1 + TextEnd - TextStart) });
						Ops.push_back({ LogTemplateOp::OpSkip, 1 });
						TextStart = TextEnd + 2;
					}
					continue;
				}

				// Text
				if (TextStart != TextEnd)
				{
					Ops.push_back({ LogTemplateOp::OpText, PTRDIFF_TO_INT32(TextEnd - TextStart) });
					TextStart = TextEnd;
				}

				// End
				if (!*TextEnd)
				{
					Ops.push_back({ LogTemplateOp::OpEnd });
					break;
				}

				// Field
				const char* const FieldStart = TextStart;

				std::string ValidLogFieldName = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
				size_t		EndIndex = 1;
				while (ValidLogFieldName.find(FieldStart[EndIndex]) != std::string::npos)
				{
					EndIndex++;
				}
				const char* const FieldNameEnd = &FieldStart[EndIndex];
				/*checkf(*FieldNameEnd, TEXT("Log format has an unterminated field reference. Use '{{' to escape '{' if needed. [[%s]]"), Format);
				checkf(*FieldNameEnd == TEXT('}'), TEXT("Log format has invalid character '%c' in field name. "
														"Use '{{' to escape '{' if needed. Names must match \"[A-Za-z0-9][A-Za-z0-9_]*\". [[%s]]"),
													*FieldNameEnd, Format);*/
				const char* const FieldEnd = FieldNameEnd + 1;
				const int32_t	  FieldLen = PTRDIFF_TO_INT32(FieldEnd - FieldStart);
				const int32_t	  FieldNameLen = FieldLen - 2;
				/*checkf(FieldStart[1] != TEXT('_'), TEXT("Log format uses reserved field name '%.*s' with leading '_'. "
														"Names must match \"[A-Za-z0-9][A-Za-z0-9_]*\". [[%s]]"),
					FieldNameLen, FieldStart + 1, Format);*/

				if (bFindFields && !bPositional)
				{
					bool bFoundField = false;
					for (int32_t BaseFieldIndex = 0; BaseFieldIndex < FieldCount; ++BaseFieldIndex)
					{
						const int32_t FieldIndex = (FormatFieldCount + BaseFieldIndex) % FieldCount;
						const char*	  FieldName = Fields[FieldIndex].Name;
						if (strncmp(FieldName, FieldStart + 1, FieldNameLen) == 0 && !FieldName[FieldNameLen])
						{
							Ops.push_back({ LogTemplateOp::OpIndex, FieldIndex });
							bFoundField = true;
							break;
						}
					}
					/*checkf(bFoundField, TEXT("Log format requires field '%.*s' which was not provided. [[%s]]"),
						FieldNameLen, FieldStart + 1, Format);*/
				}

				Ops.push_back({ LogTemplateOp::OpName, FieldLen });
				++FormatFieldCount;

				TextStart = FieldEnd;
			}

			/*checkf(!bFindFields || !bPositional || FormatFieldCount == FieldCount, TEXT("Log format requires %d fields and %d were provided. [[%s]]"), FormatFieldCount, FieldCount, Format);*/

			LogTemplate* Template = new LogTemplate(Format, std::move(Ops));
			return Template;
		}

		void LogTemplate::Destroy(LogTemplate* Template)
		{
			free(Template);
		}

		void LogTemplate::FormatTo(std::ostream& Out, std::vector<LogField> Fields) const
		{
			auto FindField = [&Fields, It = Fields.begin(), Index = 0, Format = StaticFormat](std::string Name, int32_t IndexHint = -1) mutable -> LogField& {
				if (IndexHint >= 0)
				{
					for (; Index < IndexHint && It < Fields.end(); ++Index, ++It)
					{
					}
					if (IndexHint < Index)
					{
						It = Fields.begin();
						for (Index = 0; Index < IndexHint && It < Fields.end(); ++Index, ++It)
						{
						}
					}
					if (IndexHint == Index && Name == (It->Name != nullptr ? std::string(It->Name) : std::string("")))
					{
						return Fields[std::distance(Fields.begin(), It)];
					}
				}
				const int32_t PrevIndex = Index;
				for (; It < Fields.end(); ++Index, ++It)
				{
					if (Name == (It->Name != nullptr ? std::string(It->Name) : std::string("")))
					{
						return Fields[std::distance(Fields.begin(), It)];
					}
				}
				It = Fields.begin();
				for (Index = 0; Index < PrevIndex && It < Fields.end(); ++Index, ++It)
				{
					if (Name == (It->Name != nullptr ? std::string(It->Name) : std::string("")))
					{
						return Fields[std::distance(Fields.begin(), It)];
					}
				}
				// checkf(false, TEXT("Log format requires field '%.*hs' which was not provided. [[%s]]"), Name.Len(), Name.GetData(), Format);
				return Fields[std::distance(Fields.begin(), It)];
			};

			const char* NextFormat = StaticFormat;
			int32_t		FieldIndexHint = -1;
			for (const LogTemplateOp& Op : OpData)
			{
				switch (Op.Code)
				{
					case LogTemplateOp::OpEnd:
						Out << std::endl;
						return;
					case LogTemplateOp::OpText:
						Out << std::string(NextFormat, Op.Value);
						break;
					case LogTemplateOp::OpIndex:
						FieldIndexHint = Op.Value;
						break;
					case LogTemplateOp::OpName:
						const std::string Name(NextFormat + 1, Op.Value - 2);
						LogField&		  Field = FindField(Name, FieldIndexHint);
						Field.WriteValue(Out, Field.Value);
						FieldIndexHint = -1;
						break;
				}

				NextFormat += Op.GetSkipSize();
			}
		}

		static void FormatRecordMessageTo(std::ostream& Out, const LogRecord& Record)
		{
			struct std::tm LocalTime;
			localtime_s(&LocalTime, &Record.GetTime());
			Out << "[" << std::put_time(&LocalTime, "%Y.%m.%d-%H.%M.%S") << "]" << Record.GetCategory() << ": ";

			const LogTemplate* Template = Record.GetTemplate();
			if (LIKELY(Template))
			{
				return Template->FormatTo(Out, Record.GetFields());
			}
		}

		struct StaticLogTemplateManager
		{
			std::atomic<LogTemplate*> Head = nullptr;

			~StaticLogTemplateManager()
			{
				for (LogTemplate* Template = Head.exchange(nullptr); Template;)
				{
					LogTemplate* NextTemplate = Template->GetNext();
					LogTemplate::Destroy(Template);
					Template = NextTemplate;
				}
			}
		};

		static StaticLogTemplateManager GStaticLogTemplateManager;

		template <typename StaticLogRecordType>
		FORCENOINLINE static LogTemplate& CreateLogTemplate(const LogCategoryBase& Category, const StaticLogRecordType& Log, const LogField* Fields, const int32_t FieldCount)
		{
			for (LogTemplate* Template = Log.DynamicData.Template.load(std::memory_order_acquire);;)
			{
				if (Template && Template->GetFormat() == Log.Format)
				{
					return *Template;
				}

				LogTemplate* NewTemplate = LogTemplate::CreateStatic(Log, Fields, FieldCount);
				if (LIKELY(Log.DynamicData.Template.compare_exchange_strong(Template, NewTemplate, std::memory_order_release, std::memory_order_acquire)))
				{
					// Register the template to destroy on exit.
					for (LogTemplate* Head = GStaticLogTemplateManager.Head.load(std::memory_order_relaxed);;)
					{
						NewTemplate->SetNext(Head);
						if (LIKELY(GStaticLogTemplateManager.Head.compare_exchange_weak(Head, NewTemplate, std::memory_order_release, std::memory_order_relaxed)))
						{
							break;
						}
					}
					return *NewTemplate;
				}
				LogTemplate::Destroy(NewTemplate);
			}
		}

		template <typename StaticLogRecordType>
		inline static LogTemplate& EnsureLogTemplate(const LogCategoryBase& Category, const StaticLogRecordType& Log, const LogField* Fields, const int32_t FieldCount)
		{
			// Format can change on a static log record due to Live Coding.
			if (LogTemplate* Template = Log.DynamicData.Template.load(std::memory_order_acquire); LIKELY(Template && Template->GetFormat() == Log.Format))
			{
				return *Template;
			}
			return CreateLogTemplate(Category, Log, Fields, FieldCount);
		}

		template <typename StaticLogRecordType>
		inline static LogRecord CreateLogRecord(const LogCategoryBase& Category, const StaticLogRecordType& Log, const LogField* Fields, const int32_t FieldCount)
		{
			const LogTemplate& Template = EnsureLogTemplate(Category, Log, Fields, FieldCount);

			LogRecord Record;
			Record.SetFormat(Log.Format);
			Record.SetTemplate(&Template);
			Record.SetFields(Template, Fields, FieldCount);
			Record.SetFile(Log.File);
			Record.SetLine(Log.Line);
			Record.SetCategory(Category.GetCategoryName());
			Record.SetVerbosity(Log.Verbosity);
			Record.SetTime(std::time(nullptr));
			return Record;
		}

		template <typename StaticLogRecordType>
		inline static void DispatchLogRecord(const StaticLogRecordType& Log, const LogRecord& Record)
		{
			std::ostream* OutputDevice = &GLog;
			switch (Log.Verbosity)
			{
				case ELogVerbosity::Error:
				case ELogVerbosity::Warning:
				case ELogVerbosity::Display:
				case ELogVerbosity::SetColor:
					OutputDevice = &GWarn;
					break;
				default:
					break;
			}

			FormatRecordMessageTo(*OutputDevice, Record);
		}

		void LogWithNoFields(const LogCategoryBase& Category, const StaticLogRecord& Log)
		{
			// A non-null field pointer enables field validation in FLogTemplate::Create.
			static constexpr LogField EmptyField{};
			LogWithFieldArray(Category, Log, &EmptyField, 0);
		}

		void LogWithFieldArray(const LogCategoryBase& Category, const StaticLogRecord& Log, const LogField* Fields, int32_t FieldCount)
		{
			DispatchLogRecord(Log, CreateLogRecord(Category, Log, Fields, FieldCount));
		}

		void FatalLogWithNoFields(const LogCategoryBase& Category, const StaticLogRecord& Log)
		{
			// A non-null field pointer enables field validation in FLogTemplate::Create.
			static constexpr LogField EmptyField{};
			FatalLogWithFieldArray(Category, Log, &EmptyField, 0);
		}

		void FatalLogWithFieldArray(const LogCategoryBase& Category, const StaticLogRecord& Log, const LogField* Fields, int32_t FieldCount)
		{
			DispatchLogRecord(Log, CreateLogRecord(Category, Log, Fields, FieldCount));
			// TODO: crash nicer
			exit(1);
		}

		LogTemplateFieldIterator& LogTemplateFieldIterator::operator++()
		{
			// Iterate on a copy
			std::vector<LogTemplateOp> Ops = NextOp;
			for (LogTemplateOp& Op : Ops)
			{
				if (Op.Code == LogTemplateOp::OpName)
				{
					Name = std::string(NextFormat + 1, Op.Value - 2);
					NextFormat += Op.GetSkipSize();
					NextOp.erase(NextOp.begin());
					return *this;
				}
				if (Op.Code == LogTemplateOp::OpEnd)
				{
					break;
				}
				NextFormat += Op.GetSkipSize();
				NextOp.erase(NextOp.begin());
			}

			NextOp.clear();
			Name = "";
			return *this;
		}
	} // namespace Logging
} // namespace Acrylic