#pragma once
#include <stdbool.h>
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
	 char* str;
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
	bool Writable;
}BcdSection;
#define BCD_VERSION 0001
typedef struct {
	uint32_t Version;
	uint64_t Count;
	BcdSection* Sections;
}BCD;

uint64_t StrHash(const char* str);

BCD* ParseBCD(const char* bcdStr);

FreeBCD(BCD* bcd);
/// <summary>
/// looks up a entry in a bcd section by its string key. returns a pointer to the pair if found, otherwise returns NULL. the section must be provided as a pointer to avoid having to look it up by string again. if you dont have a pointer to the section use FindPairDirect or FindPairDirectHash instead which will look up the section for you but will be slower.
/// </summary>
/// <param name="self"></param>
/// <param name="section"></param>
/// <param name="key"></param>
/// <returns></returns>
BcdPair* FindPair(BCD* self, BcdSection* section, const char* key);

/// <summary>
/// Finds a pair in a BCD section by its hash value.
/// note that this still itterates in o(n). the hash is just to skipp the string comparsion. the section must be provided as a pointer to avoid having to look it up by string again. if you dont have a pointer to the section use FindPairDirect or FindPairDirectHash instead which will look up the section for you but will be slower.
/// </summary>
/// <param name="self">Pointer to the BCD structure to search in.</param>
/// <param name="section">Pointer to the BCD section containing the pair.</param>
/// <param name="hash">The hash value to search for.</param>
/// <returns>Pointer to the found BcdPair, or null if not found.</returns>
BcdPair* FindPairHash(BCD* self, BcdSection* section, uint64_t hash, const char* fallback);
BcdPair* FindPairDirect(BCD* self, const char* section, const char* key);
BcdPair* FindPairDirectHash(BCD* self, uint64_t hashSection, const char* fallbackSection, uint64_t hashKey, const char* fallbackKey);
BcdSection* FindSection(BCD* self, const char* section);
BcdSection* FindSectionHash(BCD* self, uint64_t hash, const char* fallback);