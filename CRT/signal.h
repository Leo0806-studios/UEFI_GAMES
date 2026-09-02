#pragma once
#ifndef __SIGNAL__
#define __SIGNAL__
#include "CRT_CORE.h"
#include "CRT_MACROS.h"
__CRT_START
typedef void (__CRT_CONV* __CRT_SIGNALE_HANDLER)(int);
typedef int sig_atomic_t;

#define SIG_DFL ((__CRT_SIGNALE_HANDLER)0)     
#define SIG_IGN ((__CRT_SIGNALE_HANDLER)1)    
#define SIG_ERR ((__CRT_SIGNALE_HANDLER)2)     

#define SIGSEGV 0
#define SIGABRT 1
#define SIGTERM 2
#define SIGFPE 3
#define SIGILL 4
#define SIGINT 5


__CRT_API __CRT_SIGNALE_HANDLER __CRT_CONV signal(int sig, __CRT_SIGNALE_HANDLER func);

__CRT_API int __CRT_CONV raise(int sig);
__CRT_END
#endif // !__SIGNAL__
