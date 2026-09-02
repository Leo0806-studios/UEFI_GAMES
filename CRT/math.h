
//###########################################
//				math.h 
//	Copyright (C) Leo0806 2026
//	Created 26.08.2026

//	Last modified 30.08.2026
//	This file is part of CRT
//	math.h implementation of the C standart library.
// 
//	NOTE : Sections marked between msvc start and msvc end are taken from existing msvc headers and licenced under the MIT licence.
//	3rd party attribution is inside the LICENCES/CRT/ directory with files named after the origins of the code.
//############################################

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
int __iseqsigFloat(float x, float y);
int __iseqsigDouble(double x, double y);
int __iseqsigLongDouble(long double x, long double y);
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

#define iseqsig(X) (_CLASSIFY2(X, X, __iseqsigFloat, __iseqsigDouble, __iseqsigLongDouble) != 0)

#define iscanonical(x) (fpclassify(x) == FP_NORMAL    || \
						fpclassify(x) == FP_SUBNORMAL || \
						fpclassify(x) == FP_ZERO      || \
						fpclassify(x) == FP_INFINITE  || \
						fpclassify(x) == FP_NAN)
#define issignaling(X) ((fpclassify(X) == FP_NAN) && ((_FPCOMPARE(X,NAN)&_FP_EQ) != 0))
#define issubnormal(x) (fpclassify(x) == FP_SUBNORMAL)
#define iszero(x) (fpclassify(x) == FP_ZERO)

#ifdef __CRT_DONT_USE_X87
#define __CRT_TRIG_FUNC(X) __SOFT_##X
#include "MATH_SOFT_TRIG.h"
#else
#define  __CRT_TRIG_FUNC(X) __X87_##X
#include "MATH_X87_TRIG.h"
#endif


//|------------------- TRIGONOMETRIC FUNCTIONS -------------------|


inline double acos(double x) {
	return __CRT_TRIG_FUNC(acos)(x);
}
inline float acosf(float x) {
	return __CRT_TRIG_FUNC(acosf)(x);
}
inline long double acosl(long double x) {
	return __CRT_TRIG_FUNC(acosl)(x);
}


inline double asin(double x) {
	return __CRT_TRIG_FUNC(asin)(x);
}
inline float asinf(float x) {
	return __CRT_TRIG_FUNC(asinf)(x);
}
inline long double asinl(long double x) {
	return __CRT_TRIG_FUNC(asinl)(x);
}

inline double atan(double x) {
	return __CRT_TRIG_FUNC(atan)(x);
}
inline float atanf(float x) {
	return __CRT_TRIG_FUNC(atanf)(x);
}
inline long double atanl(long double x) {
	return __CRT_TRIG_FUNC(atanl)(x);
}

inline double atan2(double y, double x) {
	return __CRT_TRIG_FUNC(atan2)(y, x);
}
inline float atan2f(float y, float x) {
	return __CRT_TRIG_FUNC(atan2f)(y, x);
}
inline long double atan2l(long double y, long double x) {
	return __CRT_TRIG_FUNC(atan2l)(y, x);
}

inline double cos(double x) {
	return __CRT_TRIG_FUNC(cos)(x);
}
inline float cosf(float x) {
	return __CRT_TRIG_FUNC(cosf)(x);
}
inline long double cosl(long double x) {
	return __CRT_TRIG_FUNC(cosl)(x);
}

inline double sin(double x) {
	return __CRT_TRIG_FUNC(sin)(x);
}
inline float sinf(float x) {
	return __CRT_TRIG_FUNC(sinf)(x);
}
inline long double sinl(long double x) {
	return __CRT_TRIG_FUNC(sinl)(x);
}


inline double tan(double x) {
	return __CRT_TRIG_FUNC(tan)(x);
}
inline float tanf(float x) {
	return __CRT_TRIG_FUNC(tanf)(x);
}
inline long double tanl(long double x) {
	return __CRT_TRIG_FUNC(tanl)(x);
}

