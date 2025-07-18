#pragma once
namespace SYSTEM {
	namespace SUBSYSTEMS {
		/// <summary>
		/// the console subsystem
		/// </summary>
		namespace CONSOLE {
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
			/// inits the console. must be called before any other console function can safely be used.
			/// </summary>
			/// <param name="collums">amount of collums to use. does nothing rn</param>
			/// <param name="rows">amount of rows. does nothing rn</param>
			/// <param name="pixelsperRow">influences vertial spacing should larger than the max size of a character or clipping might occur.</param>
			/// <param name="pixelsperCollum">influences horizontal spacing. should be larger than the widht o a character or clipping might occur</param>
			/// <returns>succses is true</returns>
			static bool InitConsole( size_t collums, size_t rows, size_t pixelsperRow,size_t pixelsperCollum);
			/// <summary>
			/// writes a line to the console out
			/// </summary>
			/// <param name="str"></param>
			static void WriteLine(const char* str);
			/// <summary>
			/// analog to printf. takes a format string and avariable amount of input
			/// additinaly it takse the amount of extra args as a parameter to increase safety
			/// </summary>
			/// <param name="format"></param>
			/// <param name=""></param>
			static void WriteF(size_t args ,const char* format, ...);
			/// <summary>
			/// writes an unformated string to the console.
			/// </summary>
			/// <param name="str"></param>
			static void Write(const char* str);
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
			/// does nothing in early init while redering draws directly to the framebuffer
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