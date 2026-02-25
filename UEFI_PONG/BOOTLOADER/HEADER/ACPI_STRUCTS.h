#pragma once
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include <stdint.h>
#ifdef __cplusplus
}
#endif // __cplusplus
#define BitfieldWidth(top,bottom) ((top) - (bottom)) +1

#define CombineCharsInt(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))
#define CombineCharsLongLong(a, b, c, d,e,f,g,h) ((uint64_t)(a) | ((uint64_t)(b) << 8) | ((uint64_t)(c) << 16) | ((uint64_t)(d) << 24) | ((uint64_t)(e) << 32) | ((uint64_t)(f) << 40) | ((uint64_t)(g) << 48) | ((uint64_t)(h) << 56))
typedef enum sigValsACPI {
	APIC = CombineCharsInt('A', 'P', 'I', 'C'),
	BERT = CombineCharsInt('B', 'E', 'R', 'T'),
	BGRT = CombineCharsInt('B', 'G', 'R', 'T'),
	CPEP = CombineCharsInt('C', 'P', 'E', 'P'),
	DSDT = CombineCharsInt('D', 'S', 'D', 'T'),
	ECDT = CombineCharsInt('E', 'C', 'D', 'T'),
	EINJ = CombineCharsInt('E', 'I', 'N', 'J'),
	ERST = CombineCharsInt('E', 'R', 'S', 'T'),
	FACP = CombineCharsInt('F', 'A', 'C', 'P'),
	FACS = CombineCharsInt('F', 'A', 'C', 'S'),
	PFDT = CombineCharsInt('P', 'F', 'D', 'T'),
	GTDT = CombineCharsInt('G', 'T', 'D', 'T'),
	HEST = CombineCharsInt('H', 'E', 'S', 'T'),
	MSCT = CombineCharsInt('M', 'S', 'C', 'T'),
	MPST = CombineCharsInt('M', 'P', 'S', 'T'),
	NFIT = CombineCharsInt('N', 'F', 'I', 'T'),
	//insert all OEMx sigs here
	PCCT = CombineCharsInt('P', 'C', 'C', 'T'),
	PHAT = CombineCharsInt('P', 'H', 'A', 'T'),
	PMIT = CombineCharsInt('P', 'M', 'I', 'T'),
	PSDT = CombineCharsInt('P', 'S', 'D', 'T'),
	RASF = CombineCharsInt('R', 'A', 'S', 'F'),
	RSDT = CombineCharsInt('R', 'S', 'D', 'T'),
	SBST = CombineCharsInt('S', 'B', 'S', 'T'),
	SDEV = CombineCharsInt('S', 'D', 'E', 'V'),
	SLIT = CombineCharsInt('S', 'L', 'I', 'T'),
	SRAT = CombineCharsInt('S', 'R', 'A', 'T'),
	SSDT = CombineCharsInt('S', 'S', 'D', 'T'),
	XSDT = CombineCharsInt('X', 'S', 'D', 'T'),
}ACPISignatureValues;

static uint64_t RDSP = CombineCharsLongLong('R', 'S', 'D', ' ', 'P', 'T', 'R', ' ');

#pragma pack(push, 1)
typedef struct {
	uint64_t Signature;
	uint8_t Checksum;
	uint8_t OemId[6];
	uint8_t Revision;
	uint32_t RsdtAddress;

}RootSystemDescriptionPointer;
typedef struct {
	uint64_t Signature;
	uint8_t Checksum;
	uint8_t OemId[6];
	uint8_t Revision;
	uint32_t RsdtAddress;
	uint32_t Length;
	uint64_t XsdtAddress;
	uint8_t ExtendedChecksum;
	uint8_t Reserved[3];
}ExtendedDescriptionPointer;

typedef struct{
	uint32_t Signature;
	uint32_t Length;
	uint8_t Revision;
	uint8_t Checksum;
	uint8_t OemId[6];
	uint64_t OemTableId;
	uint32_t OemRevision;
	uint32_t CreatorId;
	uint32_t CreatorRevision;
} SystemDescriptionTableHeader;
typedef struct {
	uint32_t Signature;
	uint32_t Length;
	uint8_t Revision;
	uint8_t Checksum;
	uint8_t OemId[6];
	uint64_t OemTableId;
	uint32_t OemRevision;
	uint32_t CreatorId;
	uint32_t CreatorRevision;
	uint32_t Entry[];
}RootSystemDescriptionTable;
typedef struct {
	uint32_t Signature;
	uint32_t Length;
	uint8_t Revision;
	uint8_t Checksum;
	uint8_t OemId[6];
	uint64_t OemTableId;
	uint32_t OemRevision;
	uint32_t CreatorId;
	uint32_t CreatorRevision;
	uint64_t Entry[];
}ExtendedSystemDescriptionTable;

