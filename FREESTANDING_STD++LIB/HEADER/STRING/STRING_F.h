#pragma once
namespace STD {
	static size_t strlen(const char* str) {
		size_t len = 0;
		if (str == nullptr) {
			return 0; // Handle null pointer case
		}
		while (str[len]) { //-V3539
			len++;
		}
		return len;
	}
	static size_t strlen(const wchar_t* str) {
		size_t len = 0;
		if(str==nullptr) {
			return 0; // Handle null pointer case
		}
		while (str[len]) { //-V3539
			len ++;
		}
		return len;
	}
	static void* memcpy(void* dest, const void* src, size_t count) {
		if (dest == nullptr || src == nullptr) {
			return nullptr; // Handle null pointer case
		}
		char* d = static_cast<char*>(dest);
		const char* s = static_cast<const char*>(src);
		for (size_t i = 0; i < count; ++i) {
			d[i] = s[i];
		}
		return dest;
	}
}