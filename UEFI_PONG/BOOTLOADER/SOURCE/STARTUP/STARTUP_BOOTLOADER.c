#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#pragma warning (push,0)
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>

#pragma warning (pop)
#ifdef __cplusplus
}
#endif // __cplusplus
#include <intrin.h>
#include "GLOBALS.h"
#include "HEAP/HEAP.h"
#include "STARTUP/STARTUP.h"
#include "SUB/SUB_FORWARD_DECLARATIONS.h"
EFI_STATUS CollectProtocolls(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);

EFI_STATUS InitBootloader(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	Print(L"Bootloader Startup...\n");
	CreatHeap(MiB(20));
	MEMSET((char*)&BootloaderProtocols, 0, sizeof(BootloaderProtocols));
	MEMSET((char*)&BootInterface, 0, sizeof(BootInterface));
	EFI_STATUS status = CollectProtocolls(ImageHandle, SystemTable);
	if (EFI_ERROR(status)) {
		Print(L"An error occured wheile collectiong Boot Protocolls: %d", status);
		return status;
	}
	BootInterface.Version = BOOT_DATA_INTERFACE_VERSION;

	BootInterface.SystemInterface = (EFI_SYSTEM_INTERFACE){
		.Version = EFI_SYSTEM_INTERFACE_VARSION,
		.SystemTable = SystemTable,
		.BootServices = SystemTable->BootServices,
		.RuntimeServices = SystemTable->RuntimeServices,
	};



	bool found_ACPI_2_0 = false;
	bool found_SMBIOS3 = false;
	EFI_GUID ACPI_1_0 = {
		.Data1 = 0xeb9d2d30,
		.Data2 = 0x2d88,
		.Data3 = 0x11d3,
		.Data4 = {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}
	};
	EFI_GUID ACPI_2_0 = {
		.Data1 = 0x8868e871,
		.Data2 = 0xe4f1,
		.Data3 = 0x11d3,
		.Data4 = {0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}
	};
	Print(L"Enumerating Vendor Tables...\n");
	for (unsigned long long index = 0; index < SystemTable->NumberOfTableEntries; index++) {
		EFI_CONFIGURATION_TABLE* configTable = &SystemTable->ConfigurationTable[index];
		Print(TABW L"Config Table %d: %g\n", index, &configTable->VendorGuid);

		//insert giant if else chain to check fro all known guids and print them out for debugging purposes and later assign them to the boot interfacer

		if (CompareGuid(&configTable->VendorGuid, &gEfiSmbios3TableGuid)) {

			Print(TABW TABW L"Found SMBIOS 3 Table: %p\n", configTable->VendorTable);
			SMBIOS3_STRUCTURE_TABLE* smbios3Table = (SMBIOS3_STRUCTURE_TABLE*)configTable->VendorTable;
			Print(TABW TABW L"SMBIOS 3 Table Address: %p\n", smbios3Table);
			Print(TABW TABW L"SMBIOS 3 Table Size: %d\n", smbios3Table->TableMaximumSize);
		}
		else if (CompareGuid(&configTable->VendorGuid, &ACPI_2_0)) {

			Print(TABW TABW L"Found ACPI 2.0 Table: %p\n", configTable->VendorTable);
			Print(TABW TABW L"index: %d\n", index);
			Print(TABW TABW L"GUID of ACPI 2.0 Table: %g\n", &configTable->VendorGuid);
			//here for acpi https://uefi.org/sites/default/files/resources/ACPI_6_3_final_Jan30.pdf
			found_ACPI_2_0 = true;
			ExtendedDescriptionPointer* XSDP = (ExtendedDescriptionPointer*)configTable->VendorTable;
			ParseXACPI(XSDP);



		}
	}
	bool found_ACPI_1_0 = false;
	Print(L"\n\n");
	if (!found_ACPI_2_0) {
		Print(L"ACPI 2.0 not found, searching for ACPI 1.0...\n");
		for (unsigned long long index = 0; index < SystemTable->NumberOfTableEntries; index++) {
			const EFI_CONFIGURATION_TABLE* configTable = &SystemTable->ConfigurationTable[index];

			if (CompareGuid(&configTable->VendorGuid, &ACPI_1_0)) {
				Print(TABW L"Found ACPI 1.0 Table: %p\n", configTable->VendorTable);
				Print(TABW L"index: %d\n", index);
				Print(TABW L"GUID of ACPI 1.0 Table: %g\n", &configTable->VendorGuid);
				found_ACPI_1_0 = true;
			}

		}

	}

	bool found_SMBIOS_2 = false;
	if (!found_SMBIOS3) {
		Print(L"SMBIOS 3 not found, searching for SMBIOS 2...\n");
		for (unsigned long long index = 0; index < SystemTable->NumberOfTableEntries; index++) {
			const EFI_CONFIGURATION_TABLE* configTable = &SystemTable->ConfigurationTable[index];
			if (CompareGuid(&configTable->VendorGuid, &gEfiSmbiosTableGuid)) {
				Print(TABW L"Found SMBIOS 2 Table: %p\n", configTable->VendorTable);
				Print(TABW L"index: %d\n", index);
				Print(TABW L"GUID of SMBIOS 2 Table: %g\n", &configTable->VendorGuid);
				
				found_SMBIOS_2 = true;
			}
		}
	}



	if (!found_SMBIOS3 && !found_SMBIOS_2) {
		Print(L"SystemTable does not contain SMBIOS Tables. this means that the system is eitehr devective,Too old or the bootloader is corupted/bugged");
		return EFI_UNSUPPORTED; //SMBIOS not found. should never occur but if its the case the system is too old
	}
	if (!found_ACPI_1_0 && !found_ACPI_2_0) {
		Print(L"SystemTable does not contain ACPI Tables. this means that the system is eitehr devective,Too old or the bootloader is corupted/bugged");
		return EFI_UNSUPPORTED; //ACPI not found. should never occur but if its the case the system is too old
	}

	Print(L"Parsing CPUID...\n");
	BootInterface.CpuIdInfo = ParseCPUID();
	PrintHeap();
	return EFI_SUCCESS;
}





