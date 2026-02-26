#include "BCD.h"
#include "HEAP/HEAP.h"
#include <stdbool.h>

uint64_t StrHash(const char* str)
{

	uint64_t len = 0;
	for (; str[len]; len++);
	uint64_t seed = 0;
	uint64_t index = 0;

	for (; index + 8 < len; index += 8) {
		seed ^= (*((const uint64_t*)&str[index])) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	for (; index < len; index++) {
		seed ^= ((uint64_t)str[index]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;
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
const char* GetNextLine(const char* str, int64_t CurrentLine) {
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
BCD* ParseBCD(const char* bcdStr)
{
	const uint64_t countSections = CountSections(bcdStr);
	const uint64_t CommentCount = CountComments(bcdStr);
	const uint64_t lines = CountLines(bcdStr);
	 uint64_t totalIndex = 0;
	BCD*const  ret = Alloc(sizeof(BCD));
	if (!ret) {
		return NULL;
	}
	int64_t currentLine = -1;
	while (true) {
		const char* linePtr = GetNextLine(bcdStr, currentLine);

	}

	return ret;
}
