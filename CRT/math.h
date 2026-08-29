#pragma once
#ifndef __MATH__
#define __MATH__

#include "CRT_CORE.h"
#include "CRT_MACROS.h"
#include "stdbool.h"
__CRT_START
typedef float  float_t;
typedef double double_t;


#define HOPEFULLY_BIG_NUMBER 1e+300
#define INFINITY  ((float)HOPEFULLY_BIG_NUMBER) //hell yeah
#define HUGE_VAL   ((double)INFINITY)
#define HUGE_VALF  ((float)INFINITY)
#define HUGE_VALL  ((long double)INFINITY)
#define NAN  (__CRT_C_BITCAST(float, 0b01111111110000000000000000000000))


//msvc start
#define _DENORM    (-2)
#define _FINITE    (-1)
#define _INFCODE   1
#define _NANCODE   2
#define FP_INFINITE  _INFCODE
#define FP_NAN       _NANCODE
#define FP_NORMAL    _FINITE
#define FP_SUBNORMAL _DENORM
#define FP_ZERO      0
//msvc end


#define FP_INT_UPWARD 1
#define FP_INT_DOWNWARD 2
#define FP_INT_TOWARDZERO 3
#define FP_INT_TONEARESTFROMZERO 4
#define FP_INT_TONEAREST 5
#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
#define math_errhandling  (MATH_ERRNO | MATH_ERREXCEPT)

int __classifyFloat(float x);
int __classifyDouble(double x);
int __classifyLongDouble(long double x);
int __compareFloat(float x, float y);
int __compareDouble(double x, double y);
int __compareLongDouble(long double x, long double y);
int __signbitFloat(float x);
int __signbitDouble(double x);
int __signbitLongDouble(long double x);
bool __isQuietNaNFloat(float x);
bool __isQuietNaNDouble(double x);
bool __isQuietNaNLongDouble(long double x);
typedef enum _FpCompareEnum
{
	_FP_LT = 1,
	_FP_EQ = 2,
	_FP_GT = 4
} _FpCompareEnum;
//i have no fuckingclue how this worls but its from the msvc crt so it has to be right
//msvc start
#define _CLASS_ARG(_Val)                                  __pragma(warning(suppress:6334))(sizeof ((_Val) + (float)0) == sizeof (float) ? 'f' : sizeof ((_Val) + (double)0) == sizeof (double) ? 'd' : 'l')
#define _CLASSIFY(_Val, _FFunc, _DFunc, _LDFunc)          (_CLASS_ARG(_Val) == 'f' ? _FFunc((float)(_Val)) : _CLASS_ARG(_Val) == 'd' ? _DFunc((double)(_Val)) : _LDFunc((long double)(_Val)))
#define _CLASSIFY2(_Val1, _Val2, _FFunc, _DFunc, _LDFunc) (_CLASS_ARG((_Val1) + (_Val2)) == 'f' ? _FFunc((float)(_Val1), (float)(_Val2)) : _CLASS_ARG((_Val1) + (_Val2)) == 'd' ? _DFunc((double)(_Val1), (double)(_Val2)) : _LDFunc((long double)(_Val1), (long double)(_Val2)))


#define fpclassify(_Val)      (_CLASSIFY(_Val, __classifyFloat, __classifyDouble, __classifyLongDouble))

#define _FPCOMPARE(_Val1, _Val2) (_CLASSIFY2(_Val1, _Val2, __compareFloat, __compareDouble, __compareLongDouble))

#define isfinite(_Val)      (fpclassify(_Val) <= 0)
#define isinf(_Val)         (fpclassify(_Val) == FP_INFINITE)
#define isnan(_Val)         (fpclassify(_Val) == FP_NAN)
#define isnormal(_Val)      (fpclassify(_Val) == FP_NORMAL)
#define signbit(_Val)       (_CLASSIFY(_Val, __signbitFloat, __signbitDouble, __signbitLongDouble))

#define isgreater(x, y)      ((_FPCOMPARE(x, y) & _FP_GT) != 0)
#define isgreaterequal(x, y) ((_FPCOMPARE(x, y) & (_FP_EQ | _FP_GT)) != 0)
#define isless(x, y)         ((_FPCOMPARE(x, y) & _FP_LT) != 0)
#define islessequal(x, y)    ((_FPCOMPARE(x, y) & (_FP_LT | _FP_EQ)) != 0)
#define islessgreater(x, y)  ((_FPCOMPARE(x, y) & (_FP_LT | _FP_GT)) != 0)
#define isunordered(x, y)    (_FPCOMPARE(x, y) == 0)
//msvc end

#define iscanonical(x) (fpclassify(x) == FP_NORMAL    || \
						fpclassify(x) == FP_SUBNORMAL || \
						fpclassify(x) == FP_ZERO      || \
						fpclassify(x) == FP_INFINITE  || \
						fpclassify(x) == FP_NAN)
#define issignaling(X) ((fpclassify(X) == FP_NAN) && ((_FPCOMPARE(X,NAN)&_FP_EQ) != 0))
#define issubnormal(x) (fpclassify(x) == FP_SUBNORMAL)
#define iszero(x) (fpclassify(x) == FP_ZERO)

double acos(double x);
float acosf(float x);
long double acosl(long double x);
__CRT_END
#endif // !__MATH__
