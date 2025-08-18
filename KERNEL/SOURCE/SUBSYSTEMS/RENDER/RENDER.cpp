// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "../../../HEADER/SUBSYSTEMS/RENDER/RENDER.h"
#include "../../../../FREESTANDING_STD++LIB/HEADER/STRING/STRING_F.h"
#include <stdint.h>
namespace SYSTEM::SUBSYSTEMS::RENDER {
	namespace SIMPLE {

		unsigned int* FramebufferBase = nullptr;
		unsigned int FramebufferWidth = 0;
		unsigned int FramebufferHeight = 0;
		unsigned int PixelsPerScanline = 0; // Number of bytes per row in the framebuffer
		//i might change the 8X8 BMP to be 16H and 8W //i did change it
		

		int TranslationTable[256] = { 
			0,			//\0
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			1,			//A
			2,			//B
			3,			//C
			4,			//D
			5,			//E
			6,			//F
			7,			//G
			8,			//H
			9,			//I
			10,			//J
			11,			//K
			12,			//K
			13,			//M
			14,			//N
			15,			//O
			16,			//P
			17,			//Q
			18,			//R
			19,			//S
			20,			//T
			21,			//U
			22,			//V
			23,			//W
			24,			//X
			25,			//Y
			26,			//Z
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			-1,			//
			27,			//a
			28,			//b
			29,			//c
			30,			//d
			31,			//e
			32,			//f
			33,			//g
			34,			//h	
			35,			//i
			36,			//j
			37,			//k
			38,			//l
			39,			//m
			40,			//n
			41,			//o
			42,			//p
			43,			//q
			44,			//r
			45,			//s
		};
		void SimpleDrawChar(unsigned int x, unsigned int y, wchar_t c ,unsigned int colour,unsigned int bg_colour)
		{
			//find char in translation table
			const int index = TranslationTable[static_cast<unsigned char>(c)];
			if (index < 0 || index >= 255) 
			{
				return SimpleDrawChar(x, y, 'A',colour,bg_colour); // Invalid character index
			}
			const SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::CHAR& character = SimpleFont[index];
			// Draw the character bitmap to the framebuffer
			for (unsigned int row = 0; row < character.height; ++row) {
				for (unsigned int col = 0; col < character.width; ++col) {
					if (character.BMP[row] & (1 << (character.width - 1 - col))) { // Check if the bit is set
						unsigned int pixelX = x + col;
						unsigned int pixelY = y + row;
						if (pixelX < FramebufferWidth && pixelY < FramebufferHeight) { // Ensure within bounds
							__assume(FramebufferBase != nullptr);
							FramebufferBase[pixelY * PixelsPerScanline + pixelX] = colour; // Set pixel color //-V108 //-V3539
						}
					}
					else {
						unsigned int pixelX = x + col;
						unsigned int pixelY = y + row;
						if (pixelX < FramebufferWidth && pixelY < FramebufferHeight) { // Ensure within bounds
							__assume(FramebufferBase != nullptr);
							FramebufferBase[pixelY * PixelsPerScanline + pixelX] = bg_colour; // Set pixel color //-V108 //-V3539
						}
					}
				}
			}

		}
		void SimpleDrawString(const wchar_t* str, unsigned int x, unsigned int y)
		{
			if (str == nullptr) { return; }
			const size_t len = STD::strlen(str);
			unsigned int xpos = x;
			const unsigned int ypos = y;
			if (len >= UINT32_MAX) {
				SimpleDrawString(L"String too long", x, y);
				return;
			}
			//we check beforehand if the string is too long
			for (size_t i = 0; i < len; i++) {
				if(str[i]==' '){ //-V3539
					xpos += 8; //-V127


					continue;
				}
				SimpleDrawChar(xpos,ypos,str[i],COLOURS::White,COLOURS::Black); //-V3539
				xpos += 8; //-V127

			}
		}
	}
}


