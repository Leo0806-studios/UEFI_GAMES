#pragma once
//#include "stdbool.h"

#include "CRT_MACROS.h"
#include "CRT_CORE.h"
#include "stdint.h"
#include "wchar.h"
__CRT_START

typedef struct __CRT_OS_CALLBACKS
{
	
	void(*TerminateProcess)(int exitCode);
	void* (*alocatePage)(void) ;
	void* (*allocatePages)(size_t amountOfPages) ;
	bool (*freePage)(void* ptr) ;
	bool(*freePages)(void* ptr) ;
	void* (*openFile)(const char* path) ;
	void  (*closeFile)(void* handle);
	size_t(*readFile)(void* handle, void* buffer, size_t size) ;
	size_t(*writeFile)(void* handle, const void* buffer, size_t size) ;
	void (*WriteStdOutW)(const wchar_t* str) ;
	void (*WriteStdOut)(const char* str) ;
} __CRT_OS_CALLBACKS;
void TerminateProcess(int exitCode);
void* AlocatePage(void);
void* AllocatePages(size_t amountOfPages);
bool FreePage(void* ptr);
void* OpenFile(const char* path);
void CloseFile(void* handle);
size_t ReadFile(void* handle, void* buffer, size_t size);
size_t WriteFile(void* handle, const void* buffer, size_t size);
void WriteStdOutW(const wchar_t* str);
void WriteStdOut(const char* str);
extern int main(int argc, char** argv);
bool __MainCRTStartup(__CRT_OS_CALLBACKS* callbacks, int argc, char** argv);
__CRT_END