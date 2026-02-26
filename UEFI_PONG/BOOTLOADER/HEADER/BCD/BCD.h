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
	const char* str;
	uint64_t hash;
}StringHashPair;
typedef struct {
	StringHashPair Key;
	void* Value;
	AllowedValueTypes Type;

}BcdPair;
typedef struct {
	StringHashPair Key;
	uint64_t Count;
	BcdPair* Pairs;
}BcdSection;
#define BCD_VERSION 0001
typedef struct {
	uint32_t Version;
	uint64_t Count;
	BcdSection* Sections;
}BCD;

uint64_t StrHash(const char* str);

BCD* ParseBCD(void);

FreeBCD(BCD* bcd);
BcdPair* FindPair(BCD* self, BcdSection* section, const char* key);
BcdPair* FindPairHash(BCD* self, BcdSection* section, uint64_t hash);
BcdPair* FindPairDirect(BCD* self, const char* section, const char* key);
BcdPair* FindPairDirectHash(BCD* self, uint64_t hashSection, uint64_t hashKey);
BcdSection* FindSection(BCD* self, const char* section);
BcdSection* FindSectionHash(BCD* self, uint64_t hash);