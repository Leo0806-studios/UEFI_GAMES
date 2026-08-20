//###########################################
//				LANGUAGE_EXTENSIONS.h
//	Copyright (C) Leo0806 2025
//	Created -
//	Created by Leo0806
//	Last modified 03.09.2025
//	This file is part of the cpp runtime
//	contains nonstandart extension o the language this runtime makes
// can be disabled with compiler flags ( set macros)
//############################################


#pragma once
#ifndef __LANG_EXTENSIONS_DISABLED__


/// <summary>
/// returns true if the current call is part of the unwinding process
/// </summary>
///  <returns></returns>
bool __isExceptionStack();
#endif // __LANG_EXTENSIONS