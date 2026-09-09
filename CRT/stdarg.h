#pragma once 
#ifndef __STDARG__
#define __STDARG__
#include "CRT_CORE.h"
#include "CRT_MACROS.h"
__CRT_START
typedef unsigned char* va_list;
void __cdecl __va_start(va_list*, ...); //stupid ms compatibility compiler magic
#define va_start(ap, v) ((void)(__va_start(&ap, &v)))
#define va_arg(ap, T)                                                   \
    ((sizeof(T) > 8 || (sizeof(T) & (sizeof(T) - 1)) != 0)            \
        ? **(T **)((ap += 8) - 8)                                      \
        : *(T *)((ap += 8) - 8))				//fucking disgusting
#define va_end(ap) ((void)(ap = (va_list)0))
#define  va_copy(dest, src) ((dest) = (src))
__CRT_END
#endif // !__STDARG__
