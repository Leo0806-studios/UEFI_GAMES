#pragma once
#pragma warning(push, 0)
#include <efi.h>
#include <stdbool.h>
#include "BOOT_DATA_INTERFACE.h"
#pragma warning(pop)
extern EFI_SYSTEM_TABLE* GlobalST;
#define NULLPTR ((void*)0)
#ifndef __cplusplus

#define nullptr NULLPTR
#endif // !__cplusplus

typedef struct {
	EFI_GRAPHICS_OUTPUT_PROTOCOL GOP;
	EFI_MP_SERVICES_PROTOCOL MPP;
	//EFI_SMBIOS_PROTOCOL SMBIOS; GRRRRRRR gnuefi doesnt support this protocol for some reason. i rly should switch to EDK II

} BOOTLOADER_PROTOCOLS;
extern BOOT_DATA_INTERFACE BootInterface;
extern BOOTLOADER_PROTOCOLS BootloaderProtocols;