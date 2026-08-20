//###########################################
//				ALLOCATORS.h
//	Copyright (C) Leo0806 2025
//	Created -
//	Created by Leo0806
//	Last modified 03.09.2025
//	This file is part of the cpp runtime
//	contains forward declarations for internal heap functionality
//############################################
#pragma once

#ifndef __CPP_RUNTIME_ALLOCATOR__
#define __CPP_RUNTIME_ALLOCATOR__

/// <summary>
/// Creates the ProgramHeap with a initial size.
/// InitSize is in pages.
/// when 0 it will create with one page
/// </summary>
/// <param name="initSize"></param>
/// <returns></returns>
bool CreateHeap(size_t initSize);
#endif // !__CPP_RUNTIME_ALLOCATOR__