inline double acospi(double x) {
	return __CRT_TRIG_FUNC(acospi)(x);
}
inline float acospif(float x) {
	return __CRT_TRIG_FUNC(acospif)(x);
}
inline long double acospil(long double x) {
	return __CRT_TRIG_FUNC(acospil)(x);
}

inline double asinpi(double x) {
	return __CRT_TRIG_FUNC(asinpi)(x);
}
inline float asinpif(float x) {
	return __CRT_TRIG_FUNC(asinpif)(x);
}
inline long double asinpil(long double x) {
	return __CRT_TRIG_FUNC(asinpil)(x);
}

inline double atanpi(double x) {
	return __CRT_TRIG_FUNC(atanpi)(x);
}
inline float atanpif(float x) {
	return __CRT_TRIG_FUNC(atanpif)(x);
}
inline long double atanpil(long double x) {
	return __CRT_TRIG_FUNC(atanpil)(x);
}

inline double atan2pi(double y, double x) {
	return __CRT_TRIG_FUNC(atan2pi)(y, x);
}
inline float atan2pif(float y, float x) {
	return __CRT_TRIG_FUNC(atan2pif)(y, x);
}
inline long double atan2pil(long double y, long double x) {
	return __CRT_TRIG_FUNC(atan2pil)(y, x);
}

inline double cospi(double x) {
	return __CRT_TRIG_FUNC(cospi)(x);
}
inline float cospif(float x) {
	return __CRT_TRIG_FUNC(cospif)(x);
}
inline long double cospil(long double x) {
	return __CRT_TRIG_FUNC(cospil)(x);
}

inline double sinpi(double x) {
	return __CRT_TRIG_FUNC(sinpi)(x);
}
inline float sinpif(float x) {
	return __CRT_TRIG_FUNC(sinpif)(x);
}
inline long double sinpil(long double x) {
	return __CRT_TRIG_FUNC(sinpil)(x);
}

inline double tanpi(double x) {
	return __CRT_TRIG_FUNC(tanpi)(x);
}
inline float tanpif(float x) {
	return __CRT_TRIG_FUNC(tanpif)(x);
}
inline long double tanpil(long double x) {
	return __CRT_TRIG_FUNC(tanpil)(x);
}


//|------------------- HYPERBOLIC FUNCTIONS -------------------| 



__CRT_API double __CRT_CONV asinh(double x);
__CRT_API float __CRT_CONV asinhf(float x);
__CRT_API long double __CRT_CONV asinhl(long double x);

__CRT_API double __CRT_CONV atanh(double x);
__CRT_API float __CRT_CONV atanhf(float x);
__CRT_API long double __CRT_CONV atanhl(long double x);

__CRT_API double __CRT_CONV cosh(double x);
__CRT_API float __CRT_CONV coshf(float x);
__CRT_API long double __CRT_CONV coshl(long double x);

__CRT_API double __CRT_CONV sinh(double x);
__CRT_API float __CRT_CONV sinhf(float x);
__CRT_API long double __CRT_CONV sinhl(long double x);

__CRT_API double __CRT_CONV tanh(double x);
__CRT_API float __CRT_CONV tanhf(float x);
__CRT_API long double __CRT_CONV tanhl(long double x);


//|------------------- EXPONENTIAL AND LOGARITHMIC FUNCTIONS -------------------|



__CRT_API double __CRT_CONV exp(double x);
__CRT_API float __CRT_CONV expf(float x);
__CRT_API long double __CRT_CONV expl(long double x);

__CRT_API double __CRT_CONV exp10(double x);
__CRT_API float __CRT_CONV exp10f(float x);
__CRT_API long double __CRT_CONV exp10l(long double x);

__CRT_API double __CRT_CONV exp10m1(double x);
__CRT_API float __CRT_CONV exp10m1f(float x);
__CRT_API long double __CRT_CONV exp10m1l(long double x);

__CRT_API double __CRT_CONV exp2(double x);
__CRT_API float __CRT_CONV exp2f(float x);
__CRT_API long double __CRT_CONV exp2l(long double x);

