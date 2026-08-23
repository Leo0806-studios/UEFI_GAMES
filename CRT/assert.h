#pragma once
#ifndef  __ASSERT__
#define __ASSERT__

#include  <corecrt.h>
#include "stdbool.h"
#include "wchar.h"

#include  "CRT_MACROS.h"
#include "CRT_CORE.h"
__CRT_START
#ifndef __cplusplus
#	if _CRT_HAS_C11
#		define static_assert _Static_assert
#	endif
#endif // !__cplusplus
_ACRTIMP void __cdecl __wassertImpl(
	_In_z_ wchar_t const* _Message,
	_In_z_ wchar_t const* _File,
	_In_   unsigned       _Line
);


#if __STDC_VERSION__ >= 202311L
#	define __STDC_VERSION_ASSERT_H__ 202311L
#	ifdef NDEBUG
#		define assert(...) ((void)0)
#	else
#		define assert(...) ((void)((!!(__VA_ARGS__)) ||(__wassertImpl(_CRT_WIDE(#__VA_ARGS__), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)) 
#	endif
#else
#	ifdef NDEBUG
#		define assert(condition) ((void)0)
#	else
#		define assert(expression) ((void)((!!(expression)) ||(__wassertImpl(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)) 
#	endif
#endif

__CRT_END


#endif // ! __ASSERT__
