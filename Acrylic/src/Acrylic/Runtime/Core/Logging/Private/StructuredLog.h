#pragma once

#include "Core/PreprocessorHelpers.h"
#include "LogCategory.h"
#include "LogVerbosity.h"

namespace Acrylic
{
#define AC_LOG(CategoryName, Verbosity, Format, ...) AC_PRIVATE_LOG_CALL(AC_LOG_EX, (CategoryName, Verbosity, Format AC_PRIVATE_LOG_FIELDS(__VA_ARGS__)))

#define AC_LOG_EX(CategoryName, Verbosity, Format, ...) AC_PRIVATE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)

	namespace Logging
	{
		struct ACRYLIC_API LogTemplateOp
		{
			enum EOpCode : int32_t
			{
				OpEnd,
				OpSkip,
				OpText,
				OpName,
				OpIndex,
				OpCount
			};

			static constexpr int32_t ValueShift = 3;
			static_assert(OpCount <= (1 << ValueShift));

			EOpCode Code = OpEnd;
			int32_t Value = 0;

			inline int32_t GetSkipSize() const { return Code == OpIndex ? 0 : Value; }
		};

		struct ACRYLIC_API LogField
		{
			using WriteFn = void(std::ostream& Writer, const void* Value);

			const char* Name = nullptr;
			const void* Value = nullptr;
			WriteFn*	WriteValue = nullptr;

			template <typename ValueType>
			static void Write(std::ostream& Writer, const void* Value)
			{
				Writer << *(const ValueType*)Value;
			}
		};

		class ACRYLIC_API LogTemplate;

		/** Data about a static log that is created on-demand. */
		struct ACRYLIC_API StaticLogDynamicData
		{
			std::atomic<LogTemplate*> Template;
		};

		/** Data about a static log that is constant for every occurrence. */
		struct ACRYLIC_API StaticLogRecord
		{
			const char*			  Format = nullptr;
			const char*			  File = nullptr;
			int32_t				  Line = 0;
			ELogVerbosity		  Verbosity = ELogVerbosity::Log;
			StaticLogDynamicData& DynamicData;

			// Workaround for https://developercommunity.visualstudio.com/t/Incorrect-warning-C4700-with-unrelated-s/10285950
			constexpr StaticLogRecord(
				const char*			  InFormat,
				const char*			  InFile,
				int32_t				  InLine,
				ELogVerbosity		  InVerbosity,
				StaticLogDynamicData& InDynamicData)
				: Format(InFormat)
				, File(InFile)
				, Line(InLine)
				, Verbosity(InVerbosity)
				, DynamicData(InDynamicData)
			{
			}
		};

		class ACRYLIC_API LogTemplate
		{
		public:
			static LogTemplate* CreateStatic(const StaticLogRecord& Log, const LogField* Fields, int32_t FieldCount)
			{
				return Create(Log.Format, Fields, FieldCount);
			}

			static LogTemplate* Create(const char* Format, const LogField* Fields = nullptr, int32_t FieldCount = 0);

			static void Destroy(LogTemplate* Template);

			const char* GetFormat() const { return StaticFormat; }

			LogTemplate* GetNext() { return Next; }
			void		 SetNext(LogTemplate* Template) { Next = Template; }

			std::vector<LogTemplateOp>&		  GetOpData() { return OpData; }
			const std::vector<LogTemplateOp>& GetOpData() const { return OpData; }

			void FormatTo(std::ostream& Out, std::vector<LogField> Fields) const;

		private:
			inline constexpr LogTemplate(const char* StaticFormat, std::vector<LogTemplateOp>&& OpData)
				: StaticFormat(StaticFormat)
				, OpData(OpData)
			{
			}
			~LogTemplate() = default;
			LogTemplate(const LogTemplate&) = delete;
			LogTemplate& operator=(const LogTemplate&) = delete;

			const char*				   StaticFormat = nullptr;
			LogTemplate*			   Next = nullptr;
			std::vector<LogTemplateOp> OpData;
		};

