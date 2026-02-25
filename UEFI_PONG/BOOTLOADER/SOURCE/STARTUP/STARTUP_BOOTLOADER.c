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
#define TABW4 "    "
#define TABW8 "        "
#define TABW12 "            "
#define TABW16 "                "
#define TABW20 "                    "
#define TABW24 "                        "
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
void PrintFADT(FixedACPIDescriptionTable* fadt, const char* intendent);
ACPI_TABLES ParseXACPI(ExtendedDescriptionPointer* ptr);
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

/// <summary>
/// allocates the memory for num+1 subleafs and zeros the memory
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
static int* AllocateSubLeafsZ(unsigned int num) {
	if (num > UINT32_MAX / 2) {
		return NULL; // Prevent overflow
	}
	void* ptr = Alloc((sizeof(uint32_t) * 4) * (num + 1));
	if (!ptr) {
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
			int* AllocatedSubLeafs = AllocateSubLeafsZ(SubLeafCount);
			if (!AllocatedSubLeafs) {
				Print(L"Failed to allocate memory for CPUID Leaf 0x04 subleafs\n");
				return EFI_ABORTED;
			}


			for (unsigned int i = 0; i < SubLeafCount; i++) {

				__cpuidex(&localBuff[0], leaf, i);
				Print(L"SubLeaf: %d, EAX %d, EBX %d, ECX %d, EDX %d\n", i, localBuff[0], localBuff[1], localBuff[2], localBuff[3]);
				MEMCPY(&AllocatedSubLeafs[i * 4], localBuff, sizeof(localBuff));

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
				Print(L"SubLeaf: %d, EAX %d, EBX %d, ECX %d, EDX %d\n", subLeaf, buff[*index][0], buff[*index][1], buff[*index][2], buff[*index][3]);

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
			int* AllocatedSubLeafs = AllocateSubLeafsZ(SubLeafCount);
			if (!AllocatedSubLeafs) {
				Print(L"Failed to allocate memory for CPUID Leaf 0x04 subleafs\n");
				return 0;
			}
			for (unsigned int i = 0; i < SubLeafCount; i++) {

				__cpuidex(&localBuff[0], leaf, i);
				Print(L"SubLeaf: %d, EAX %d, EBX %d, ECX %d, EDX %d\n", i, localBuff[0], localBuff[1], localBuff[2], localBuff[3]);
				MEMCPY(&AllocatedSubLeafs[i * 4], localBuff, sizeof(localBuff));

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

			__cpuidex(localbuff, leaf, 0);

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
	__cpuid(&localbuff[0], 0);
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
		QueryInnerCPUIDLoop(leaf, &index);
	}
	MEMCPY(&allLeafs, buff, sizeof(CPUIDLeafs));
	return (CPUID) {
		.Version = CPUID_VERSION, .Size = sizeof(CPUID), .Leafs = allLeafs
	};
}

void PrintFADT(FixedACPIDescriptionTable* fadt, const char* intendent)
{
	//header allready printed by the caller

	Print(L"%aFACP FirmwareCtrl: %x\n", intendent, fadt->FirmwareCtrl);
	Print(L"%aFACP Dsdt: %x\n", intendent, fadt->Dsdt);
	Print(L"%aFACP PreferredPmProfile: %d\n", intendent, fadt->PreferredPmProfile);
	Print(L"%aFACP SciInt: %d\n", intendent, fadt->SciInt);
	Print(L"%aFACP SmiCmd: %x\n", intendent, fadt->SmiCmd);
	Print(L"%aFACP AcpiEnable: %x\n", intendent, fadt->AcpiEnable);
	Print(L"%aFACP AcpiDisable: %x\n", intendent, fadt->AcpiDisable);
	Print(L"%aFACP S4BiosReq: %x\n", intendent, fadt->S4BiosReq);
	Print(L"%aFACP PstateCnt: %x\n", intendent, fadt->PstateCnt);
	Print(L"%aFACP Pm1aEvtBlk: %x\n", intendent, fadt->Pm1aEvtBlk);
	Print(L"%aFACP Pm1bEvtBlk: %x\n", intendent, fadt->Pm1bEvtBlk);
	Print(L"%aFACP Pm1aCntBlk: %x\n", intendent, fadt->Pm1aCntBlk);
	Print(L"%aFACP Pm1bCntBlk: %x\n", intendent, fadt->Pm1bCntBlk);
	Print(L"%aFACP Pm2CntBlk: %x\n", intendent, fadt->Pm2CntBlk);
	Print(L"%aFACP PmTmrBlk: %x\n", intendent, fadt->PmTmrBlk);
	Print(L"%aFACP Gpe0Blk: %x\n", intendent, fadt->Gpe0Blk);
	Print(L"%aFACP Gpe1Blk: %x\n", intendent, fadt->Gpe1Blk);
	Print(L"%aFACP Pm1EvtLen: %d\n", intendent, fadt->Pm1EvtLen);
	Print(L"%aFACP Pm1CntLen: %d\n", intendent, fadt->Pm1CntLen);
	Print(L"%aFACP Pm2CntLen: %d\n", intendent, fadt->Pm2CntLen);
	Print(L"%aFACP PmTmrLen: %d\n", intendent, fadt->PmTmrLen);
	Print(L"%aFACP Gpe0BlkLen: %d\n", intendent, fadt->Gpe0BlkLen);
	Print(L"%aFACP Gpe1BlkLen: %d\n", intendent, fadt->Gpe1BlkLen);
	Print(L"%aFACP Gpe1Base: %d\n", intendent, fadt->Gpe1Base);
	Print(L"%aFACP CstCnt: %x\n", intendent, fadt->CstCnt);
	Print(L"%aFACP PLvl2Lat: %d\n", intendent, fadt->PLvl2Lat);
	Print(L"%aFACP PLvl3Lat: %d\n", intendent, fadt->PLvl3Lat);
	Print(L"%aFACP FlushSize: %d\n", intendent, fadt->FlushSize);
	Print(L"%aFACP FlushStride: %d\n", intendent, fadt->FlushStride);
	Print(L"%aFACP DutyOffset: %d\n", intendent, fadt->DutyOffset);
	Print(L"%aFACP DutyWidth: %d\n", intendent, fadt->DutyWidth);
	Print(L"%aFACP DayAlrm: %d\n", intendent, fadt->DayAlrm);
	Print(L"%aFACP MonAlrm: %d\n", intendent, fadt->MonAlrm);
	Print(L"%aFACP Century: %d\n", intendent, fadt->Century);
	Print(L"%aFACP IapcBootArch: %x\n", intendent, fadt->IapcBootArch);
	Print(L"%aFACP Flags: %x\n", intendent, fadt->Flags);
	Print(L"%aFACP ResetReg: %p\n", intendent, &fadt->ResetReg);
	Print(L"%aFACP ResetValue: %x\n", intendent, fadt->ResetValue);
	Print(L"%aFACP ArmBootArch: %x\n", intendent, fadt->ArmBootArch);
	Print(L"%aFACP FadtMinorVersion: %d\n", intendent, fadt->FadtMinorVersion);
	Print(L"%aFACP XFirmwareCtrl: %llx\n", intendent, fadt->XFirmwareCtrl);
	Print(L"%aFACP XDsdt: %llx\n", intendent, fadt->XDsdt);
	Print(L"%aFACP XPm1aEvtBlk: %p\n", intendent, &fadt->XPm1aEvtBlk);
	Print(L"%aFACP XPm1bEvtBlk: %p\n", intendent, &fadt->XPm1bEvtBlk);
	Print(L"%aFACP XPm1aCntBlk: %p\n", intendent, &fadt->XPm1aCntBlk);
	Print(L"%aFACP XPm1bCntBlk: %p\n", intendent, &fadt->XPm1bCntBlk);
	Print(L"%aFACP XPm2CntBlk: %p\n", intendent, &fadt->XPm2CntBlk);
	Print(L"%aFACP XPmTmrBlk: %p\n", intendent, &fadt->XPmTmrBlk);
	Print(L"%aFACP XGpe0Blk: %p\n", intendent, &fadt->XGpe0Blk);
	Print(L"%aFACP XGpe1Blk: %p\n", intendent, &fadt->XGpe1Blk);
	Print(L"%aFACP SleepControlReg: %p\n", intendent, &fadt->SleepControlReg);
	Print(L"%aFACP SleepStatusReg: %p\n", intendent, &fadt->SleepStatusReg);
	Print(L"%aFACP HypervisorVendorIdentity: %llx\n", intendent, fadt->HypervisorVendorIdentity);
}

void PrintSystemTableHeader(const SystemDescriptionTableHeader* header, const char* tableName, const char* intendetn) {
	Print(L"%a%a Signature: %d\n", intendetn, tableName, header->Signature);
	Print(L"%a%a Length: %d\n", intendetn, tableName, header->Length);
	Print(L"%a%a Revision: %d\n", intendetn, tableName, header->Revision);
	Print(L"%a%a Checksum: %d\n", intendetn, tableName, header->Checksum);
	Print(L"%a%a OemId: %a\n", intendetn, tableName, header->OemId);
	Print(L"%a%a OemTableId: %d\n", intendetn, tableName, header->OemTableId);
	Print(L"%a%a OemRevision: %d\n", intendetn, tableName, header->OemRevision);
	Print(L"%a%a CreatorId: %d\n", intendetn, tableName, header->CreatorId);
	Print(L"%a%a CreatorRevision: %d\n", intendetn, tableName, header->CreatorRevision);
}
MADT_TABLES ParseMadt(const MultipleAPICDescriptionTable* tableHeader);
ACPI_TABLES ParseXACPI(ExtendedDescriptionPointer* XSDP)
{
	ACPI_TABLES tables = { 0 };
	if (XSDP->Signature != RDSP) {
		Print(L"XSDP Signature is incorrect\n");
		return (ACPI_TABLES) {
			0
		}; //XSDP signature is incorrect, this means that the table is either not an XSDP or its corrupted
	}
	Print(TABW TABW TABW L"XSDP Signature is correct\n");
	Print(TABW TABW TABW L"XSDP Checksum: %d\n", XSDP->ExtendedChecksum);
	Print(TABW TABW TABW L"XSDP OEM ID: %a\n", XSDP->OemId);
	Print(TABW TABW TABW L"XSDP Revision: %d\n", XSDP->Revision);
	Print(TABW TABW TABW L"XSDP RSDT Address: %p\n", XSDP->RsdtAddress);
	Print(TABW TABW TABW L"XSDP Length: %d\n", XSDP->Length);
	Print(TABW TABW TABW L"XSDP XSDT Address: %p. Following...\n", XSDP->XsdtAddress);
	BootInterface.ACPI_Tables.XSDP = XSDP;
	ExtendedSystemDescriptionTable* xSDT = (ExtendedSystemDescriptionTable*)XSDP->XsdtAddress;
	if (xSDT->Signature != XSDT) {
		Print(L"XSDT Signature is incorrect\n");
		return (ACPI_TABLES) {
			0
		}; //XSDT signature is incorrect, this means that the table is either not an XSDT or its corrupted
	}
	Print(TABW TABW TABW TABW L"XSDT Signature is correct\n");

	PrintSystemTableHeader((SystemDescriptionTableHeader*)xSDT, "XSDT", TABW16);
	//Print(TABW TABW TABW TABW L"XSDT Checksum: %d\n", xSDT->Checksum);
	//Print(TABW TABW TABW TABW L"XSDT OEM ID: %a\n", xSDT->OemId);
	//Print(TABW TABW TABW TABW L"XSDT Revision: %d\n", xSDT->Revision);
	//Print(TABW TABW TABW TABW L"XSDT Oem Table ID: %d\n", xSDT->OemTableId);
	//Print(TABW TABW TABW TABW L"XSDT Oem Revision: %d\n", xSDT->OemRevision);
	//Print(TABW TABW TABW TABW L"XSDT Creator ID: %d\n", xSDT->CreatorId);
	//Print(TABW TABW TABW TABW L"XSDT Creator Revision: %d\n", xSDT->CreatorRevision);
	uint64_t numEntries = (xSDT->Length - sizeof(SystemDescriptionTableHeader)) / sizeof(uint64_t);
	Print(TABW TABW TABW TABW L"XSDT Number of Entries: %d\n", numEntries);
	Print(TABW TABW TABW TABW L"Enumerating XSDT Entries now ...\n");
	for (uint64_t i = 0; i < numEntries; i++) {
		uint64_t entryAddr = xSDT->Entry[i];
		const SystemDescriptionTableHeader* entryHeader = (SystemDescriptionTableHeader*)entryAddr;
		Print(TABW TABW TABW TABW L"Entry %d: Address: %p\n", i, entryAddr);
		char entrySig[5] = { 0 };
		((uint32_t*)entrySig)[0] = entryHeader->Signature;
		PrintSystemTableHeader(entryHeader, entrySig, TABW20);
		switch (entryHeader->Signature) {
			case APIC: {
				MultipleAPICDescriptionTable* MADT = (MultipleAPICDescriptionTable*)entryAddr;

				tables.MadtTables = ParseMadt(MADT);
				break;
			}
			case FACP: {
				FixedACPIDescriptionTable* fadt = (FixedACPIDescriptionTable*)entryAddr;
				Print(TABW TABW TABW TABW L"Parsing FADT Table...\n");
				PrintFADT(fadt, TABW24);
				break;
			}
		}
	}
}

MADT_TABLES ParseMadt(const MultipleAPICDescriptionTable* MADT)
{
	MADT_TABLES tables = { 0 };
	tables.MADT = MADT;
	Print(L"%aParsing MADT Table...\n", TABW20);
	Print(L"%aMADT Local APIC Address: %p\n", TABW20, MADT->LocalApicAddress);
	Print(L"%aMADT Flags: %d\n", TABW20, MADT->Flags);
	Print(L"%aMADT Interupt Controller Structures: \n", TABW20);
	APICHeader* current = (APICHeader*)MADT->InteruptControllerStructures;
	uint64_t offset = 0;
	uint64_t count = 0;
	while (offset < MADT->Header.Length) {
		offset += current->Length;
		current = (APICHeader*)((uint64_t)current + current->Length);
		count++;
	}
	Print(L"%aTotal APIC Structures: %d\n", TABW20, count);


	tables.Entrys = Alloc(sizeof(void*) * count);
	if (!tables.Entrys) {
		Print(L"Failed to allocate memory for MADT Entrys\n");
		return (MADT_TABLES) {
			0
		};
	}



	offset = 0;
	current = (APICHeader*)MADT->InteruptControllerStructures;
	count = 0;
	while (offset < MADT->Header.Length) {
		Print(L"%aAPIC Structure at offset %d: Type: %d, Length: %d\n", TABW20, offset, current->Type, current->Length);
		tables.Entrys[count] = current;
		tables.NumberOfEntries++;


		switch (current->Type) {
			case 0: {
				ProcessorLocalAPICStructure* localAPIC = (ProcessorLocalAPICStructure*)current;
				Print(L"%aProcessor Local APIC Structure:\n", TABW24);
				break;
			}
			case 1: {
				IOAPICStructure* ioAPIC = (IOAPICStructure*)current;
				Print(L"%aIO APIC Structure:\n", TABW24);
				Print(L"%aIO APIC ID: %d\n", TABW24, ioAPIC->IOAPICId);
				Print(L"%aIO APIC Address: %p\n", TABW24, ioAPIC->IOAPICAddress);
				Print(L"%aIO APIC Global System Interrupt Base: %d\n", TABW24, ioAPIC->GlobalSystemInterruptBase);
				break;
			}
			case 2: {
				InterruptSourceOverrideStructure* iso = (InterruptSourceOverrideStructure*)current;
				Print(L"%aInterrupt Source Override Structure:\n", TABW24);
				Print(L"%aISO Bus Source: %d\n", TABW24, iso->Bus);
				Print(L"%aISO IRQ Source: %d\n", TABW24, iso->Source);
				Print(L"%aISO Global System Interrupt: %d\n", TABW24, iso->GlobalSystemInterrupt);
				Print(L"%aISO Flags: %d\n", TABW24, iso->Flags);
				break;
			}

		}



		offset += current->Length;
		current = (APICHeader*)((uint64_t)current + current->Length);
		count++;
	}



}

BOOTLOADER_PROTOCOLS BootloaderProtocols;
BOOT_DATA_INTERFACE BootInterface;