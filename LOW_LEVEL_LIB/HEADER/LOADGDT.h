#pragma once
typedef struct {
	unsigned short SizeBytes;
	unsigned int
}GdtDescriptor;


extern "C" void __stdcall LoadGDT()
