#pragma once
#include <stdint.h>
#include <gnu_efi/efi.h>
#include <gnu_efi/efiapi.h>
typedef struct {
	const char* Name;
	uint64_t Children;
	uint64_t Size;
	void* Handle;
}DirectoryInfo;
typedef struct {
	const char* Name;
	uint64_t Size;
	void* Handle;
	uint8_t* Data;
}FileInfo;
typedef struct {
	const char* Name;
	uint64_t Size;
	uint64_t Free;
	void* Handle;
}VolumeInfo;

EFI_STATUS InitFIleSyste(void);

VolumeInfo* OpenBootDeviceVolume(void);

