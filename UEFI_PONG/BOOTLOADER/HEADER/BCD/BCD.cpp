#include "BCD.h"
#include "HEAP/HEAP.h"
#include <stdbool.h>
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
			if(str[i + 1] == '#'){

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
	split ret;
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
	uint64_t len2 = strlenTerm(str,'\0') - len1 - 1;
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
BCD* ParseBCD(const char* bcdStr)
{
	const uint64_t countSections = CountSections(bcdStr);
	const uint64_t CommentCount = CountComments(bcdStr);
	const uint64_t lines = CountLines(bcdStr);
	 uint64_t totalIndex = 0;
	BCD*  ret = Alloc(sizeof(BCD));
	if (!ret) {
		return NULL;
	}
	ret->Sections = Alloc(sizeof(BcdSection) * countSections);
	int64_t currentLine = -1;
		BcdSection currentSection = { 0 };
	while (true) {
		const char* linePtr = GetNextLine(bcdStr, currentLine);
		if (!linePtr) {
			break;
		}
		for (uint64_t i = 0;  (linePtr[i]!='\n')&&linePtr[i]; i++) {
			if (linePtr[i] == '#') {
				//comment line skip
				break;
			}
			else if (linePtr[i] == '[') {
				//section start

				ret->Sections[ret->Count] = currentSection;
				
				ret->Count++;
				const uint64_t sectionNameLen = strlenTerm(&linePtr[i + 1], ']');
				currentSection.Key.str = Alloc(sectionNameLen + 1);
				if (!currentSection.Key.str) {
					FreeBCD(ret);
					return NULL;
				}
				MEMSET(currentSection.Key.str, 0, sectionNameLen + 1);
				for (uint64_t j = 0; j < sectionNameLen; j++) {
					currentSection.Key.str[j] = linePtr[i + 1 + j];
				}
				currentSection.Key.hash = StrHash(currentSection.Key.str);
			}
			else {
				//everything else must by definition be an entry
				if (!currentSection.Key.str) {
					//entry without a section is invalid
					FreeBCD(ret);
					return NULL;
				}
				const split pair = SplitStr(linePtr, ':');
				if (!pair.str1 || !pair.str2) {
					//failed to split the line into a key and value
					if (pair.str1) {
						DeAlloc(pair.str1);
					}
					if (pair.str2) {
						DeAlloc(pair.str2);
					}
					FreeBCD(ret);
					return NULL;
				}

			}
		}
		currentLine++;
	}

	return ret;
}