		class ACRYLIC_API LogTemplateFieldIterator
		{
		public:
			inline explicit LogTemplateFieldIterator(const LogTemplate& Template)
				: NextOp(Template.GetOpData())
				, NextFormat(Template.GetFormat())
			{
				++*this;
			}

			LogTemplateFieldIterator& operator++();
			inline explicit operator bool() const { return !NextOp.empty(); }
			inline const std::string& GetName() const { return Name; }

		private:
			std::string				   Name;
			std::vector<LogTemplateOp> NextOp;
			const char*				   NextFormat = nullptr;
		};

		/**
		 * Record of a log event.
		 */
		class ACRYLIC_API LogRecord
		{
		public:
			/** The optional name of the category for the log record. None when omitted. */
			const std::string& GetCategory() const { return Category; }
			void			   SetCategory(const std::string& InCategory) { Category = InCategory; }

			/** The verbosity level of the log record. Must be a valid level with no flags or special values. */
			ELogVerbosity GetVerbosity() const { return Verbosity; }
			void		  SetVerbosity(ELogVerbosity InVerbosity) { Verbosity = InVerbosity; }

			/** The time at which the log record was created. */
			const std::time_t& GetTime() const { return Time; }
			void			   SetTime(const std::time_t& InTime) { Time = InTime; }

			/** The format string that serves as the message for the log record. Example: TEXT("FieldName is {FieldName}") */
			const char* GetFormat() const { return Format; }
			void		SetFormat(const char* InFormat) { Format = InFormat; }

			/** The optional template for the format string. */
			const LogTemplate* GetTemplate() const { return Template; }
			void			   SetTemplate(const LogTemplate* InTemplate) { Template = InTemplate; }

			/** The fields referenced by the format string, along with optional additional fields. */
			const std::vector<LogField>& GetFields() const { return Fields; }
			void						 SetFields(const LogTemplate& Template, const LogField* InFields, const int32_t FieldCount)
			{
				Fields.assign(InFields, InFields + FieldCount);

				// Anonymous params. Extract names from template
				if (FieldCount > 0 && !InFields->Name)
				{
					LogTemplateFieldIterator It(Template);
					for (LogField& Field : Fields)
					{
						Field.Name = new char[It.GetName().size() + 1];
						strcpy_s(const_cast<char*>(Field.Name), It.GetName().size() + 1, It.GetName().c_str());
						++It;
					}
				}
			}

			/** The optional source file path for the code that created the log record. Null when omitted. */
			const char* GetFile() const { return File; }
			void		SetFile(const char* InFile) { File = InFile; }

			/** The optional source line number for the code that created the log record. 0 when omitted. */
			int32_t GetLine() const { return Line; }
			void	SetLine(int32_t InLine) { Line = InLine; }

			/** The namespace of the localized text. Null when non-localized. */
			const char* GetTextNamespace() const { return TextNamespace; }
			void		SetTextNamespace(const char* InTextNamespace) { TextNamespace = InTextNamespace; }

			/** The key of the localized text. Null when non-localized. */
			const char* GetTextKey() const { return TextKey; }
			void		SetTextKey(const char* InTextKey) { TextKey = InTextKey; }

		private:
			const char*			  Format = nullptr;
			const char*			  File = nullptr;
			int32_t				  Line = 0;
			std::string			  Category;
			ELogVerbosity		  Verbosity = ELogVerbosity::Log;
			std::time_t			  Time = std::time(nullptr);
			std::vector<LogField> Fields;
			const LogTemplate*	  Template = nullptr;
			const char*			  TextNamespace = nullptr;
			const char*			  TextKey = nullptr;
		};

		void ACRYLIC_API LogWithNoFields(const LogCategoryBase& Category, const StaticLogRecord& Log);
		void			 ACRYLIC_API LogWithFieldArray(const LogCategoryBase& Category, const StaticLogRecord& Log, const LogField* Fields, int32_t FieldCount);

