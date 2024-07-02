#pragma once

// Concatenates two preprocessor tokens, performing macro expansion on them first
#define PREPROCESSOR_JOIN(x, y) PREPROCESSOR_JOIN_INNER(x, y)
#define PREPROCESSOR_JOIN_INNER(x, y) x##y

// Function type macros.
#define VARARGS __cdecl /* Functions with variable arguments */
// #define CDECL __cdecl								 /* Standard C function */
#define STDCALL __stdcall							 /* Standard calling convention */
#define FORCEINLINE __forceinline					 /* Force code to be inline */
#define FORCENOINLINE __declspec(noinline)			 /* Force code to NOT be inline */
#define FUNCTION_NON_NULL_RETURN_START _Ret_notnull_ /* Indicate that the function never returns nullptr. */
#define LIKELY(x) (x)

// These defines are used to mark a difference between two pointers as expected to fit into the specified range
// while still leaving something searchable if the surrounding code is updated to work with a 64 bit count/range
// in the future
#define PTRDIFF_TO_INT32(argument) static_cast<int32_t>(argument)
#define PTRDIFF_TO_UINT32(argument) static_cast<uint32_t>(argument)