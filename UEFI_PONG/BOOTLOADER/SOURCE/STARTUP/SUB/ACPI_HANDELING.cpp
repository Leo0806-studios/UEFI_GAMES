#include <intrin.h>
#include "ACPI_STRUCTS.h"
#include "BOOT_DATA_INTERFACE.h"
#include "SUB_FORWARD_DECLARATIONS.h"

MADT_TABLES ParseMadt(const MultipleAPICDescriptionTable* tableHeader);



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
	tables.XSDP = XSDP;
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
		case 0x03: {
			NonMaskableInterruptSourceStructure* nmis = (NonMaskableInterruptSourceStructure*)current;
			Print(L"%aNon-Maskable Interrupt (NMI) Source Structure:\n", TABW24);
			Print(L"%aNMIS Flags: %d\n", TABW24, nmis->Flags);
			Print(L"%sNMIS Global System Interrupt: %d\n", TABW24, nmis->GlobalSystemInterrupt);
			break;
		}
		case 0x04: {
			LocalAPICNMIStructure* lan = (LocalAPICNMIStructure*)current;
			Print(L"%aLocal APIC NMI Structure:\n", TABW24);
			Print(L"%aLAN ACPI Processor UID: %d\n", TABW24, lan->AcpiProcessorUId);
			Print(L"%aLAN Flags: %d\n", TABW24, lan->Flags);
			Print(L"%aLAN Local APIC LINT#: %d\n", TABW24, lan->LocalApicLint);
			break;
		}
		}



		offset += current->Length;
		current = (APICHeader*)((uint64_t)current + current->Length);
		count++;
	}



}