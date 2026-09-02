#pragma once
#ifndef  __STDIO__
#define __STDIO__
#include "CRT_CORE.h"
#include "CRT_MACROS.h"
__CRT_START
typedef struct __CRT_FILE{ void* __placeholder; } FILE;
typedef __int64 fpos_t;
#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2
#define  BUFSIZ  512
#define EOF (-1)
#define FOPEN_MAX 1
#define FILENAME_MAX 260
#define _PRINTF_NAN_LEN_MAX (14) //sign + "NAN" +"("+ 8 ascii chars from least significant("rightmost") bit +")"
__CRT_END
#endif // ! __STDIO__
