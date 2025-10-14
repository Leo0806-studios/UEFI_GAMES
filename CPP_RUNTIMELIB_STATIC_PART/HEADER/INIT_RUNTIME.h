//###########################################
//				INIT_RUNTIME.h
//	Copyright (C) Leo0806 2025
//	Created -
//	Created by Leo0806
//	Last modified 03.09.2025
//	This file is part of the cpp runtime
//	contains forward declarations for runtime startup and typedefs for the init parameters.
// also contains global state 
//############################################
namespace SYSTEM::SUBSYSTEMS::WATCHDOG::SIMPLE {
	struct Watchdog;
}
#pragma once
struct __OS_CALLBACKS {
	
	void(*StackUnwind)(void* exceptionObject, void* context) = nullptr;
	void (*raiseException)(void* exceptionObject) = nullptr;
	 void(*TerminateProcess)(int exitCode) = nullptr;
	void(*TerminateThread)(int exitCode) = nullptr;
	void(*ExitProcess)(int exitCode) = nullptr;
	void(*ExitThread)(int exitCode) = nullptr;
	void* (*getystemInfo)() = nullptr;
	void* (*alocatePage)() = nullptr;
	void* (*allocatePages)(size_t amountOfPages) = nullptr;
	bool (*freePage)(void* ptr) = nullptr;
	bool(*freePages)(void* ptr) = nullptr;
	void* (*openFile)(const char* path) = nullptr;
	void  (*closeFile)(void* handle) = nullptr;
	size_t(*readFile)(void* handle, void* buffer, size_t size) = nullptr;
	size_t(*writeFile)(void* handle, const void* buffer, size_t size) = nullptr;
	void* (*CreateThread)(void(*startAddress)(void*), void* arg) = nullptr;
	void (*JoinThread)(void* threadHandle) = nullptr;
	void (*Sleep)(size_t milliseconds) = nullptr;
	void (*WriteLine)(const wchar_t* str) = nullptr;
	SYSTEM::SUBSYSTEMS::WATCHDOG::SIMPLE::	Watchdog* (*WatchDogStart)(__int64 Cycles) = nullptr;
	void (*WatchdogCheck)(SYSTEM::SUBSYSTEMS::WATCHDOG::SIMPLE::Watchdog*) = nullptr;
	bool (*WatchdogStop)(SYSTEM::SUBSYSTEMS::WATCHDOG::SIMPLE::Watchdog*) = nullptr;
	
};
struct sectioninfo {
	size_t offset =0;
	size_t size = 0;
};
struct RuntimeInitParameters {
	__OS_CALLBACKS callbacks = {};
	size_t initialHeapSize = 0;
	void* imageBaseAddress = nullptr;
	sectioninfo text= {0};//NOLINT
	sectioninfo data= {0};	  //NOLINT
	sectioninfo bss= {0};	  //NOLINT
	sectioninfo rdata= {0};	  //NOLINT
	sectioninfo pdata= {0};	  //NOLINT
	sectioninfo xdata = { 0 };//NOLINT
};


extern RuntimeInitParameters initParameters;//NOLINT //yeh i should do something about the globals...
extern "C" {
	extern void initRuntime(const RuntimeInitParameters& InitParameters);

}
extern void* GlobalState;//NOLINT //yeh i should do something about the globals...

#ifdef _DEBUG
#define WatchdogStart(cycles) ::initParameters.callbacks.WatchDogStart(cycles)
#define WatchDogCheck(WD) ::initParameters.callbacks.WatchdogCheck(WD)
#define WatchdogStop(WD) ::initParameters.callbacks.WatchdogStop(WD)
#define DebugPrint(msg) ::initParameters.callbacks.WriteLine((msg))
#else
#define DebugPrint(msg)
#endif // DEBUG


 