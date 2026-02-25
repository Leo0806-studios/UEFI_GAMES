#pragma once
#include <stdint.h>
typedef enum {
	TypeInteger,
	TypeFloatingPoint,
	TypeBCD,
	TypeString,
	TypeByte,
	TypeBytes,
	TypeBoolean,
	TypeGuid,
	TypeDate

}AllowedValueTypes;
typedef struct {
	const char* Key;
	void* Value;
	AllowedValueTypes Type;

}BcdPair;
typedef struct {
	const char* 
}BcdSection;
#define BCD_VERSION 0001
typedef struct {
	uint32_t Version;
	uint64_t Sections
}BCD;