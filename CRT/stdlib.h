#pragma once
#ifndef __STDLIB__
#define __STDLIB__
#
#include "ThreadingSharedSymbols.h"
#include "CRT_CORE.h"
#include "CRT_MACROS.h"
typedef struct _div_t
{
	int quot;
	int rem;
} div_t;

typedef struct _ldiv_t
{
	long quot;
	long rem;
} ldiv_t;

typedef struct _lldiv_t
{
	long long quot;
	long long rem;
} lldiv_t;
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define RAND_MAX 0x7fff
double atof(const char* str);
int atoi(const char* nptr);
long int atol(const char* nptr);
long long int atoll(const char* nptr);

//fuck this. ill bother with this later
//TODO: implement these functions
int strfromd(char* restrict s, size_t n, const char* restrict format,
	double fp);
int strfromf(char* restrict s, size_t n, const char* restrict format,
	float fp);
int strfroml(char* restrict s, size_t n, const char* restrict format,
	long double fp);

#ifdef __STDC_IEC_60559_DFP__
int strfromd32(char* restrict s, size_t n, const char* restrict format,
	_Decimal32 fp);
int strfromd64(char* restrict s, size_t n, const char* restrict format,
	_Decimal64 fp);
int strfromd128(char* restrict s, size_t n, const char* restrict format,
	_Decimal128 fp);
#endif

double strtod(const char* restrict nptr, char** restrict endptr);
float strtof(const char* restrict nptr, char** restrict endptr);
long double strtold(const char* restrict nptr, char** restrict endptr);

#ifdef __STDC_IEC_60559_DFP__
_Decimal32 strtod32(const char* restrict nptr, char** restrict endptr);
_Decimal64 strtod64(const char* restrict nptr, char** restrict endptr);
_Decimal128 strtod128(const char* restrict nptr, char** restrict endptr);
#endif





long  strtol(const char* restrict nptr, char** restrict endptr, int base);
long long  strtoll(const char* restrict nptr, char** restrict endptr, int base);
unsigned long  strtoul(const char* restrict nptr, char** restrict endptr, int base);
unsigned long long  strtoull(const char* restrict nptr, char** restrict endptr, int base);

int rand(void);
#endif // !__STDLIB__
