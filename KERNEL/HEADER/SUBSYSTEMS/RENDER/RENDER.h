#pragma once
namespace SYSTEM::SUBSYSTEMS::RENDER {
	namespace COLOURS {
		constexpr unsigned int White = 0xffffffff;
	}
	/// <summary>
	/// simple render interface to be used in pre driver mode. all draw functions in this namespace are used to draw to the framebuffer directly.
	/// </summary>
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
		/// <summary>
		/// a hardcoded external font
		/// for now only contains the ASCII upercase latin letters
		/// </summary>
		extern CHAR SimpleFont[255];

		/// <summary>
		/// draws a char to the framebuffer at the specified position.
		/// will pint a A if the char is not in the font.
		/// draws directly to the framebuffer.
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="c"></param>
		void SimpleDrawChar(unsigned int x, unsigned int y, char c);

		 /// <summary>
		 /// draws a string to the framebuffer at the specified position.
		 /// calls SimpleDrawChar for each char in the string.
		 ///cant handle any controll characters.
		 /// treats every char as 10 pixels wide. does not handle wrapping
		 /// </summary>
		 /// <param name="str"></param>
		 /// <param name="x"></param>
		 /// <param name="y"></param>
		 void SimpleDrawString(const char* str, unsigned int x, unsigned int y);

	}
	/// <summary>
	/// redner state class.
	/// a global singeton that holds the current render state.
	/// </summary>
	class RenderState {
	private:
		


		RenderState* Singeton = nullptr;
	};
}