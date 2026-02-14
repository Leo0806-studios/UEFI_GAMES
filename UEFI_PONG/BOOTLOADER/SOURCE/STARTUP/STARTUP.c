#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#ifdef __cplusplus
}
#endif // __cplusplus
#include "STARTUP/STARTUP.h"
#include "GLOBALS.h"
#include <intrin.h>
static void MEMSET(char* dest, char value, size_t count) {
	for (size_t i = 0; i < count; i++) {
		dest[i] = value;
	}
}


EFI_STATUS InitBootloader(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable);

	MEMSET((char*)&BootloaderProtocols, 0, sizeof(BootloaderProtocols));
	MEMSET((char*)&BootInterface, 0, sizeof(BootInterface));

	EFI_STATUS Status;
	Status = uefi_call_wrapper(
		SystemTable->BootServices->LocateProtocol,
		3,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		(void**)&BootloaderProtocols.GOP
	);

	if (EFI_ERROR(Status)) {
		Print(L"Failed to locate GOP: %r\n", Status);
		return Status;
	}
	EFI_GUID gEfiMpServiceProtocolGuid = EFI_MP_SERVICES_PROTOCOL_GUID;
	Status = uefi_call_wrapper(
		SystemTable->BootServices->LocateProtocol,
		3,
		&gEfiMpServiceProtocolGuid,
		NULL,
		(void**)&BootloaderProtocols.MPP
	);
	if (EFI_ERROR(Status)) {
		Print(L"Failed to locate MPP: %r\n", Status);
		return Status;
	}
	BootInterface.Version = BOOT_DATA_INTERFACE_VERSION;

	BootInterface.SystemInterface = (EFI_SYSTEM_INTERFACE){ 
		.Version = EFI_SYSTEM_INTERFACE_VARSION,
		.SystemTable = SystemTable,
		.BootServices = SystemTable->BootServices,
		.RuntimeServices = SystemTable->RuntimeServices,
	};
	BootInterface.GraphicsOutputProtocolInfo = (GOP_INFO){
		.Version = GOP_INFO_VERSION,
		.FramebufferBaseAddress = (unsigned int*)BootloaderProtocols.GOP.Mode->FrameBufferBase,
		.FramebufferSize = BootloaderProtocols.GOP.Mode->FrameBufferSize,
		.FrabufferHeight = BootloaderProtocols.GOP.Mode->Info->VerticalResolution,
		.FrabufferWidth = BootloaderProtocols.GOP.Mode->Info->HorizontalResolution,
		.PixelsPerScanLine = BootloaderProtocols.GOP.Mode->Info->PixelsPerScanLine,
		.PixelFormat = BootloaderProtocols.GOP.Mode->Info->PixelFormat,
	};
	unsigned int numProcessors = 0;
	unsigned int numEnabledProcessors = 0;
	BootloaderProtocols.MPP.GetNumberOfProcessors(&BootloaderProtocols.MPP, numProcessors, numEnabledProcessors);
	


	for (unsigned long long index = 0; index < SystemTable->NumberOfTableEntries; index++) {
		EFI_CONFIGURATION_TABLE* configTable = &SystemTable->ConfigurationTable[index];
		Print(L"Config Table %d: %g\n", index, &configTable->VendorGuid);

	//insert giant if else chain to check fro all known guids and print them out for debugging purposes and later assign them to the boot interfacer
		if (CompareGuid(&configTable->VendorGuid, &gEfiSmbios3TableGuid)) {
			Print(L"Found SMBIOS 3 Table: %p\n", configTable->VendorTable);
			SMBIOS3_STRUCTURE_TABLE* smbios3Table = (SMBIOS3_STRUCTURE_TABLE*)configTable->VendorTable;
			Print(L"SMBIOS 3 Table Address: %p\n", smbios3Table);
			Print(L"SMBIOS 3 Table Size: %d\n", smbios3Table->TableMaximumSize);
		}
		//here for acpi https://uefi.org/sites/default/files/resources/ACPI_6_3_final_Jan30.pdf
	}

	return EFI_SUCCESS;
}