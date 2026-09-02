#pragma once
#ifndef __STDDEF__
#define __STDDEF__
#include "CRT_CORE.h"
#include "CRT_MACROS.h"
__CRT_START
typedef unsigned long long max_align_t;

#define unreachable() __assume(false)
#ifdef __cplusplus
#define offsetof(s,m) __builtin_offsetof(s,m)
#else
#define offsetof(s,m) ((size_t)&(((s*)0)->m))
#endif // __cplusplus





 
__CRT_END
#endif // !__STDDEF__
