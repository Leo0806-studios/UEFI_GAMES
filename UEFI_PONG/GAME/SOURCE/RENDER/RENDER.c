#pragma once
#include <../GAME/HEADER/RENDER/RENDER.h>
#pragma warning(push,0)
#include <gnu-efi/inc/efilib.h>
#pragma warning(pop)
#include "../../../HEADER/HEAP/HEAP.h"
static EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput = NULL;
static UINT32* DoubleBuffer = NULLPTR;

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
	Print(L"Render Initialized\n");
}

void ClearScreen()
{
	GlobalST->ConOut->ClearScreen(GlobalST->ConOut);

}

void DrawPixel(Vector2 point, uint32_t color)
{
	int a =point.y* GlobalFramebuffer.PixelsPerScanLine + point.x;
	GlobalFramebuffer.FrameBuffer[a] = color;
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
	for (size_t i = 0;i  < GlobalFramebuffer.Height*GlobalFramebuffer.PixelsPerScanLine; i++) {
		GlobalFramebuffer.FrameBuffer[i] = DoubleBuffer[i];

	}
}
