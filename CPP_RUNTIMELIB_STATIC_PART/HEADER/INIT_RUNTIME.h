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
};
struct RuntimeInitParameters {
	__OS_CALLBACKS callbacks = {};
	size_t initialHeapSize = 0;
};


extern RuntimeInitParameters initParameters;
extern "C" {
	extern void initRuntime(RuntimeInitParameters InitParameters);
}