__CRT_API double __CRT_CONV exp2m1(double x);
__CRT_API float __CRT_CONV exp2m1f(float x);
__CRT_API long double __CRT_CONV exp2m1l(long double x);

__CRT_API double __CRT_CONV expm1(double x);
__CRT_API float __CRT_CONV expm1f(float x);
__CRT_API long double __CRT_CONV expm1l(long double x);

__CRT_API double __CRT_CONV frexp(double value, int* p);
__CRT_API float __CRT_CONV frexpf(float value, int* p);
__CRT_API long double __CRT_CONV frexpl(long double value, int* p);

__CRT_API int __CRT_CONV ilogb(double x);
__CRT_API int __CRT_CONV ilogbf(float x);
__CRT_API int __CRT_CONV ilogbl(long double x);

__CRT_API double __CRT_CONV ldexp(double x, int p);
__CRT_API float __CRT_CONV ldexpf(float x, int p);
__CRT_API long double __CRT_CONV ldexpl(long double x, int p);

__CRT_API long int __CRT_CONV llogb(double x);
__CRT_API long int __CRT_CONV llogbf(float x);
__CRT_API long int __CRT_CONV llogbl(long double x);

__CRT_API double __CRT_CONV log(double x);
__CRT_API float __CRT_CONV logf(float x);
__CRT_API long double __CRT_CONV logl(long double x);

__CRT_API double __CRT_CONV log10(double x);
__CRT_API float __CRT_CONV log10f(float x);
__CRT_API long double __CRT_CONV log10l(long double x);

__CRT_API double __CRT_CONV log10p1(double x);
__CRT_API float __CRT_CONV log10p1f(float x);
__CRT_API long double __CRT_CONV log10p1l(long double x);

__CRT_API double __CRT_CONV log1p(double x);
__CRT_API float __CRT_CONV log1pf(float x);
__CRT_API long double __CRT_CONV log1pl(long double x);
__CRT_API double __CRT_CONV logp1(double x);
__CRT_API float __CRT_CONV logp1f(float x);
__CRT_API long double __CRT_CONV logp1l(long double x);

__CRT_API double __CRT_CONV log2p1(double x);
__CRT_API float __CRT_CONV log2p1f(float x);
__CRT_API long double __CRT_CONV log2p1l(long double x);

__CRT_API double __CRT_CONV logb(double x);
__CRT_API float __CRT_CONV logbf(float x);
__CRT_API long double __CRT_CONV logbl(long double x);

__CRT_API double __CRT_CONV modf(double value, double* iptr);
__CRT_API float __CRT_CONV modff(float value, float* iptr);
__CRT_API long double __CRT_CONV modfl(long double value, long double* iptr);

__CRT_API double __CRT_CONV scalbn(double x, int n);
__CRT_API float __CRT_CONV scalbnf(float x, int n);
__CRT_API long double __CRT_CONV scalbnl(long double x, int n);
__CRT_API double __CRT_CONV scalbln(double x, long int n);
__CRT_API float __CRT_CONV scalblnf(float x, long int n);
__CRT_API long double __CRT_CONV scalblnl(long double x, long int n);


//|------------------- POWER FUNCTIONS -------------------|


__CRT_API double __CRT_CONV cbrt(double x);
__CRT_API float __CRT_CONV cbrtf(float x);
__CRT_API long double __CRT_CONV cbrtl(long double x);

__CRT_API double __CRT_CONV compoundn(double x, long long int n);
__CRT_API float __CRT_CONV compoundnf(float x, long long int n);
__CRT_API long double __CRT_CONV compoundnl(long double x, long long int n);

__CRT_API double __CRT_CONV fabs(double x);
__CRT_API float __CRT_CONV fabsf(float x);
__CRT_API long double __CRT_CONV fabsl(long double x);

