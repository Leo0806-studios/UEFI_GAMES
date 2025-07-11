#pragma once
namespace SYSTEM::SUBSYSTEMS::RENDER {
	namespace SIMPLE {
		extern unsigned int* FramebufferBase;
		extern unsigned int FramebufferWidth;
		extern unsigned int FramebufferHeight;
		extern unsigned int PixelsPerScanline;

		struct CHAR {
			char c; // the char itself
			/// <summary>
			/// the actua lwidth of the char. can be used to improve spacing between characters
			/// </summary>
			unsigned char width;
				/// <summary>
				/// the actual height of the char. can be used to improve spacing between characters
				/// </summary>
				unsigned char height;
		private:
		public:
			/// <summary>
			/// the colour of the char
			/// </summary>
			unsigned int Color;
			/// <summary>
			/// bitmap of the char. max cvhar size is 8X8
			/// </summary>
			unsigned char BMP[8];
 		};
		extern CHAR SimpleFont[255];
		extern void SimpleDrawChar(unsigned int x, unsigned int y, char c);

	}
	class RenderState {
	private:
		


		RenderState* Singeton = nullptr;
	};
}