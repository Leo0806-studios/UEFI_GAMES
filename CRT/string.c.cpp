#include "string.h"

char* strcpy(char* restrict dest, const char* restrict src)
{
	size_t i = 0;
	for (i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	i++;
	dest[i] = '\0';

	return dest;
}

char* strncpy(char* restrict dest, const char* restrict src, size_t count)
{
	size_t i = 0;
	for (i = 0; i < count && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	for (;i < count; i++)
	{
		dest[i] = '\0';
	}
	return dest;
}

char* strcat(char* restrict dest, const char* restrict src)
{
	size_t srcLen = strlen(src);
	size_t destLen = strlen(dest);
	size_t i = 0;
	for (i = destLen; i < destLen + srcLen; i++)
	{
		dest[i] = src[i - destLen];
	}
	i++;
	dest[i] = '\0';
	return dest;
}

char* strncat(char* restrict dest, const char* restrict src, size_t count)
{
	size_t srcLen = strlen(src);
	size_t destLen = strlen(dest);
	size_t i = 0;
	size_t fixedCount = destLen + count;
	for (i = destLen; i < fixedCount && src[i - destLen] != '\0'; i++) {
		dest[i] = src[i - destLen];
	}
	for (; i < fixedCount; i++)
	{
		dest[i] = '\0';
	}

}

char* strdup(const char* src)
{
	return nullptr;
}