__CRT_API double __CRT_CONV hypot(double x, double y);
__CRT_API float __CRT_CONV hypotf(float x, float y);
__CRT_API long double __CRT_CONV hypotl(long double x, long double y);

__CRT_API double __CRT_CONV pow(double x, double y);
__CRT_API float __CRT_CONV powf(float x, float y);
__CRT_API long double __CRT_CONV powl(long double x, long double y);

__CRT_API double __CRT_CONV pown(double x, long long int n);
__CRT_API float __CRT_CONV pownf(float x, long long int n);
__CRT_API long double __CRT_CONV pownl(long double x, long long int n);

__CRT_API double __CRT_CONV powr(double y, double x);
__CRT_API float __CRT_CONV powrf(float y, float x);
__CRT_API long double __CRT_CONV powrl(long double y, long double x);

__CRT_API double __CRT_CONV rootn(double x, long long int n);
__CRT_API float __CRT_CONV rootnf(float x, long long int n);
__CRT_API long double __CRT_CONV rootnl(long double x, long long int n);

__CRT_API double __CRT_CONV rsqrt(double x);
__CRT_API float __CRT_CONV rsqrtf(float x);
__CRT_API long double __CRT_CONV rsqrtl(long double x);

__CRT_API double __CRT_CONV sqrt(double x);
__CRT_API float __CRT_CONV sqrtf(float x);
__CRT_API long double __CRT_CONV sqrtl(long double x);


//|------------------- Error and Gamma Functions -------------------|


__CRT_API double __CRT_CONV erf(double x);
__CRT_API float __CRT_CONV erff(float x);
__CRT_API long double __CRT_CONV erfl(long double x);

__CRT_API double __CRT_CONV erfc(double x);
__CRT_API float __CRT_CONV erfcf(float x);
__CRT_API long double __CRT_CONV erfcl(long double x);

__CRT_API double __CRT_CONV lgamma(double x);
__CRT_API float __CRT_CONV lgammaf(float x);
__CRT_API long double __CRT_CONV lgammal(long double x);

__CRT_API double __CRT_CONV tgamma(double x);
__CRT_API float __CRT_CONV tgammaf(float x);
__CRT_API long double __CRT_CONV tgammal(long double x);


//|------------------- Nearest Integer Functions -------------------|


__CRT_API double __CRT_CONV ceil(double x);
__CRT_API float __CRT_CONV ceilf(float x);
__CRT_API long double __CRT_CONV ceill(long double x);

__CRT_API double __CRT_CONV floor(double x);
__CRT_API float __CRT_CONV floorf(float x);
__CRT_API long double __CRT_CONV floorl(long double x);

__CRT_API double __CRT_CONV nearbyint(double x);
__CRT_API float __CRT_CONV nearbyintf(float x);
__CRT_API long double __CRT_CONV nearbyintl(long double x);

__CRT_API double __CRT_CONV rint(double x);
__CRT_API float __CRT_CONV rintf(float x);
__CRT_API long double __CRT_CONV rintl(long double x);

__CRT_API long int __CRT_CONV lrint(double x);
__CRT_API long int __CRT_CONV lrintf(float x);
__CRT_API long int __CRT_CONV lrintl(long double x);
__CRT_API long long int __CRT_CONV llrint(double x);
__CRT_API long long int __CRT_CONV llrintf(float x);
__CRT_API long long int __CRT_CONV llrintl(long double x);

__CRT_API double __CRT_CONV round(double x);
__CRT_API float __CRT_CONV roundf(float x);
__CRT_API long double __CRT_CONV roundl(long double x);

__CRT_API long int __CRT_CONV lround(double x);
__CRT_API long int __CRT_CONV lroundf(float x);
__CRT_API long int __CRT_CONV lroundl(long double x);
__CRT_API long long int __CRT_CONV llround(double x);
__CRT_API long long int __CRT_CONV llroundf(float x);
__CRT_API long long int __CRT_CONV llroundl(long double x);

__CRT_API double __CRT_CONV roundeven(double x);
__CRT_API float __CRT_CONV roundevenf(float x);
__CRT_API long double __CRT_CONV roundevenl(long double x);