		void ACRYLIC_API FatalLogWithNoFields(const LogCategoryBase& Category, const StaticLogRecord& Log);
		void			 ACRYLIC_API FatalLogWithFieldArray(const LogCategoryBase& Category, const StaticLogRecord& Log, const LogField* Fields, int32_t FieldCount);

		/** Wrapper to identify field names interleaved with field values. */
		template <typename NameType>
		struct ACRYLIC_API TLogFieldName
		{
			NameType Name;
		};

		/** Create log fields from values optionally preceded by names. */
		struct ACRYLIC_API LogFieldCreator
		{
			template <typename T>
			inline constexpr static int32_t ValueCount = 1;

			template <typename... FieldArgTypes>
			inline constexpr static int32_t GetCount()
			{
				return (ValueCount<FieldArgTypes> + ...);
			}

			inline static void Create(LogField* Fields)
			{
			}

			template <typename ValueType, typename... FieldArgTypes, std::enable_if_t<ValueCount<ValueType>>* = nullptr>
			inline static void Create(LogField* Fields, const ValueType& Value, FieldArgTypes&&... FieldArgs)
			{
				new (Fields) LogField{ nullptr, &Value, LogField::Write<std::remove_reference_t<ValueType>> };
				Create(Fields + 1, (FieldArgTypes&&)FieldArgs...);
			}
		};

		template <typename T>
		struct ACRYLIC_API TFieldArgType
		{
			using Type = T;
		};

		/** Log with fields created from the arguments, which may be values or pairs of name/value. */
		template <typename... FieldArgTypes, typename LogType>
		FORCENOINLINE void LogWithFields(const LogCategoryBase& Category, const LogType& Log, typename TFieldArgType<FieldArgTypes>::Type... FieldArgs)
		{
			constexpr int32_t FieldCount = LogFieldCreator::template GetCount<FieldArgTypes...>();
			static_assert(FieldCount > 0);
			LogField Fields[FieldCount];
			LogFieldCreator::Create(Fields, (FieldArgTypes&&)FieldArgs...);
			LogWithFieldArray(Category, Log, Fields, FieldCount);
		}

		/** Fatal log with fields created from the arguments, which may be values or pairs of name/value. */
		template <typename... FieldArgTypes, typename LogType>
		inline void FatalLogWithFields(const LogCategoryBase& Category, const LogType& Log, typename TFieldArgType<FieldArgTypes>::Type... FieldArgs)
		{
			constexpr int32_t FieldCount = LogFieldCreator::template GetCount<FieldArgTypes...>();
			static_assert(FieldCount > 0);
			LogField Fields[FieldCount];
			LogFieldCreator::Create(Fields, (FieldArgTypes&&)FieldArgs...);
			FatalLogWithFieldArray(Category, Log, Fields, FieldCount);
		}

