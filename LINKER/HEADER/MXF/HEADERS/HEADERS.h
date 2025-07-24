#pragma once
constexpr int MakeFourCC(char a, char b, char c, char d) {
	return (static_cast<int>(a) << 24) |
		(static_cast<int>(b) << 16) |
		(static_cast<int>(c) << 8) |
		(static_cast<int>(d));
}
#include "MAIN_HEADER.h"
#include "SECTION_POINTER_HEADER.h"
#include "RELOCATION_HEADER.h"