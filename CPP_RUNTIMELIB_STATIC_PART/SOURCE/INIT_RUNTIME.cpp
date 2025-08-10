#include "INIT_RUNTIME.h"
#define _CRT_ALLOCATE(X)__declspec(allocate(X))

#pragma section(".CRT$XIA",long,read)
#pragma section(".CRT$XIZ",long,read)
#pragma section(".CRT$XCA",long,read)
#pragma section(".CRT$XCZ",long,read)
#pragma section(".CRT$XPA",long,read)
#pragma section(".CRT$XPZ",long,read)
#pragma section(".CRT$XTA",long,read)
#pragma section(".CRT$XTZ",long,read)


using _PVFV = void (*)(void);
//using _PIFV =  int (*)(void)
extern "C" _CRT_ALLOCATE(".CRT$XIA") _PVFV __xi_a[] = { 0 };//C initialization array start
extern "C" _CRT_ALLOCATE(".CRT$XIZ") _PVFV __xi_z[] = { 0 };//C initialization array end
extern "C" _CRT_ALLOCATE(".CRT$XCA") _PVFV __xc_a[] = { 0 };//C++ initialization array start
extern "C" _CRT_ALLOCATE(".CRT$XCZ") _PVFV __xc_z[] = { 0 };//C++ initialization array end
extern "C" _CRT_ALLOCATE(".CRT$XPA") _PVFV __xp_a[] = { 0 }; // C pre-terminators (first)
extern "C" _CRT_ALLOCATE(".CRT$XPZ") _PVFV __xp_z[] = { 0 }; // C pre-terminators (last)
extern "C" _CRT_ALLOCATE(".CRT$XTA") _PVFV __xt_a[] = { 0 }; // C terminators (first)
extern "C" _CRT_ALLOCATE(".CRT$XTZ") _PVFV __xt_z[] = { 0 }; // C terminators (last)
void _init_Ctors() {
		// Call C++ initializers
	for (_PVFV* p = __xc_a; p < __xc_z; ++p) {
		if (*p) {
			(*p)();
		}
	}
	// Call C initializers
	for (_PVFV* p = __xi_a; p < __xi_z; ++p) {
		if (*p) {
			(*p)();
		}
	}
}
void initRuntime()
{
	_init_Ctors();
}