__CRT_API double __CRT_CONV trunc(double x);
__CRT_API float __CRT_CONV truncf(float x);
__CRT_API long double __CRT_CONV truncl(long double x);

__CRT_API double __CRT_CONV fromfp(double x, int rnd, unsigned int width);
__CRT_API float __CRT_CONV fromfpf(float x, int rnd, unsigned int width);
__CRT_API long double __CRT_CONV fromfpl(long double x, int rnd, unsigned int width);
__CRT_API double __CRT_CONV ufromfp(double x, int rnd, unsigned int width);
__CRT_API float __CRT_CONV ufromfpf(float x, int rnd, unsigned int width);
__CRT_API long double __CRT_CONV ufromfpl(long double x, int rnd, unsigned int width);

__CRT_API double __CRT_CONV fromfpx(double x, int rnd, unsigned int width);
__CRT_API float __CRT_CONV fromfpxf(float x, int rnd, unsigned int width);
__CRT_API long double __CRT_CONV fromfpxl(long double x, int rnd, unsigned int width);
__CRT_API double __CRT_CONV ufromfpx(double x, int rnd, unsigned int width);
__CRT_API float __CRT_CONV ufromfpxf(float x, int rnd, unsigned int width);
__CRT_API long double __CRT_CONV ufromfpxl(long double x, int rnd, unsigned int width);


//|------------------- Remainder Functions -------------------|


__CRT_API double __CRT_CONV fmod(double x, double y);
__CRT_API float __CRT_CONV fmodf(float x, float y);
__CRT_API long double __CRT_CONV fmodl(long double x, long double y);

__CRT_API double __CRT_CONV remainder(double x, double y);
__CRT_API float __CRT_CONV remainderf(float x, float y);
__CRT_API long double __CRT_CONV remainderl(long double x, long double y);

__CRT_API double __CRT_CONV remquo(double x, double y, int* quo);
__CRT_API float __CRT_CONV remquof(float x, float y, int* quo);
__CRT_API long double __CRT_CONV remquol(long double x, long double y, int* quo);


//|------------------- Manipulation Functions -------------------|

__CRT_API double __CRT_CONV copysign(double x, double y);
__CRT_API float __CRT_CONV copysignf(float x, float y);
__CRT_API long double __CRT_CONV copysignl(long double x, long double y);

__CRT_API double __CRT_CONV nan(const char* tagp);
__CRT_API float __CRT_CONV nanf(const char* tagp);
__CRT_API long double __CRT_CONV nanl(const char* tagp);

__CRT_API double __CRT_CONV nextafter(double x, double y);
__CRT_API float __CRT_CONV nextafterf(float x, float y);
__CRT_API long double __CRT_CONV nextafterl(long double x, long double y);

__CRT_API double __CRT_CONV nexttoward(double x, long double y);
__CRT_API float __CRT_CONV nexttowardf(float x, long double y);
__CRT_API long double __CRT_CONV nexttowardl(long double x, long double y);

__CRT_API double __CRT_CONV nextup(double x);
__CRT_API float __CRT_CONV nextupf(float x);
__CRT_API long double __CRT_CONV nextupl(long double x);

__CRT_API double __CRT_CONV nextdown(double x);
__CRT_API float __CRT_CONV nextdownf(float x);
__CRT_API long double __CRT_CONV nextdownl(long double x);

__CRT_API int __CRT_CONV canonicalize(double* cx, const double* x);
__CRT_API int __CRT_CONV canonicalizef(float* cx, const float* x);
__CRT_API int __CRT_CONV canonicalizel(long double* cx, const long double* x);


//|------------------- Maximum, minimum and positive difference functions -----	--------------|


__CRT_API double __CRT_CONV fdim(double x, double y);
__CRT_API float __CRT_CONV fdimf(float x, float y);
__CRT_API long double __CRT_CONV fdiml(long double x, long double y);

