#pragma once
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#ifdef __cplusplus
}
#endif // __cplusplus

typedef struct {
	unsigned int* FramebufferBaseAddress;
	unsigned long long FramebufferSize;
	unsigned int FrabufferHeight;
	unsigned int FrabufferWidth;
	unsigned int PixelsPerScanLine;
	unsigned int PixelFormat;
}GOP_INFO;

typedef struct {
	EFI_SYSTEM_TABLE* SystemTable;
	EFI_BOOT_SERVICES* BootServices;
	EFI_RUNTIME_SERVICES* RuntimeServices;
} EFI_SYSTEM_INTERFACE;

/// <summary>
/// contains all the data that the bootloader needs to pass to the kernel or other chainloaded UEFI applications from this repo.
/// </summary>
typedef struct {
	EFI_SYSTEM_INTERFACE SystemInterface;
	GOP_INFO GraphicsOutputProtocolInfo;


	char** CmdlineArgs;


} BOOT_DATA_INTERFACE;