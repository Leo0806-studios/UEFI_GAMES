// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//###########################################
//				TERMINATE.cpp 
//	Copyright (C) Leo0806 2025
//	Created  -
//	Last modified 03.09.2025
//	This file is part of the CPP runtime 
//	this file contains the logic for std::terinate and for setting and storing a termination handler
//############################################
#include "INIT_RUNTIME.h"
#include "CPPRUNTIME.h"
namespace std {
static	void(*HTERMINATE_HANDLER)() = nullptr;

	_declspec(noreturn) void terminate() { //-V1082
		if (AS_BOOL(HTERMINATE_HANDLER)) {
		HTERMINATE_HANDLER();

		}
		if (AS_BOOL(initParameters.callbacks.TerminateProcess)) {
			initParameters.callbacks.TerminateProcess(-1);
		} else {
			while(true) {
				// If TerminateProcess is not set, we can only loop indefinitely
				// to prevent the program from continuing execution.
			}
		}
		while (true) {
			//this is here only to make the compiler happy bc i cant declare a  function pointer as noreturn (╯‵□′)╯︵┻━┻
		}
	}
	void setTerminateHandler(void(*handler)())
	{
		HTERMINATE_HANDLER = handler;
	}
}//namespace std

__declspec(noreturn) void abort() {
	std::terminate();
}