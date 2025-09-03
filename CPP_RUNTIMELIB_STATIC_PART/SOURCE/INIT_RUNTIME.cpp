// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "INIT_RUNTIME.h"
#include "ALLOCATORS.h"
#include "CPPRUNTIME.h"
#define _CRT_ALLOCATE(X)__declspec(allocate(X)) // NOLINT
#pragma section(".CRT$XIA",long,read)
#pragma section(".CRT$XIZ",long,read)
#pragma section(".CRT$XCA",long,read)
#pragma section(".CRT$XCZ",long,read)
#pragma section(".CRT$XPA",long,read)
#pragma section(".CRT$XPZ",long,read)
#pragma section(".CRT$XTA",long,read)
#pragma section(".CRT$XTZ",long,read)


using _PVFV = void (*)(void);
//using _PIFV =  int (*)(void) //NOSONAR
extern "C" _CRT_ALLOCATE(".CRT$XIA") _PVFV __xi_a[] = { nullptr };//C initialization array start //NOLINT//NOSONAR
extern "C" _CRT_ALLOCATE(".CRT$XIZ") _PVFV __xi_z[] = {nullptr };//C initialization array end //NOLINT		//NOSONAR
extern "C" _CRT_ALLOCATE(".CRT$XCA") _PVFV __xc_a[] = { nullptr };//C++ initialization array start//NOLINT	//NOSONAR
extern "C" _CRT_ALLOCATE(".CRT$XCZ") _PVFV __xc_z[] = { nullptr };//C++ initialization array end//NOLINT	//NOSONAR
extern "C" _CRT_ALLOCATE(".CRT$XPA") _PVFV __xp_a[] = { nullptr }; // C pre-terminators (first)//NOLINT		//NOSONAR
extern "C" _CRT_ALLOCATE(".CRT$XPZ") _PVFV __xp_z[] = { nullptr }; // C pre-terminators (last)//NOLINT		//NOSONAR
extern "C" _CRT_ALLOCATE(".CRT$XTA") _PVFV __xt_a[] = { nullptr }; // C terminators (first)//NOLINT			//NOSONAR
extern "C" _CRT_ALLOCATE(".CRT$XTZ") _PVFV __xt_z[] = { nullptr }; // C terminators (last)//NOLINT			//NOSONAR
 static void _init_Ctors() {																				//NOSONAR
	// Call C initializers
	 for (_PVFV* p = &__xi_a[0]; p < &__xi_z[0]; ++p) {//NOLINT
		 __assume(p != nullptr);
		if (AS_BOOL(*p)) {//NOSONAR
			(*p)();
		}
	}
		// Call C++ initializers
	 for (_PVFV* p = &__xc_a[0]; p < &__xc_z[0]; ++p) {//NOLINT
		 __assume(p != nullptr);

		if (AS_BOOL(*p)) {//NOSONAR
			(*p)();
		}
	}
}
struct AtExitEntry {
	using AtExitfunc = void(*)();
	AtExitfunc func = nullptr;
};
struct AtExitList {
	AtExitEntry* Entries = nullptr;
	size_t count = 0;
	size_t capacity = 0;
};

static bool registerAtExit() {
	return false;
}
extern "C" 
{
	void initRuntime(const RuntimeInitParameters& InitParameters)
	{

		CreateHeap(InitParameters.initialHeapSize);
		_init_Ctors();
		::initParameters = InitParameters;// must be done after ctor calls as it would be overriden otherwise

		(void)registerAtExit();
	}
	
}

RuntimeInitParameters initParameters; //NOLINT //yeh i should do something about the globals...

void* GlobalState = nullptr;//NOLINT //yeh i should do something about the globals...
