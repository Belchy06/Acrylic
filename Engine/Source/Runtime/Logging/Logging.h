#pragma once

#include <iostream>
#include <format>

#define AC_LOG(Category, Fmt, ...)                                             \
	{                                                                          \
		std::string Format;                                                    \
		Format += "[";                                                         \
		Format += #Category;                                                   \
		Format += "] ";                                                        \
		Format += Fmt;                                                         \
		Format += "\n";                                                        \
		std::cout << std::vformat(Format, std::make_format_args(__VA_ARGS__)); \
	}