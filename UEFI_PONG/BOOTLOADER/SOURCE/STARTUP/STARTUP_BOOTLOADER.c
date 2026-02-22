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
#include "HEAP/HEAP.h"

#define MiB(x) ((UINT64)(x) << 20)
#define GiB(x) ((UINT64)(x) << 30)
#define TABW L"    "
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
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	Print(L"Bootloader Startup...\n");
	CreatHeap(MiB(20));
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
	BootInterface.Version = BOOT_DATA_INTERFACE_VERSION;

	BootInterface.SystemInterface = (EFI_SYSTEM_INTERFACE){
		.Version = EFI_SYSTEM_INTERFACE_VARSION,
		.SystemTable = SystemTable,
		.BootServices = SystemTable->BootServices,
		.RuntimeServices = SystemTable->RuntimeServices,
	};
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
	return EFI_SUCCESS;
}

/// <summary>
/// allocates the memory for num+1 subleafs and zeros the memory
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
static int** AllocateSubLeafsZ(unsigned int num) {
	if (num > UINT32_MAX / 2) {
		return NULL; // Prevent overflow
	}
	void* ptr = Alloc((sizeof(uint32_t) * 4) * (num + 1));
	if(!ptr) {
		return NULL; // Allocation failed
	}
	MEMSET((char*)ptr, 0, (sizeof(uint32_t) * 4) * (num + 1));
	return ptr;
}

static	int buff[(sizeof(CPUIDLeafs) / sizeof(int)) / 4][4] = { 0 };//EAX, EBX, ECX, and EDX registers (in that order) (from https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=msvc-170#remarks)
static EFI_STATUS QueryInnerCPUIDLoop(const int leaf, uint32_t* index) {
	
	switch (leaf) {
		case 0x04: {
			int localBuff[4] = { 0,0,0,0 };
			int SubLeafCount = 0;
			for (unsigned int i = 0;; i++) {
				__cpuidex(localBuff, leaf, i);
				SubLeafCount++;
				if (!((CPUIDLeaf0x04SubLeaf*)localBuff)->CACHE_LEVEL) {
					break;
				}
			}
			int** AllocatedSubLeafs = AllocateSubLeafsZ(SubLeafCount);
			if (!AllocatedSubLeafs) {
				Print(L"Failed to allocate memory for CPUID Leaf 0x04 subleafs\n");
				return EFI_ABORTED;
			}
			for (unsigned int i = 0;i<SubLeafCount; i++) {
				__cpuidex(AllocatedSubLeafs[i], leaf, i);
				Print(L"SubLeaf: %d, EAX %d, EBX %d, ECX %d, EDX %d\n", i, AllocatedSubLeafs[i][0], AllocatedSubLeafs[i][1], AllocatedSubLeafs[i][2], AllocatedSubLeafs[i][3]);
			}

			((CPUIDLeaf0x04*)buff[*index])->subleafs = AllocatedSubLeafs;
			break;
		}
		case 0x07: {
			int localbuff[4] = { 0,0,0,0 };
			__cpuidex(localbuff, leaf, 0);
			uint32_t maxLeaf = ((CPUIDLeaf0x07SubLeaf0*)localbuff)->MAX_SUBLEAF;
			//const int a = 
			if (maxLeaf >= sizeof(CPUIDLeaf0x07) / sizeof(uint32_t) / 4) {
				Print(L"more subleafs than currently supported\n");
				return EFI_ABORTED;
			}
			for (uint32_t subLeaf = 0; subLeaf <= maxLeaf; subLeaf++) {
				
				__cpuidex(buff[*index], leaf, subLeaf);
				(*index)++;
			}

			break;
		}
		case 0x0B: {
			int localBuff[4] = { 0,0,0,0 };
			int SubLeafCount = 0;
			for (unsigned int i = 0;; i++) {
				__cpuidex(localBuff, leaf, i);
				SubLeafCount++;
				if (!((CPUIDLeaf0x0BSubLeaf*)localBuff)->LOCAL_NEXT_LEVEL_NUM_LP) {
					break;
				}
			}
			int** AllocatedSubLeafs = AllocateSubLeafsZ(SubLeafCount);
			if (!AllocatedSubLeafs) {
				Print(L"Failed to allocate memory for CPUID Leaf 0x04 subleafs\n");
				return EFI_ABORTED;
			}
			for (unsigned int i = 0; i < SubLeafCount; i++) {
				__cpuidex(AllocatedSubLeafs[i], leaf, i);

			}

			((CPUIDLeaf0x0B*)buff[*index])->subleafs = AllocatedSubLeafs;
			break;
		}
		case 0x0D: {
			break;
		}
		case 0x0F: {
			break;
		}
		case 0x10: {
			break;
		}
		case 0x12: {
			break;
		}
		case 0x14: {
			break;
		}
		case 0x17: {
			break;
		}
		case 0x18: {
			break;
		}
		case 0x1d: {
			break;
		}
		case 0x1F: {
			break;
		}
		case 0x20: {
			break;
		}
		case 0x23: {
			break;
		}
		case 0x24: {
			break;
		}
		default: {
			int localbuff[4] = { 0,2,3,4 };

			__cpuidex(localbuff, leaf,0);
			
			Print(L"Leaf %d, EAX %d, EBX %d, ECX %d, EDX %d \n", leaf, localbuff[0], localbuff[1], localbuff[2], localbuff[3]);
			MEMCPY(buff[*index], localbuff, sizeof(localbuff));
			//Print(L"Leaf %d, EAX %d, EBX %d, ECX %d, EDX %d \n", leaf, buff[*index][0], buff[*index][1], buff[*index][2], buff[*index][3]);

			(*index)++;
			break;
		}


	}
	return EFI_SUCCESS;

}


CPUID ParseCPUID(void) {
	static_assert(sizeof(int) == 4, "This code assumes that sizeof(int) is 4 bytes");
	static_assert(sizeof(buff) == sizeof(buff), " buff and CPUID must be the same size");
	static_assert(sizeof(CPUIDLeafs) % 4 == 0, "CPUIDLeafs must be a multiple of 4 bytes in size");
	MEMSET(buff, 0, sizeof(buff));
	int localbuff[4] = { 0,0,0,0 };
	__cpuid(buff[0], 0);
	CPUIDLeafs allLeafs = { 0 };
	Print(L"Leaf %d, EAX %d, EBX %d, ECX %d, EDX %d \n", 0, buff[0][0], buff[0][1], buff[0][2], buff[0][3]);

	const int maxCPUIDLeaves = buff[0][0];

	char venStr[4 * 3 + 1] = { 0 };
	((int*)venStr)[0] = localbuff[1];
	((int*)venStr)[1] = localbuff[3];
	((int*)venStr)[2] = localbuff[2];
	venStr[12] = 0x0;
	Print(L"Vendor ID str: %s", venStr);
	Print(L"Max leafs supported: %d", maxCPUIDLeaves);
	uint32_t index = 1;
	for (int leaf = 1; leaf < maxCPUIDLeaves; leaf++) {
		Print(L"Querying CPUID Leaf %d\n", leaf);
		QueryInnerCPUIDLoop(leaf,&index);
	}
	MEMCPY(&allLeafs, buff, sizeof(CPUIDLeafs));
	return (CPUID) {
		.Version = CPUID_VERSION, .Size = sizeof(CPUID), .Leafs = allLeafs
	};
}


		BOOTLOADER_PROTOCOLS BootloaderProtocols ;
		BOOT_DATA_INTERFACE BootInterface ;