#pragma once

struct GlobalStateStruct {
	void* imageBase = nullptr;
	size_t textOffset = 0;
	size_t dataOffset = 0;
	size_t bssOffset = 0;
	size_t rdataOffset = 0;
	size_t pdataOffset = 0;
	size_t xdataOffset = 0;
};