typedef struct {
	SystemDescriptionTableHeader Header;
	uint32_t FirmwareCtrl;
	uint32_t Dsdt;
uint8_t:BitfieldWidth(7, 0);
	uint8_t PreferredPmProfile;
	uint16_t SciInt;
	uint32_t SmiCmd;
	uint8_t AcpiEnable;
	uint8_t AcpiDisable;
	uint8_t S4BiosReq;
	uint8_t PstateCnt;
	uint32_t Pm1aEvtBlk;
	uint32_t Pm1bEvtBlk;
	uint32_t Pm1aCntBlk;
	uint32_t Pm1bCntBlk;
	uint32_t Pm2CntBlk;
	uint32_t PmTmrBlk;
	uint32_t Gpe0Blk;
	uint32_t Gpe1Blk;
	uint8_t Pm1EvtLen;
	uint8_t Pm1CntLen;
	uint8_t Pm2CntLen;
	uint8_t PmTmrLen;
	uint8_t Gpe0BlkLen;
	uint8_t Gpe1BlkLen;
	uint8_t Gpe1Base;
	uint8_t CstCnt;
	uint16_t PLvl2Lat;
	uint16_t PLvl3Lat;
	uint16_t FlushSize;
	uint16_t FlushStride;
	uint8_t DutyOffset;
	uint8_t DutyWidth;
	uint8_t DayAlrm;
	uint8_t MonAlrm;
	uint8_t Century;
	uint16_t IapcBootArch;
uint8_t:BitfieldWidth(7, 0);
	uint32_t Flags;
	uint8_t ResetReg[12];
	uint8_t ResetValue;
	uint16_t ArmBootArch;
	uint8_t FadtMinorVersion;
	uint64_t XFirmwareCtrl;
	uint64_t XDsdt;
	uint8_t XPm1aEvtBlk[12];
	uint8_t XPm1bEvtBlk[12];
	uint8_t XPm1aCntBlk[12];
	uint8_t XPm1bCntBlk[12];
	uint8_t XPm2CntBlk[12];
	uint8_t XPmTmrBlk[12];
	uint8_t XGpe0Blk[12];
	uint8_t XGpe1Blk[12];
	uint8_t SleepControlReg[12];
	uint8_t SleepStatusReg[12];
	uint64_t HypervisorVendorIdentity;

}FixedACPIDescriptionTable;

typedef struct {
	uint32_t Signature;
	uint32_t Length;
	uint32_t HardwareSignature;
	uint32_t FirmwareWakingVector;
	uint32_t GlobalLock;
	uint32_t Flags;
	uint64_t xFirmwareWakingVector;
	uint8_t Version;
uint16_t:BitfieldWidth(15, 0);
uint8_t:BitfieldWidth(7, 0);
	uint32_t OspmFlags;
uint64_t :BitfieldWidth(63, 0);
uint64_t:BitfieldWidth(63, 0);
uint64_t :BitfieldWidth(63, 0);


}FirmwareACPIControlStructure;


typedef struct {
	SystemDescriptionTableHeader Header;
	uint8_t data[];
}DifferentiatedSystemDescriptionTable;

typedef struct {
	SystemDescriptionTableHeader Header;
	uint8_t data[];
}SecondarySystemDescriptionTable;
typedef struct 
{
	SystemDescriptionTableHeader Header;
}PersistentSystemDescriptionTable;

typedef struct {
	SystemDescriptionTableHeader Header;
	uint32_t Reserved;
	uint64_t LocalApicAddress;
	uint32_t Flags;
	uint8_t InteruptControllerStructures[];
}MultipleAPICDescriptionTable;
typedef struct {
	uint8_t Type;
	uint8_t Length;
}APICHeader;
typedef struct {
	APICHeader Header;
	uint8_t AcpiProcessorUId;
	uint8_t ApicId;
	uint32_t Flags;
}ProcessorLocalAPICStructure;
typedef struct {
	APICHeader Header;
uint8_t IOAPICId;
uint8_t:BitfieldWidth(7, 0);
	uint32_t IOAPICAddress;
	uint32_t GlobalSystemInterruptBase;
}IOAPICStructure;

typedef struct {
	APICHeader Header;
	uint8_t Bus;
	uint8_t Source;
	uint32_t GlobalSystemInterrupt;
	uint16_t Flags;
}InterruptSourceOverrideStructure;
typedef struct {
	APICHeader Header;
	uint16_t Flags;
	uint32_t GlobalSystemInterrupt;
}NonMaskableInterruptSourceStructure;
typedef struct {
	APICHeader Header;
	uint8_t AcpiProcessorUId;
	uint16_t Flags;
	uint8_t LocalApicLint;
}LocalAPICNMIStructure;
typedef struct {
	APICHeader Header;
uint16_t:BitfieldWidth(15, 0);
	uint64_t LocalApicAddress;
}LocalAPICAddressOverrideStructure;



