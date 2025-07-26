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
}