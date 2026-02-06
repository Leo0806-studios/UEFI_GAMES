#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
	/// <summary>
	/// inits render. only call any render functions after a call to this
	/// </summary>
	void InitRender();
	/// <summary>
	/// clears the screen
	/// </summary>
	void ClearScreen();
	/// <summary>
	/// draws a single pixel.
	/// this is the foundation of every other draw
	/// </summary>
	/// <param name="point"></param>
	/// <param name="color"></param>
	void DrawPixel(Vector2 point, uint32_t color);
	/// <summary>
	/// draws a line 1 pixel thick
	/// calls drawPixel internaly
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="color"></param>
	void DrawLine(Vector2 a, Vector2 b, uint32_t color);
	/// <summary>
	/// draws arecangle. calls draw Line internaly
	/// </summary>
	/// <param name="start"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="color"></param>
	void DrawRectangle(Vector2 start, int width, int height, uint32_t color);
	/// <summary>
	/// Draws a circle.
	/// calls Draw pixel internaly
	/// </summary>
	/// <param name="center"></param>
	/// <param name="radius"></param>
	/// <param name="color"></param>
	void DrawCircle(Vector2 center, int radius, uint32_t color);
	/// <summary>
	/// not yet impleemnted
	/// </summary>
	/// <param name="start"></param>
	/// <param name="text"></param>
	/// <param name="color"></param>
	void DrawText(Vector2 start, const char* text, uint32_t color);
	/// <summary>
	/// this flushes the seond buffer to the drawbuffer
	/// </summary>
	void RefreshScreen();
