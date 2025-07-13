#pragma once
namespace SYSTEM::SUBSYSTEMS::RENDER {
	namespace SIMPLE {
		extern unsigned int* FramebufferBase;
		extern unsigned int FramebufferWidth;
		extern unsigned int FramebufferHeight;
		extern unsigned int PixelsPerScanline;
		/// <summary>
		/// represents a 16X8 hardcoded char for asimple font used in the early stages of initalisation of the kernel
		/// </summary>
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
			/// bitmap of the char. max char  size is 16X8
			/// </summary>
			unsigned char BMP[16];
 		};
		extern CHAR SimpleFont[255];
		void SimpleDrawChar(unsigned int x, unsigned int y, char c);
		 void SimpleDrawString(const char* str, unsigned int x, unsigned int y);

	}
	class RenderState {
	private:
		


		RenderState* Singeton = nullptr;
	};
}