//###########################################
//				STRING.h 
//	Copyright (C) Leo0806 2025
//	Created -
//	Created by Leo0806
//	Last modified 03.09.2025
//	This file is part of the cpp runtime
//	contaions c string function forward declarations
//############################################

#pragma once
#include "CRT_MACROS.h"


CRT_START

void* __cdecl memcpy(void* _Dst, const void* _Src, size_t _Size);

void* __cdecl memmove(void* _Dst, const void* _Src, size_t _Size);

void* __cdecl memset(void* _Dst, int _Val, size_t _Size);

CRT_END