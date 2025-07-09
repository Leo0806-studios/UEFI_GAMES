#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
	void InitRender();
	void ClearScreen();
	void DrawPixel(Vector2 point, uint32_t color);
	void DrawLine(Vector2 a, Vector2 b, uint32_t color);
	void DrawRectangle(Vector2 start, int width, int height, uint32_t color);
	void DrawCircle(Vector2 center, int radius, uint32_t color);
	void DrawText(Vector2 start, const char* text, uint32_t color);
	void RefreshScreen();
