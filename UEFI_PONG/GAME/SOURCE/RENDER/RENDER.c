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

typedef struct { 
	size_t xStart;
	size_t xEnd;
	size_t yStart;
	size_t yEnd;
	bool dirty;
}DrawCell;
//ill use 9 drawcells. in a 3X3 grid
DrawCell* Cellgrid[3][3];

static bool isInDrawCell(DrawCell* cell, Vector2 pixel) {
	bool xbounds = pixel.x >= cell->xStart && pixel.x < cell->xEnd;
	bool ybounds = pixel.y >= cell->yStart && pixel.y < cell->yEnd;
	return ybounds && xbounds;
}
void CreateCellGrid() {
	size_t ScreenHeigt = GlobalFramebuffer.Height;
	size_t screenWidth = GlobalFramebuffer.Width;
	size_t heightThirds = ScreenHeigt / 3;
	size_t WidthsThirds = screenWidth / 3;
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			Cellgrid[i][j] = Alloc(sizeof(DrawCell));
			Cellgrid[i][j]->dirty = false;
			Cellgrid[i][j]->xStart = j * WidthsThirds;
			Cellgrid[i][j]->xEnd = ((j + 1) * WidthsThirds) - 1;
			Cellgrid[i][j]->yStart = j * heightThirds;
			Cellgrid[i][j]->yEnd = ((j + 1) * heightThirds) - 1;

		}
}
}
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
	Print(L"Creating DrawCells...\n");
	CreateCellGrid();
	Print(L"Created drawCells\n");
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
	//find in wich cell the pixel is
	bool found = false;
	for (size_t i=0; i < 3; i++) {
		if (found) {
			break;
		}
		for(size_t j = 0; j < 3; j++) {
			if (isInDrawCell(Cellgrid[i][j], point)) {
				Cellgrid[j][i]->dirty = true;
				found = true;
				break;
			}
		}
	}
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
	///update refresh to make use of draw cells //Disabled as it doesnt fucking work
	//for (size_t ii = 0; ii < 3; ii++) {
	//	Print(L"Drawing line %d\n",ii);
	//	GlobalST->BootServices->Stall(100000);

	//	for (size_t j = 0; j < 3; j++) {


	//		DrawCell* cell = Cellgrid[j][ii];
	//		if (cell->dirty) {
	//			// Print(L"Cell %d , %d is clean. skipping\n",ii,j);
	//		//	GlobalST->BootServices->Stall(50000);
	//		goto skip; }
	//		///now copy the part of the doublebuffer that coresponds to the cell to the frame buffer
	//		Print(L"drawing Cell %d\n", j);
	//		GlobalST->BootServices->Stall(50000);
	//		const size_t CellStartx = cell->xStart;
	//		const size_t CellStarty = cell->yStart;
	//		const size_t CellEndx = cell->xEnd;
	//		const size_t CellEndy = cell->yEnd;

	//		//we itterace trough each line
	//		for (size_t indexY = CellStarty; indexY < CellEndy; indexY++) {
	//			//we take four elementss of a line (4 rouws ) at atime
	//			size_t indexX = CellStartx;
	//			for (; indexX + 4 < CellEndx; indexX += 4) {
	//				size_t BuffIndex = indexY * GlobalFramebuffer.PixelsPerScanLine + indexX;
	//				__m128i data = _mm_loadu_si128((__m128i*) & DoubleBuffer[BuffIndex]);
	//			//	_mm_storeu_si128((__m128i*) & GlobalFramebuffer.FrameBuffer[BuffIndex], data); //rn ill just fill it wiht a solid colour to make debug
	//			_mm_storeu_si128((__m128i*) & GlobalFramebuffer.FrameBuffer[BuffIndex], _mm_setr_epi32(0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF));

	//				_mm_storeu_si128((__m128i*) & DoubleBuffer[BuffIndex], _mm_setzero_si128()); // Clear the double buffer
	//			}
	//			//now copy the remaining one at a time
	//			for (; indexX < CellEndx; indexX++) {
	//				size_t BuffIndex = indexY * GlobalFramebuffer.PixelsPerScanLine + indexX;
	//				GlobalFramebuffer.FrameBuffer[indexX] = DoubleBuffer[indexX];
	//				DoubleBuffer[indexX] = 0; // Clear the double buffer
	//			}
	//		}
	//		cell->dirty = false;
	//		if (false) {
	//		skip:;
	//		}


	//	}
	//	GlobalST->BootServices->Stall(100000);

	//}


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