		/** Log if the category is active at this level of verbosity. */
		template <typename LogCategoryType, ELogVerbosity Verbosity, typename LogRecordType, typename... FieldArgTypes>
		inline void LogIfActive(const LogCategoryType& Category, const LogRecordType& Log, FieldArgTypes&&... FieldArgs)
		{
			static_assert((Verbosity & ELogVerbosity::VerbosityMask) < ELogVerbosity::NumVerbosity && static_cast<uint8_t>(Verbosity) > 0, "Verbosity must be constant and in range.");
			if constexpr ((Verbosity & ELogVerbosity::VerbosityMask) == ELogVerbosity::Fatal)
			{

				if constexpr (sizeof...(FieldArgTypes) == 0)
				{
					FatalLogWithNoFields(Category, Log);
				}
				else
				{
					FatalLogWithFields<FieldArgTypes...>(Category, Log, (FieldArgTypes&&)FieldArgs...);
				}
			}
			else if constexpr ((Verbosity & ELogVerbosity::VerbosityMask) <= ELogVerbosity::VeryVerbose)
			{
				if (!Category.IsSuppressed(Verbosity))
				{
					if constexpr (sizeof...(FieldArgTypes) == 0)
					{
						LogWithNoFields(Category, Log);
					}
					else
					{
						LogWithFields<FieldArgTypes...>(Category, Log, (FieldArgTypes&&)FieldArgs...);
					}
				}
			}
		}
	} // namespace Logging

#define AC_PRIVATE_LOG(CategoryName, Verbosity, Format, ...)                                                                                                                                \
	do                                                                                                                                                                                      \
	{                                                                                                                                                                                       \
		static Acrylic::Logging::StaticLogDynamicData				  LOG_Dynamic;                                                                                                          \
		static constexpr Acrylic::Logging::StaticLogRecord LOG_Static AC_PRIVATE_LOG_AGGREGATE(Format, __builtin_FILE(), __builtin_LINE(), Acrylic::ELogVerbosity::Verbosity, LOG_Dynamic); \
		Acrylic::Logging::LogIfActive<LogCategory##CategoryName, Acrylic::ELogVerbosity::Verbosity>(CategoryName, LOG_Static, ##__VA_ARGS__);                                               \
	}                                                                                                                                                                                       \
	while (false)

// This macro avoids having non-parenthesized commas in the log macros.
// Such commas can cause issues when a log macro is wrapped by another macro.
#define AC_PRIVATE_LOG_AGGREGATE(...) \
	{                                 \
		__VA_ARGS__                   \
	}

// This macro expands a field from either `(Name, Value)` or `Value`
// A `(Name, Value)` field is converted to `CheckFieldName(Name), Value`
// A `Value` field is passed through as `Value`
#define AC_PRIVATE_LOG_FIELD(Field) AC_PRIVATE_LOG_FIELD_EXPAND(AC_PRIVATE_LOG_NAMED_FIELD Field)
// This macro is only called when the field was parenthesized.
#define AC_PRIVATE_LOG_NAMED_FIELD(Name, ...) AC_PRIVATE_LOG_NAMED_FIELD Name, __VA_ARGS__
// The next three macros remove AC_PRIVATE_LOG_NAMED_FIELD from the expanded expression.
#define AC_PRIVATE_LOG_FIELD_EXPAND(...) AC_PRIVATE_LOG_FIELD_EXPAND_INNER(__VA_ARGS__)
#define AC_PRIVATE_LOG_FIELD_EXPAND_INNER(...) AC_PRIVATE_LOG_STRIP_##__VA_ARGS__
#define AC_PRIVATE_LOG_STRIP_AC_PRIVATE_LOG_NAMED_FIELD

// This macro expands `Arg1, Arg2` to `AC_PRIVATE_LOG_FIELD(Arg1), AC_PRIVATE_LOG_FIELD(Arg2), ...`
// This macro expands `("Name1", Arg1), ("Name2", Arg2)` to `AC_PRIVATE_LOG_FIELD(("Name1", Arg1)), AC_PRIVATE_LOG_FIELD(("Name2", Arg2)), ...
#define AC_PRIVATE_LOG_FIELDS(...) AC_PRIVATE_LOG_CALL(PREPROCESSOR_JOIN(AC_PRIVATE_LOG_FIELDS_, AC_PRIVATE_LOG_COUNT(__VA_ARGS__)), (__VA_ARGS__))

#define AC_PRIVATE_LOG_FIELDS_0()
#define AC_PRIVATE_LOG_FIELDS_1(A) , AC_PRIVATE_LOG_FIELD(A)
#define AC_PRIVATE_LOG_FIELDS_2(A, B) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B)
#define AC_PRIVATE_LOG_FIELDS_3(A, B, C) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C)
#define AC_PRIVATE_LOG_FIELDS_4(A, B, C, D) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D)
#define AC_PRIVATE_LOG_FIELDS_5(A, B, C, D, E) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E)
#define AC_PRIVATE_LOG_FIELDS_6(A, B, C, D, E, F) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F)
#define AC_PRIVATE_LOG_FIELDS_7(A, B, C, D, E, F, G) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G)
#define AC_PRIVATE_LOG_FIELDS_8(A, B, C, D, E, F, G, H) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H)
#define AC_PRIVATE_LOG_FIELDS_9(A, B, C, D, E, F, G, H, I) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H), AC_PRIVATE_LOG_FIELD(I)

#define AC_PRIVATE_LOG_FIELDS_10(A, B, C, D, E, F, G, H, I, J) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H), AC_PRIVATE_LOG_FIELD(I), AC_PRIVATE_LOG_FIELD(J)
#define AC_PRIVATE_LOG_FIELDS_11(A, B, C, D, E, F, G, H, I, J, K) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H), AC_PRIVATE_LOG_FIELD(I), AC_PRIVATE_LOG_FIELD(J), AC_PRIVATE_LOG_FIELD(K)
#define AC_PRIVATE_LOG_FIELDS_12(A, B, C, D, E, F, G, H, I, J, K, L) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H), AC_PRIVATE_LOG_FIELD(I), AC_PRIVATE_LOG_FIELD(J), AC_PRIVATE_LOG_FIELD(K), AC_PRIVATE_LOG_FIELD(L)
#define AC_PRIVATE_LOG_FIELDS_13(A, B, C, D, E, F, G, H, I, J, K, L, M) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H), AC_PRIVATE_LOG_FIELD(I), AC_PRIVATE_LOG_FIELD(J), AC_PRIVATE_LOG_FIELD(K), AC_PRIVATE_LOG_FIELD(L), AC_PRIVATE_LOG_FIELD(M)
#define AC_PRIVATE_LOG_FIELDS_14(A, B, C, D, E, F, G, H, I, J, K, L, M, N) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H), AC_PRIVATE_LOG_FIELD(I), AC_PRIVATE_LOG_FIELD(J), AC_PRIVATE_LOG_FIELD(K), AC_PRIVATE_LOG_FIELD(L), AC_PRIVATE_LOG_FIELD(M), AC_PRIVATE_LOG_FIELD(N)
#define AC_PRIVATE_LOG_FIELDS_15(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H), AC_PRIVATE_LOG_FIELD(I), AC_PRIVATE_LOG_FIELD(J), AC_PRIVATE_LOG_FIELD(K), AC_PRIVATE_LOG_FIELD(L), AC_PRIVATE_LOG_FIELD(M), AC_PRIVATE_LOG_FIELD(N), AC_PRIVATE_LOG_FIELD(O)
#define AC_PRIVATE_LOG_FIELDS_16(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P) , AC_PRIVATE_LOG_FIELD(A), AC_PRIVATE_LOG_FIELD(B), AC_PRIVATE_LOG_FIELD(C), AC_PRIVATE_LOG_FIELD(D), AC_PRIVATE_LOG_FIELD(E), AC_PRIVATE_LOG_FIELD(F), AC_PRIVATE_LOG_FIELD(G), AC_PRIVATE_LOG_FIELD(H), AC_PRIVATE_LOG_FIELD(I), AC_PRIVATE_LOG_FIELD(J), AC_PRIVATE_LOG_FIELD(K), AC_PRIVATE_LOG_FIELD(L), AC_PRIVATE_LOG_FIELD(M), AC_PRIVATE_LOG_FIELD(N), AC_PRIVATE_LOG_FIELD(O), AC_PRIVATE_LOG_FIELD(P)

#define AC_PRIVATE_LOG_COUNT(...) AC_PRIVATE_LOG_CALL(AC_PRIVATE_LOG_COUNT_IMPL, (_, ##__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define AC_PRIVATE_LOG_COUNT_IMPL(_, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Count, ...) Count

#define AC_PRIVATE_LOG_CALL(F, A) AC_PRIVATE_LOG_EXPAND(F A)
#define AC_PRIVATE_LOG_EXPAND(X) X
} // namespace Acrylic