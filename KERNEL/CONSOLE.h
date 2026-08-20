#pragma once
namespace SYSTEM {
	namespace SUBSYSTEMS {
		/// <summary>
		/// the console subsystem
		/// </summary>
		namespace CONSOLE {
			constexpr inline size_t tabSize = 4; //the size of a tab in the console. this is used to calculate the cursor position when a tab is encountered
			/// <summary>
			/// contains information about the console
			/// 
			/// </summary>
			struct ConsoleInfo {
			private:
				
			public:
				size_t buffer;
				size_t collums;
				size_t rows;
				bool scrollingEnabled;

			};

			class Console {
			public:
			/// <summary>
			/// Initializes the console. must be called before any other console function can safely be used.
			/// </summary>
			/// <param name="collums">amount of columns to use. does nothing currently</param>
			/// <param name="rows">amount of rows. does nothing currently</param>
			/// <param name="pixelsperRow">influences vertical spacing should larger than the max size of a character or clipping might occur.</param>
			/// <param name="pixelsperCollum">influences horizontal spacing. should be larger than the width o a character or clipping might occur</param>
			/// <returns>success is true</returns>
			static bool InitConsole( size_t collums, size_t rows, size_t pixelsperRow,size_t pixelsperCollum);
			/// <summary>
			/// writes a line to the console out
			/// </summary>
			/// <param name="str"></param>
			static void WriteLine(const wchar_t* str);
			/// <summary>
			/// analog to printf. takes a format string and a variable amount of input
			/// additionally it takes the amount of extra args as a parameter to increase safety
			/// </summary>
			/// <param name="format"></param>
			/// <param name=""></param>
			static void WriteF(size_t args ,const char* format, ...);
			/// <summary>
			/// writes an unformatted string to the console.
			/// </summary>
			/// <param name="str"></param>
			static void Write(const wchar_t* str);
			/// <summary>
			/// returns a instance of ConsoleInfo containing information about the console
			/// </summary>
			/// <returns></returns>
			static const ConsoleInfo QueryConsoleInfo();
			/// <summary>
			/// clears the console and moves the cursor to 0,0
			/// </summary>
			static void ClearConsole();
			/// <summary>
			/// flushes the console buffer to the screen
			/// does nothing in early stages while rendering draws directly to the framebuffer
			/// </summary>
			static void Flush();
			/// <summary>
			/// toggles if the console should use ANSICodes or not.
			/// this is used to enable or disable color and other formatting codes.
			/// returns true if the codes are now enabled, false if they are now disabled.
			/// </summary>
			static bool ToggleANSICodes();
			};

		}
	}
}