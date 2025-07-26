#pragma once
namespace SYSTEM::SUBSYSTEMS::RENDER {
	namespace COLOURS {
		constexpr unsigned int White = 0xffffffffU; //-V112
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
		/// represents a 16X8 hardcode char for a simple font used in the early stages of initialization of the kernel
		/// </summary>
		struct CHAR {
			char c; // the char itself
			/// <summary>
			/// the actual width of the char. can be used to improve spacing between characters
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
		/// a hardcode external font
		/// for now only contains the ASCII uppercase Latin letters
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
		void SimpleDrawChar(unsigned int x, unsigned int y, wchar_t c); //-V2565 // contains direct recursion on a invalid char. will only recur once as the char printed instead is always valid

		 /// <summary>
		 /// draws a string to the framebuffer at the specified position.
		 /// calls SimpleDrawChar for each char in the string.
		 ///cant handle any control characters.
		 /// treats every char as 10 pixels wide. does not handle wrapping
		 /// </summary>
		 /// <param name="str"></param>
		 /// <param name="x"></param>
		 /// <param name="y"></param>
		 void SimpleDrawString(const wchar_t* str, unsigned int x, unsigned int y); //-V2565 // contains direct recursion on a too long string. will only recur once as the string printed instead is always short enough

	}
	/// <summary>
	/// render state class.
	/// a global singleton that holds the current render state.
	/// </summary>
	class RenderState {
	private:
		


		RenderState* Singeton = nullptr;
	};
}