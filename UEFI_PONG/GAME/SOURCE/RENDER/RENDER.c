#pragma once
#include <../GAME/HEADER/RENDER/RENDER.h>
#pragma warning(push,0)
#include <string.h>
#include <gnu-efi/inc/efilib.h>
#pragma warning(pop)
#include "../../../HEADER/HEAP/HEAP.h"
#include <intrin.h>
static EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput = NULL;
static  UINT32* DoubleBuffer = NULLPTR;
size_t screen_mod_16;
size_t screen_mod_32;
void InitRender()
{
	Print(L"initializing Render...\n");
	EFI_STATUS Status;
	// Locate the Graphics Output Protocol
	Status = uefi_call_wrapper(
		GlobalST->BootServices->LocateProtocol,
		3,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		(void**)&GraphicsOutput
	);
	if (EFI_ERROR(Status)) {
		Print(L"Failed to locate GOP: %r\n", Status);
		return ;
	}
	GlobalFramebuffer.FrameBuffer = (UINT32*)GraphicsOutput->Mode->FrameBufferBase;
	GlobalFramebuffer. Width = GraphicsOutput->Mode->Info->HorizontalResolution;
	GlobalFramebuffer. Height = GraphicsOutput->Mode->Info->VerticalResolution;
	GlobalFramebuffer. PixelsPerScanLine = GraphicsOutput->Mode->Info->PixelsPerScanLine;
	//creating double buffer;
	DoubleBuffer = Alloc(sizeof(UINT32) * GlobalFramebuffer.PixelsPerScanLine * GlobalFramebuffer.Height);
	Print(L"allocated double buffer of size %d\n", sizeof(UINT32) * GlobalFramebuffer.PixelsPerScanLine * GlobalFramebuffer.Height);
	screen_mod_16 = (GlobalFramebuffer.PixelsPerScanLine * GlobalFramebuffer.Height * sizeof(UINT32)) % 16;
	screen_mod_32 = (GlobalFramebuffer.PixelsPerScanLine * GlobalFramebuffer.Height * sizeof(UINT32)) % 32;
	Print(L"screen_mod_16: %d\n", screen_mod_16);
	Print(L"screen_mod_32: %d\n", (GlobalFramebuffer.PixelsPerScanLine * GlobalFramebuffer.Height * sizeof(UINT32)) % 32);
	PrintHeap();
	Print(L"Render Initialized\n");
}

void ClearScreen()
{
	GlobalST->ConOut->ClearScreen(GlobalST->ConOut);

}

void DrawPixel(Vector2 point, uint32_t color)
{
	size_t h = GlobalFramebuffer.Height;
	size_t w = GlobalFramebuffer.PixelsPerScanLine;
	size_t a =point.y* w + point.x;
	DoubleBuffer[a] = color;
}

void DrawLine(Vector2 a, Vector2 b, uint32_t color)
{
	//f(x)= mx + b. 
	// we can remove b by using the point a as the origin
	Vector2 CurrentPoint = a;
	while (!EqualsVector2(CurrentPoint, b)) {
		DrawPixel(CurrentPoint, color);

		if (CurrentPoint.x < b.x) {
			CurrentPoint.x++;
		}
		else if (CurrentPoint.x > b.x) {
			CurrentPoint.x--;
		}
		if (CurrentPoint.y < b.y) {
			CurrentPoint.y++;
		}
		else if (CurrentPoint.y > b.y) {
			CurrentPoint.y--;
		}

	}
}

void DrawRectangle(Vector2 start, int width, int height, uint32_t color)
{
	Vector2 end = { start.x + width, start.y + height };
	// Draw the four sides of the rectangle
	DrawLine(start, (Vector2){ end.x, start.y }, color); // Top side
	DrawLine((Vector2){ end.x, start.y }, end, color); // Right side
	DrawLine(end, (Vector2){ start.x, end.y }, color); // Bottom side
	DrawLine((Vector2) { start.x, end.y }, start, color); // Left side
}

void DrawCircle(Vector2 center, int radius, uint32_t color)
{
	int x = radius, y = 0;
	int err = 0;
	while (x >= y) {
		DrawPixel((Vector2){ center.x + x, center.y + y }, color);
		DrawPixel((Vector2){ center.x + y, center.y + x }, color);
		DrawPixel((Vector2){ center.x - y, center.y + x }, color);
		DrawPixel((Vector2){ center.x - x, center.y + y }, color);
		DrawPixel((Vector2){ center.x - x, center.y - y }, color);
		DrawPixel((Vector2){ center.x - y, center.y - x }, color);
		DrawPixel((Vector2){ center.x + y, center.y - x }, color);
		DrawPixel((Vector2){ center.x + x, center.y - y }, color);
		if (err <= 0) {
			y += 1;
			err += 2 * y + 1;
		}
		if (err > 0) {
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}

void DrawText(Vector2 start, const char* text, uint32_t color) 
{
}

void RefreshScreen()
{

	size_t pixels = GlobalFramebuffer.Height * GlobalFramebuffer.PixelsPerScanLine;
	size_t i = 0;
	//something aint right about this code so ill just disable it
	//TODO: fix this code
	if (false) {
		// Copy 8 pixels = 32 bytes per iteratio
		for (; i + 8 <= pixels; i += 8) {
			__m256i data = _mm256_loadu_si256((__m256i*) & DoubleBuffer[i]);
			_mm256_storeu_si256((__m256i*) & GlobalFramebuffer.FrameBuffer[i], data);

			// Zero out backbuffer (optional if not drawing full frame)
			_mm256_storeu_si256((__m256i*) & DoubleBuffer[i], _mm256_setzero_si256());
		}
	}

		// Copy 4 pixels (16 bytes) at a time
		for (; i + 4 <= pixels; i += 4) {
			__m128i data = _mm_loadu_si128((__m128i*) & DoubleBuffer[i]);
			_mm_storeu_si128((__m128i*) & GlobalFramebuffer.FrameBuffer[i], data);
			_mm_storeu_si128((__m128i*) & DoubleBuffer[i], _mm_setzero_si128()); // Clear the double buffer
		}

		// Copy remaining pixels (1 by 1)
		for (; i < pixels; i++) {
			GlobalFramebuffer.FrameBuffer[i] = DoubleBuffer[i];
			DoubleBuffer[i] = 0; // Clear the double buffer
		}
	

//	size_t h = GlobalFramebuffer.Height;
//	size_t w = GlobalFramebuffer.PixelsPerScanLine;
//	if (screen_mod_16 == 0) {
//		//do copying with intriniscsics
//		for(size_t i = 0; i < h*w*sizeof(UINT32); i+=16) {
//			_mm_storeu_si128((__m128i*) & GlobalFramebuffer.FrameBuffer[i], _mm_loadu_si128((__m128i*) & DoubleBuffer[i]));
//			_mm_storeu_si128(&DoubleBuffer[i], _mm_setzero_si128());
//		}
//	}
	//memcpy(GlobalFramebuffer.FrameBuffer, DoubleBuffer, w * h*sizeof(UINT32));
	//memset(DoubleBuffer, 0, w * h*sizeof(UINT32));
}
