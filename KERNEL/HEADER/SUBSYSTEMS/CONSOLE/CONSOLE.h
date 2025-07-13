#pragma once
namespace SYSTEM {
	namespace SUBSYSTEMS {
		namespace CONSOLE {
			struct ConsoleInfo {
				size_t buffer;
				size_t collums;
				size_t rows;
				bool scrollingEnabled;

			};
			class Console {
			public:
			static bool InitConsole( size_t collums, size_t rows, size_t pixelsperRow,size_t pixelsperCollum);
			static void WriteLine(const char* str);
			static void WriteF(const char* format, ...);
			static void Write(const char* str);
			static ConsoleInfo QueryConsoleInfo();
			static void ClearConsole();
			static void Flush();
			static void ToggleANSICodes();
			};

		}
	}
}