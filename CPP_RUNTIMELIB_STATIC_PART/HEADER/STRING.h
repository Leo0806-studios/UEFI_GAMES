#pragma once
#include "CRT_MACROS.h"

CRT_START

void* __cdecl memcpy(void* _Dst, const void* _Src, size_t _Size);

void* __cdecl memmove(void* _Dst, const void* _Src, size_t _Size);

void* __cdecl memset(void* _Dst, int _Val, size_t _Size);

CRT_END