#pragma once
#ifndef __FENV__
#define __FENV__

#include "CRT_CORE.h"
#include "CRT_MACROS.h"
__CRT_START
typedef unsigned long fexcept_t;

typedef unsigned long long fenv_t;

int feclearexcept(int excepts);
int fetestexcept(int excepts);
int feraiseexcept(int excepts);
int fegetexceptflag(fexcept_t* flagp, int excepts);
int fesetexceptflag(const fexcept_t* flagp, int excepts);
int fesetround(int round);
int fegetround();
int fegetenv(fenv_t* envp);
int fesetenv(const fenv_t* envp);
int feholdexcept(fenv_t* envp);
int feupdateenv(const fenv_t* envp);
__CRT_END

#endif // !__FENV__
