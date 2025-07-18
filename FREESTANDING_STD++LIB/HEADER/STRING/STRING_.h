#pragma once
namespace STD {
	static size_t strlen(const char* str) {
		size_t len = 0;
		while (str[len]) {
			len++;
		}
		return len;
	}
	static size_t strlen(const wchar_t* str) {
		size_t len = 0;
		while (str[len]) {
			len ++;
		}
		return len;
	}
}