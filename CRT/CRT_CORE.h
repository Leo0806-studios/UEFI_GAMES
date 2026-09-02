#pragma once
#ifndef __CRT_CORE__
#define __CRT_CORE__
#include "CRT_MACROS.h"
#ifndef __STDC_VERSION__
#define __STDC_VERSION__ 0

#endif // !__STDC_VERSION__



#ifndef _WCHAR_T_DEFINED
#	define _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#endif

#ifndef _SIZE_T_DEFINED
#	define _SIZE_T_DEFINED
typedef unsigned long long size_t;
#endif
#ifndef _PTRDIFF_T_DEFINED
#define _PTRDIFF_T_DEFINED
typedef signed long long ptrdiff_t;
#endif // !_PTRDIFF_T_DEFINED

#define NULL ((void*)0)


typedef int errno_t;
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
typedef long __time32_t;
typedef __int64 __time64_t;
typedef size_t rsize_t;

#if __STDC_VERSION__ >= 202311l //BOOOOOOOOO intellisense lies when cstdlatest is set and falsely claims the bool keyword exists
#	ifndef __cplusplus
typedef _Bool bool; //yeah yeah. intellisense will complain but in pure C this is very validuntill the compiler ships and actrual c23 implementation.
#	define true 1
#	define false 0
#	endif
#endif

//C version dedection macros
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#		define __CRT_HAS_C23 1
#		define __CRT_HAS_C17 1
#		define __CRT_HAS_C11 1
#	else
#		if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201710L
#			define __CRT_HAS_C17 1
#			define __CRT_HAS_C11 1
#		else
#			if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#				define __CRT_HAS_C11 1
#			else 
#			endif // __STDC_VERSION__ >= 201112L
#		endif//__STDC_VERSION__ >= 201710L
#endif// __STDC_VERSION__ >= 202311L


#endif // !__CRT_CORE__
