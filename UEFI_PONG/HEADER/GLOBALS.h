#pragma once
#include <gnu-efi/inc/efi.h>
#include <stdbool.h>
#include <setjmp.h>
extern EFI_SYSTEM_TABLE* GlobalST;
#define NULLPTR ((void*)0)
#define nullptr NULLPTR
typedef struct {
	UINT32 Width;
	UINT32 Height;
	UINT32 PixelsPerScanLine;
	UINT32* FrameBuffer;
} Framebuffer ;
extern Framebuffer GlobalFramebuffer ;
