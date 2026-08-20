#include "BCD.h"
#include "HEAP/HEAP.h"
#include <stdbool.h>
static bool isspace(unsigned char c) {
	return (c == ' ') || (c == '\t');
}
static bool isdigit(unsigned char c) {
	return (c >= '0') && (c <= '9');
}
static uint64_t atoi(const char* s) {
	uint64_t ret = 0;
	int sign = 1;
	while (isspace((unsigned char)*s)) s++;
	if (*s == '-') { sign = -1.0; s++; }
	else if (*s == '+') s++;
	while (isdigit((unsigned char)*s)) { ret = ret * 10 + ((uint64_t)*s - '0'); s++; }
	return ret;

}
static double atof(const char* s) {
	double a = 0.0, sign = 1.0;
	int e = 0;
	while (isspace((unsigned char)*s)) s++;
	if (*s == '-') { sign = -1.0; s++; }
	else if (*s == '+') s++;
	while (isdigit((unsigned char)*s)) { a = a * 10.0 + (*s - '0'); s++; }
	if (*s == '.') {
		s++;
		while (isdigit((unsigned char)*s)) { a = a * 10.0 + (*s - '0'); e--; s++; }
	}
	if (*s == 'e' || *s == 'E') {
		s++; int esign = 1, exp = 0;
		if (*s == '-') { esign = -1; s++; }
		else if (*s == '+') s++;
		while (isdigit((unsigned char)*s)) { exp = exp * 10 + (*s - '0'); s++; }
		e += (exp * esign);
	}
	while (e > 0) { a *= 10.0; e--; }
	while (e < 0) { a *= 0.1; e++; }
	return a * sign;
}


uint64_t strlenTerm(const char* str, char term) {
	uint64_t len = 0;
	for (; str[len] && str[len] != term; len++);
	return len;
}
static void MEMSET(char* src, char val, uint64_t size) {
	for (uint64_t i = 0; i < size; i++) {
		src[i] = val;
	}
}
uint64_t StrHash(const char* str)
{
#define FNV_OFFSET_BASIS 14695981039346656037ULL
#define FNV_PRIME        1099511628211ULL
	uint64_t hash = FNV_OFFSET_BASIS;

	while (*str)
	{
		hash ^= (uint8_t)*str;
		hash *= FNV_PRIME;
		str++;
	}
#undef FNV_OFFSET_BASIS
#undef FNV_PRIME

	return hash;
}
static uint64_t CountSections(const char* str) {
	uint64_t count = 0;
	for (uint64_t i = 0; str[i]; i++) {
		if (str[i] == '[') {
			count++;
		}
	}
	return count;

}
static uint64_t CountLines(const char* str) {
	uint64_t count = 0;
	for (uint64_t i = 0; str[i]; i++) {
		if (str[i] == '\n') {
			count++;
		}
	}
	return count;
}
static uint64_t CountComments(const char* str) {
	uint64_t count = 0;
	for (uint64_t i = 0; str[i]; i++) {
		if (str[i] == '\n') {
			if (str[i + 1] == '#') {

				count++;
			}
		}
	}
	return count;
}
/// <summary>
/// itterites str utnill i finds the CurrentLine th new line and returns a pointer to one past that 
/// returns nullptr if no currentLine th newline can be found or if the next char would be invalid
/// if CurrentLine is negative it returns Str
/// </summary>
/// <param name="str"></param>
/// <param name="CurrentLine"></param>
/// <returns></returns>
static const char* GetNextLine(const char* str, int64_t CurrentLine) {
	if (CurrentLine < 0) {
		return str;
	}
	uint64_t cnt = 0;
	for (uint64_t index = 0; str[index]; index++) {
		if (str[index] == '\n') {
			cnt++;
		}
		if (cnt == CurrentLine) {
			if (str[index + 1] == '\0') {
				return NULL;
			}
			return &str[index + 1];
		}
	}
}
typedef struct {
	char* str1;
	char* str2;
}split;
split SplitStr(const char* str, char term) {
	split ret = { 0 };
	uint64_t len1 = strlenTerm(str, term);
	ret.str1 = Alloc(len1 + 1);
	if (!ret.str1) {
		ret.str2 = NULL;
		return ret;
	}
	MEMSET(ret.str1, 0, len1 + 1);
	for (uint64_t i = 0; i < len1; i++) {
		ret.str1[i] = str[i];
	}
	uint64_t len2 = strlenTerm(str, '\0') - len1 - 1;
	ret.str2 = Alloc(len2 + 1);
	if (!ret.str2) {
		DeAlloc(ret.str1);
		ret.str1 = NULL;
		return ret;
	}
	MEMSET(ret.str2, 0, len2 + 1);
	for (uint64_t i = 0; i < len2; i++) {
		ret.str2[i] = str[len1 + 1 + i];
	}
	return ret;
}


