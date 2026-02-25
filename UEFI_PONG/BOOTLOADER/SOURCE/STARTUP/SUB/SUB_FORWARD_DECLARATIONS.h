#pragma once
#include "ACPI_STRUCTS.h"
#include "BOOT_DATA_INTERFACE.h"

CPUID ParseCPUID(void);
void PrintFADT(FixedACPIDescriptionTable* fadt, const char* intendent);
ACPI_TABLES ParseXACPI(ExtendedDescriptionPointer* ptr);


#define MiB(x) ((UINT64)(x) << 20)
#define GiB(x) ((UINT64)(x) << 30)
#define TABW4 "    "
#define TABW8 "        "
#define TABW12 "            "
#define TABW16 "                "
#define TABW20 "                    "
#define TABW24 "                        "
#define TABW28 "                            "
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
