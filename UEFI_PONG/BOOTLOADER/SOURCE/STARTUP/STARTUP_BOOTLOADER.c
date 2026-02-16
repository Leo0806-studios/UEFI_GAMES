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
#include "STARTUP/STARTUP.h"
#include "GLOBALS.h"
#include <intrin.h>
static void MEMSET(char* dest, char value, size_t count) {
	for (size_t i = 0; i < count; i++) {
		dest[i] = value;
	}
}
static void MEMCPY(char* dest, const char* src, size_t count) {
	for (size_t i = 0; i < count; i++) {
		dest[i] = src[i];
	}
}
CPUID ParseCPUID(void);
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
	UINTN numProcessors = 0;
	UINTN numEnabledProcessors = 0;
	BootloaderProtocols.MPP.GetNumberOfProcessors(&BootloaderProtocols.MPP, &numProcessors, &numEnabledProcessors);



	bool found_ACPI_2_0 = false;
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
		.Data4 = 0x11d3,
		.Data4 = {0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}
	};
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
		else if (CompareGuid(&configTable->VendorGuid, &ACPI_2_0)) {

			//here for acpi https://uefi.org/sites/default/files/resources/ACPI_6_3_final_Jan30.pdf
			found_ACPI_2_0 = true;

		}
	}
	bool found_ACPI_1_0 = false;
	if (!found_ACPI_2_0) {
		for (unsigned long long index = 0; index < SystemTable->NumberOfTableEntries; index++) {
			EFI_CONFIGURATION_TABLE* configTable = &SystemTable->ConfigurationTable[index];

			if (CompareGuid(&configTable->VendorGuid, &ACPI_1_0)) {

			}

		}

	}
	if (!found_ACPI_1_0 && !found_ACPI_2_0) {
		Print(L"SystemTable does not contain ACPI Tables. this means that the system is eitehr devective,Too old or the bootloader is corupted/bugged");
		return 0xff; //ACPI not found. should never occur but if its the case the system is too old
	}

	return EFI_SUCCESS;
}

CPUID ParseCPUID(void) {
	int buff[25][4] = {0};//EAX, EBX, ECX, and EDX registers (in that order) (from https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=msvc-170#remarks)
	MEMSET(buff, 0, sizeof(buff));
	__cpuid(buff[0], 0);
	CPUIDLeafs allLeafs={ 0 };
	const int maxCPUIDLeaves = buff[0][0];
	for (int leaf = 1; leaf < maxCPUIDLeaves; leaf++) {
		__cpuid(buff[leaf], leaf);
	}
	MEMCPY(&allLeafs, buff,sizeof(CPUIDLeafs));
	return (CPUID) { .Version = CPUID_VERSION, .Size = sizeof(CPUID), .Leafs = allLeafs };
}


 BOOTLOADER_PROTOCOLS BootloaderProtocols;
 BOOT_DATA_INTERFACE BootInterface;