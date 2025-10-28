//###########################################
//				CRT_MACROS.h
//	Copyright (C) Leo0806 2025
//	Created -
//	Created by Leo0806
//	Last modified 03.09.2025
//	This file is part of the cpp runtime
//	contains macros for the crt (and cpp rt)
//############################################

#pragma once
#define CRT_START extern "C"{

#define CRT_END }

#define __NOT_NULL(ptr) __assume( (ptr) != nullptr )