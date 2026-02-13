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
	EFI_SYSTEM_TABLE* SystemTable;
	EFI_BOOT_SERVICES* BootServices;
	EFI_RUNTIME_SERVICES* RuntimeServices;
} EFI_SYSTEM_INTERFACE;
#define SMBIOS_DATA_VERSION 0001
typedef struct {
	unsigned int Version;
	SMBIOS_STRUCTURE_POINTER SMBIOS;
	SMBIOS_STRUCTURE_TABLE* SmbiosTable;
	SMBIOS3_STRUCTURE_TABLE* SmbiosTable3;

}SMBIOS_DATA;
#define MP_DATA_VERSION 0001
typedef struct {
	unsigned long long CoreCount;
}MP_DATA;
#define BOOT_DATA_INTERFACE_VERSION 0001
/// <summary>
/// contains all the data that the bootloader needs to pass to the kernel or other chainloaded UEFI applications from this repo.
/// </summary>
typedef struct {
	unsigned int Version;
	EFI_SYSTEM_INTERFACE SystemInterface;
	GOP_INFO GraphicsOutputProtocolInfo;
	SMBIOS_DATA SmbiosData;
	
	char** CmdlineArgs;


} BOOT_DATA_INTERFACE;
