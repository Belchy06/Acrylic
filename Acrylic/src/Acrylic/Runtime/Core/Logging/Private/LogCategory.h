#pragma once

#include "Core/Core.h"
#include "LogVerbosity.h"

using LogCategoryName = std::string;

namespace Acrylic
{
	/** Base class for all log categories. */
	struct LogCategoryBase
	{
		/**
		* Constructor, registers with the log suppression system and sets up the default values.
		*
		* @param CategoryName           Name of the category.
		* @param DefaultVerbosity       Default verbosity used to filter this category at runtime.
		*/
		LogCategoryBase(const LogCategoryName& CategoryName, ELogVerbosity DefaultVerbosity);
	
		/** Destructor, unregisters from the log suppression system. */
		~LogCategoryBase();

		/** Should not generally be used directly. Tests the runtime verbosity and maybe triggers a debug break, etc. */
		FORCEINLINE constexpr bool IsSuppressed(ELogVerbosity VerbosityLevel) const
		{
			return !((VerbosityLevel & ELogVerbosity::VerbosityMask) <= Verbosity);
		}

		inline constexpr const LogCategoryName& GetCategoryName() const { return CategoryName; }

		/** Gets the working verbosity. */
		inline constexpr ELogVerbosity GetVerbosity() const { return (ELogVerbosity)Verbosity; }

		/** Sets up the working verbosity and clamps to the compile time verbosity. */
		void SetVerbosity(ELogVerbosity Verbosity);

	private:
		/** Internal call to set up the working verbosity from the boot time default. */
		void ResetFromDefault();

		/** Holds the current suppression state */
		ELogVerbosity Verbosity;
		/** Holds default suppression */
		ELogVerbosity DefaultVerbosity;
		/** Name for this category */
		const LogCategoryName CategoryName;
	};

	/** Template for log categories that transfers the compile-time constant default and compile time verbosity to the LogCategoryBase constructor. */
	template <ELogVerbosity TDefaultVerbosity>
	struct LogCategory : public LogCategoryBase
	{
		static_assert((TDefaultVerbosity & ELogVerbosity::VerbosityMask) < ELogVerbosity::NumVerbosity, "Invalid default verbosity.");

		FORCEINLINE LogCategory(const LogCategoryName& CategoryName)
			: LogCategoryBase(CategoryName, TDefaultVerbosity)
		{
		}
	};
} // namespace Acrylic