BOOTLOADER_PROTOCOLS BootloaderProtocols;
BOOT_DATA_INTERFACE BootInterface;

EFI_STATUS CollectProtocolls(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
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
	Print(L"Found GOP\n");
	Print(TABW L"GOP info:\n ");
	Print(TABW L"Framebuffer Base Address: %p\n", BootloaderProtocols.GOP->Mode->FrameBufferBase);
	Print(TABW L"Framebuffer Size: %d\n", BootloaderProtocols.GOP->Mode->FrameBufferSize);
	Print(TABW L"Framebuffer Height: %d\n", BootloaderProtocols.GOP->Mode->Info->VerticalResolution);
	Print(TABW L"Framebuffer Width: %d\n", BootloaderProtocols.GOP->Mode->Info->HorizontalResolution);
	Print(TABW L"Pixels Per Scan Line: %d\n", BootloaderProtocols.GOP->Mode->Info->PixelsPerScanLine);
	Print(TABW L"Pixel Format: %d\n", BootloaderProtocols.GOP->Mode->Info->PixelFormat);


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
	Print(L"Found MPP: %p\n", BootloaderProtocols.MPP);

	BootInterface.GraphicsOutputProtocolInfo = (GOP_INFO){
	.Version = GOP_INFO_VERSION,
	.FramebufferBaseAddress = (unsigned int*)BootloaderProtocols.GOP->Mode->FrameBufferBase,
	.FramebufferSize = BootloaderProtocols.GOP->Mode->FrameBufferSize,
	.FrabufferHeight = BootloaderProtocols.GOP->Mode->Info->VerticalResolution,
	.FrabufferWidth = BootloaderProtocols.GOP->Mode->Info->HorizontalResolution,
	.PixelsPerScanLine = BootloaderProtocols.GOP->Mode->Info->PixelsPerScanLine,
	.PixelFormat = BootloaderProtocols.GOP->Mode->Info->PixelFormat,
	};
	UINTN numProcessors = 0;
	UINTN numEnabledProcessors = 0;
	BootloaderProtocols.MPP->GetNumberOfProcessors(&BootloaderProtocols.MPP, &numProcessors, &numEnabledProcessors);
	Print(L"MPP info: \n");
	Print(TABW L"Number of Processors: %d\n", numProcessors);
	Print(TABW L"Number of Enabled Processors: %d\n", numEnabledProcessors);

	return EFI_SUCCESS;
}
