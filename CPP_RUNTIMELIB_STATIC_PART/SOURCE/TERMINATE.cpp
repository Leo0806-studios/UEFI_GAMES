// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "INIT_RUNTIME.h"
#include "CPPRUNTIME.h"
namespace std {
	void(*HTERMINATE_HANDLER)() = nullptr;

	_declspec(noreturn) void terminate() { //-V1082
		if (HTERMINATE_HANDLER) {
		HTERMINATE_HANDLER();

		}
		if (initParameters.callbacks.TerminateProcess) {
			initParameters.callbacks.TerminateProcess(-1);
		} else {
			while(true) {
				// If TerminateProcess is not set, we can only loop indefinitely
				// to prevent the program from continuing execution.
			}
		}
		while (true) {
			//this is here only to make the compiler happy
		}
	}
	void setTerminateHandler(void(*handler)())
	{
		HTERMINATE_HANDLER = handler;
	}
}

__declspec(noreturn) void abort() {
	std::terminate();
}