/// <summary>
/// parses the section header and inserts it into the bcd
/// </summary>
/// <param name="bcd"></param>
/// <param name="line"></param>
/// <param name="i"></param>
/// <returns></returns>
BcdSection* ParseSectionHeader(BCD* bcd, const char* line, uint64_t i) {
	if (!bcd || !line) {
		return NULL;
	}
	BcdSection* ret = Alloc(sizeof(BcdSection));
	if (!ret) {
		FreeBCD(ret);
		return NULL;
	}
	bcd->Sections[ret->Count] = ret;

	bcd->Count++;
	const uint64_t sectionNameLen = strlenTerm(&line[i + 1], ']');
	ret->Key.str = Alloc(sectionNameLen + 1);
	if (!ret->Key.str) {
		FreeBCD(bcd);
		return NULL;
	}
	MEMSET(ret->Key.str, 0, sectionNameLen + 1);
	for (uint64_t j = 0; j < sectionNameLen; j++) {
		ret->Key.str[j] = line[i + 1 + j];
	}
	ret->Key.hash = StrHash(ret->Key.str);
	return ret;
}
ValueStorage ParseData(const char* data, AllowedValueTypes Type) {
	ValueStorage ret = { 0 };
	switch (Type) {
	case TypeBCD: {
		break;
	}
	case TypeBoolean: {
		ret.valueBoolean = '0' - data[1];
		break;
	}
	case TypeByte: {
		ret.valueByte = data[1];
		break;
	}
	case TypeBytes: {
		uint64_t len = strlenTerm((++data), '"');
		ret.valueBytes = Alloc(len);
		if (!ret.valueBytes) {
			break;
		}
		data++;
		for (uint64_t i = 0; data[i] != '"' && data[i]; i++) {
		
		}
		break;
	}
	case TypeDate: {
		break;
	}
	case TypeFloatingPoint: {

		ret.valueFloatingPoint = atof(data + 1);//valid as it should stop once it hits the closing qoute
	}
	case TypeGuid: {

	}
	case TypeInteger: {

	}
	case TypeString: {
		break;
	}
	default: {

		break;
	}
		   return ret;
	}


}
/// <summary>
/// parses the entry and inserts it into the section passed in
/// </summary>
/// <param name="bcd"></param>
/// <param name="line"></param>
/// <param name="section"></param>
/// <returns></returns>
BcdPair* ParseEntry(BCD* bcd, const char* line, BcdSection** section) {
	if (!(*section)) {
		//entry without a section is invalid
		FreeBCD(bcd);
		return NULL;
	}
	const split pair = SplitStr(line, ':');
	if (!pair.str1 || !pair.str2) {
		//failed to split the line into a key and value
		if (pair.str1) {
			DeAlloc(pair.str1);
		}
		if (pair.str2) {
			DeAlloc(pair.str2);
		}
		FreeBCD(bcd);
		return NULL;
	}
	BcdPair* KeyValuePair = Alloc(sizeof(BcdPair));
	if (!KeyValuePair) {
		DeAlloc(pair.str1);
		DeAlloc(pair.str2);
		FreeBCD(bcd);
		return NULL;
	}
	AllowedValueTypes type = pair.str1[0];
	KeyValuePair->Value = ParseData(pair.str2, type);



}

static bool HamdleLine(BCD* bcd, BcdSection** currentSection, const char* line)
{
	if (!bcd || !currentSection || !line) {
		return NULL;
	}
	for (uint64_t i = 0; (line[i] != '\n') && line[i]; i++) {
		if (line[i] == '#') {
			//comment line skip
			break;
		}
		else if (line[i] == '[') {
			//section start
			*currentSection = ParseSectionHeader(bcd, line, i);
			if (!*currentSection) {
				return NULL;
			}
		}
		else {
			//everything else must by definition be an entry
			BcdPair* pair = ParseEntry(bcd, line, currentSection);
			if (!pair) {
				return NULL;
			}


		}
	}
	return true;
}


BCD* ParseBCD(const char* bcdStr)
{
	const uint64_t countSections = CountSections(bcdStr);
	const uint64_t CommentCount = CountComments(bcdStr);
	const uint64_t lines = CountLines(bcdStr);
	uint64_t totalIndex = 0;
	BCD* ret = Alloc(sizeof(BCD));
	if (!ret) {
		return NULL;
	}
	ret->Sections = Alloc(sizeof(BcdSection*) * countSections);
	int64_t currentLine = -1;
	BcdSection* currentSection = NULL;
	while (true) {
		const char* linePtr = GetNextLine(bcdStr, currentLine);
		if (!linePtr) {
			break;
		}
		if (!HamdleLine(ret, currentSection, linePtr)) {
			return NULL;
		}

		currentLine++;
	}

	return ret;
}

FreeBCD(BCD* bcd)
{
}
