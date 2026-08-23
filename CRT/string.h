#pragma once
#ifndef __STRING__
#define __STRING__
#include <corecrt.h>
#include "CRT_MACROS.h"
__CRT_START
#pragma warning (push)
#pragma warning(disable: __DISABLE_CRT_WARNINGS)
#ifdef _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned int size_t;
#endif
#define NULL ((void*)0)

char* strcpy(char* restrict dest, const char* restrict src);
errno_t strcpy_s(char* restrict dest, rsize_t destsz, const char* restrict src);


char* strncpy(char* restrict dest, const char* restrict src, size_t count);
errno_t strncpy_s(char* restrict dest, rsize_t destsz,	const char* restrict src, rsize_t count);


char* strcat(char* restrict dest, const char* restrict src);
errno_t strcat_s(char* restrict dest, rsize_t destsz, const char* restrict src);


char* strncat(char* restrict dest, const char* restrict src, size_t count);
errno_t strncat_s(char* restrict dest, rsize_t destsz,	const char* restrict src, rsize_t count);


//size_t strxfrm(char* restrict dest, const char* restrict src, size_t count); //idk......

char* strdup(const char* src);


inline size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}

#pragma warning (pop)

__CRT_END
#endif // !__STRING__