typedef struct {
APICHeader Header;
uint8_t IoApicId;
uint8_t:BitfieldWidth(7, 0);
uint32_t GlobalSystemInterruptBase;
uint64_t IoSapicAddress;
}IOSAPICStructure;
typedef struct {
	APICHeader Header;
	uint8_t AcpiProcessorId;
	uint8_t LocalSapicId;
	uint8_t LocalSapicEid;
uint16_t:BitfieldWidth(15, 0);
uint8_t:BitfieldWidth(7, 0);
	uint32_t Flags;
	uint32_t AcpiProcessorUidValue;
	char LocalSapicUidString[];
}LocalSAPICStructure;
typedef struct {
	APICHeader Header;
	uint16_t Flags;
	uint8_t InteruptType;
	uint8_t ProcessorId;
	uint8_t ProcessorEId;
	uint8_t IoSapicVector;
	uint32_t GlobalSystemInterrupt;
	uint32_t PlatformInterruptSourceFlags;
}PlatformInterruptSourceStructure;
typedef struct {
	APICHeader Header;
uint16_t:BitfieldWidth(15, 0);
	uint32_t X2ApicId;
	uint32_t Flags;
	uint32_t AcpiProcessorUid;
}ProcessorLocalx2APICStructure;
typedef struct {
	APICHeader Header;
	uint16_t Flags;
	uint32_t AcpiProcessorUid;
	uint8_t LocalX2ApicLINT;
uint16_t:BitfieldWidth(15, 0);
uint8_t:BitfieldWidth(7, 0);
}Localx2APICNMIStructure;
typedef struct {
	APICHeader Header;
uint16_t:BitfieldWidth(15, 0);
	uint32_t CpuInterfaceNumber;
	uint32_t AcpiProcessorUid;
	uint32_t Flags;
	uint32_t ParkingProtocolVersion;
	uint32_t PerformanceInterruptGsiv;
	uint64_t ParkedAddress;
	uint64_t PhysicalBaseAddress;
	uint64_t GICV;
	uint64_t GICH;
	uint32_t VGICMaintenanceInterrupt;
	uint64_t GICRBaseAddress;
	uint64_t MPIDR;
	uint8_t ProcessorPowerEfficiencyClass;
uint8_t:BitfieldWidth(7, 0);
	uint16_t SpeOverflowInterrupt;
}GICCPUInterfaceStructure;
typedef struct {
	APICHeader Header;
uint16_t:BitfieldWidth(15, 0);
	uint32_t GICId;
	uint64_t GICBaseAddress;
	uint32_t SystemVectorBase;
	uint8_t GICVersion;
uint16_t:BitfieldWidth(15, 0);
uint8_t:BitfieldWidth(7, 0);
}GICDistributorStructure;
typedef struct {
	APICHeader Header;
uint16_t:BitfieldWidth(15, 0);
	uint32_t GiCMsiFrameId;
	uint64_t PhysicalBaseAddress;
	uint32_t Flags;
	uint16_t SpiCount;
	uint16_t SpiBase;
}GICMSIFrameStructure;
typedef struct{
	APICHeader Header;
uint16_t: BitfieldWidth(15, 0);
	uint64_t DiscoveryRangeBaseAddress;
	uint32_t DiscoveryRangeLength;
}GICRedistributorStructure;
typedef struct {
	APICHeader Header;
uint16_t:BitfieldWidth(15, 0);
	uint32_t GicItsId;
	uint64_t PhysicalBaseAddress;
uint32_t:BitfieldWidth(31, 0);
}GICInterruptTranslationServiceStructure;
typedef struct {
	APICHeader Header;
	uint16_t MailboxVersion;
uint32_t:BitfieldWidth(31, 0);
	uint64_t MailboxAddress;
}MultiprocessorWakeupStructure;

//FINALY im done with the APIC structures. UGHHHHHHH

typedef struct {
	SystemDescriptionTableHeader Header;
	uint32_t WarningEnergyLevel;
	uint32_t LowEnergyLevel;
	uint32_t CriticalEnergyLevel;
}SmartBatteryTable;
typedef struct {
	SystemDescriptionTableHeader Header;
	uint8_t EcControl[12];
	uint8_t EcData[12];
	uint32_t Uid;
	uint8_t GpeBit;
	char EcID[];
}EmbeddedControllerBootResourcesTable;

#pragma pack(pop)






typedef struct {
	MultipleAPICDescriptionTable* MADT;
	uint64_t NumberOfEntries;
	APICHeader** Entrys;
}MADT_TABLES;