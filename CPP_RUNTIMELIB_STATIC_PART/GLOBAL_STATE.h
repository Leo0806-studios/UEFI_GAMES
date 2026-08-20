//###########################################
//				GLOBAL_STATE.h
//	Copyright (C) Leo0806 2025
//	Created -
//	Created by Leo0806
//	Last modified 03.09.2025
//	This file is part of the cpp runtime
//	this file contains the typedef for the internal global state of the runtime
//############################################

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