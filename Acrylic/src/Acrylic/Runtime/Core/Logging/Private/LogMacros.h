#pragma once

#include "LogCategory.h"
#include "LogVerbosity.h"

/**
 * A macro to declare a logging category as a C++ "extern", usually declared in the header and paired with DEFINE_LOG_CATEGORY in the source. Accessible by all files that include the header.
 * @param CategoryName, category to declare
 * @param DefaultVerbosity, default run time verbosity
 * @param CompileTimeVerbosity, maximum verbosity to compile into the code
 **/
#define DECLARE_LOG_CATEGORY_EXTERN(CategoryName, DefaultVerbosity)                               \
	extern struct LogCategory##CategoryName : public Acrylic::LogCategory<Acrylic::ELogVerbosity::DefaultVerbosity> \
	{                                                                                             \
		FORCEINLINE LogCategory##CategoryName() : LogCategory(std::string(#CategoryName)) {}      \
	} CategoryName;

/**
 * A macro to define a logging category, usually paired with DECLARE_LOG_CATEGORY_EXTERN from the header.
 * @param CategoryName, category to define
 **/
#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory##CategoryName CategoryName;