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

#define GOP_INFO_VERSION 0001
typedef struct {
	unsigned int Version;
	unsigned int Size;
	unsigned int* FramebufferBaseAddress;
	unsigned long long FramebufferSize;
	unsigned int FrabufferHeight;
	unsigned int FrabufferWidth;
	unsigned int PixelsPerScanLine;
	unsigned int PixelFormat;
}GOP_INFO;
#define EFI_SYSTEM_INTERFACE_VARSION 0001
typedef struct {
	unsigned int Version;
	unsigned int Size;
	EFI_SYSTEM_TABLE* SystemTable;
	EFI_BOOT_SERVICES* BootServices;
	EFI_RUNTIME_SERVICES* RuntimeServices;
} EFI_SYSTEM_INTERFACE;
#define SMBIOS_DATA_VERSION 0001
typedef struct {
	unsigned int Version;
	unsigned int Size;
	SMBIOS3_STRUCTURE_TABLE SMBios3Table;

}SMBIOS_DATA;

typedef struct {

	unsigned int Version;
	unsigned int Size;
	
}APCI_TABLES;

#define MP_DATA_VERSION 0001
typedef struct {
	unsigned int Version;
	unsigned int Size;
	unsigned long long CoreCount;
}MP_DATA;
#define BOOT_DATA_INTERFACE_VERSION 0001
/// <summary>
/// contains all the data that the bootloader needs to pass to the kernel or other chainloaded UEFI applications from this repo.
/// </summary>
typedef struct {
	unsigned int Version;
	unsigned int Size;
	EFI_SYSTEM_INTERFACE SystemInterface;
	GOP_INFO GraphicsOutputProtocolInfo;
	
	char** CmdlineArgs;


} BOOT_DATA_INTERFACE;
