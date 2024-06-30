#include "LogCategory.h"

namespace Acrylic
{
	LogCategoryBase::LogCategoryBase(const LogCategoryName& CategoryName, ELogVerbosity DefaultVerbosity)
		: DefaultVerbosity(DefaultVerbosity)
		, CategoryName(CategoryName)
	{
		ResetFromDefault();
	}

	LogCategoryBase::~LogCategoryBase()
	{
	}

	void LogCategoryBase::SetVerbosity(ELogVerbosity NewVerbosity)
	{
		// regularize the verbosity to be at most whatever we were compiled with
		Verbosity = (ELogVerbosity)(NewVerbosity & ELogVerbosity::VerbosityMask);
	}

	void LogCategoryBase::ResetFromDefault()
	{
		// regularize the default verbosity to be at most whatever we were compiled with
		SetVerbosity(ELogVerbosity(DefaultVerbosity));
	}
}