__CRT_API double __CRT_CONV fmax(double x, double y);
__CRT_API float __CRT_CONV fmaxf(float x, float y);
__CRT_API long double __CRT_CONV fmaxl(long double x, long double y);

__CRT_API double __CRT_CONV fmin(double x, double y);
__CRT_API float __CRT_CONV fminf(float x, float y);
__CRT_API long double __CRT_CONV fminl(long double x, long double y);

__CRT_API double __CRT_CONV fmaximum(double x, double y);
__CRT_API float __CRT_CONV fmaximumf(float x, float y);
__CRT_API long double __CRT_CONV fmaximuml(long double x, long double y);

__CRT_API double __CRT_CONV fminimum(double x, double y);
__CRT_API float __CRT_CONV fminimumf(float x, float y);
__CRT_API long double __CRT_CONV fminimuml(long double x, long double y);

__CRT_API double __CRT_CONV fmaximum_mag(double x, double y);
__CRT_API float __CRT_CONV fmaximum_magf(float x, float y);
__CRT_API long double __CRT_CONV fmaximum_magl(long double x, long double y);

__CRT_API double __CRT_CONV fminimum_mag(double x, double y);
__CRT_API float __CRT_CONV fminimum_magf(float x, float y);
__CRT_API long double __CRT_CONV fminimum_magl(long double x, long double y);

__CRT_API double __CRT_CONV fmaximum_num(double x, double y);
__CRT_API float __CRT_CONV fmaximum_numf(float x, float y);
__CRT_API long double __CRT_CONV fmaximum_numl(long double x, long double y);

__CRT_API double __CRT_CONV fminimum_num(double x, double y);
__CRT_API float __CRT_CONV fminimum_numf(float x, float y);
__CRT_API long double __CRT_CONV fminimum_numl(long double x, long double y);

__CRT_API double __CRT_CONV fmaximum_mag_num(double x, double y);
__CRT_API float __CRT_CONV fmaximum_mag_numf(float x, float y);
__CRT_API long double __CRT_CONV fmaximum_mag_numl(long double x, long double y);

__CRT_API double __CRT_CONV fminimum_mag_num(double x, double y);
__CRT_API float __CRT_CONV fminimum_mag_numf(float x, float y);
__CRT_API long double __CRT_CONV fminimum_mag_numl(long double x, long double y);


//|------------------- Fused multiply-add functions -------------------|



__CRT_API double __CRT_CONV fma(double x, double y, double z);
__CRT_API float __CRT_CONV fmaf(float x, float y, float z);
__CRT_API long double __CRT_CONV fmal(long double x, long double y, long double z);


//|------------------- Functions that Round result to narrower type -------------------|


__CRT_API float __CRT_CONV fadd(double x, double y);
__CRT_API float __CRT_CONV faddl(long double x, long double y);
__CRT_API double __CRT_CONV daddl(long double x, long double y);

__CRT_API float __CRT_CONV fsub(double x, double y);
__CRT_API float __CRT_CONV fsubl(long double x, long double y);
__CRT_API double __CRT_CONV dsubl(long double x, long double y);

__CRT_API float __CRT_CONV fmul(double x, double y);
__CRT_API float __CRT_CONV fmull(long double x, long double y);
__CRT_API double __CRT_CONV dmull(long double x, long double y);

__CRT_API float __CRT_CONV fdiv(double x, double y);
__CRT_API float __CRT_CONV fdivl(long double x, long double y);
__CRT_API double __CRT_CONV ddivl(long double x, long double y);

__CRT_API float __CRT_CONV ffma(double x, double y, double z);
__CRT_API float __CRT_CONV ffmal(long double x, long double y, long double z);
__CRT_API double __CRT_CONV dfmal(long double x, long double y, long double z);

__CRT_API float __CRT_CONV fsqrt(double x);
__CRT_API float __CRT_CONV fsqrtl(long double x);
__CRT_API double __CRT_CONV dsqrtl(long double x);

//
__CRT_END
#endif // !__MATH__
