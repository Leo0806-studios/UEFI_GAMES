#include "../../../HEADER/SUBSYSTEMS/CONSOLE/CONSOLE.h"
#include "../../../HEADER/SUBSYSTEMS/RENDER/RENDER.h"
#include <STRING_.h>
namespace SYSTEM {
	namespace SUBSYSTEMS {
		namespace CONSOLE {
			class __CONSOLE__STRUCTURE {
			private:
				friend bool Console::InitConsole(size_t collums, size_t rows, size_t pixelsperRow, size_t pixelsperCollum);
			public:
				__CONSOLE__STRUCTURE(const __CONSOLE__STRUCTURE& other) = delete;
				__CONSOLE__STRUCTURE(__CONSOLE__STRUCTURE&& other) = delete;
				__CONSOLE__STRUCTURE() = default;
				__CONSOLE__STRUCTURE& operator=(const __CONSOLE__STRUCTURE& other) = delete;
				__CONSOLE__STRUCTURE& operator=(__CONSOLE__STRUCTURE&& other) = delete;
				char* buffer=nullptr;
				size_t bufferSize=0;
				size_t collums = 0;
				size_t rows = 0;

				size_t cursorPosRows = 0;
				size_t cursorPosCollums = 0;

				size_t pixelsPerRow = 0;
				size_t pixxelsPerCollum = 0;
			};
			static __CONSOLE__STRUCTURE& ConsoleStructure() {
				static __CONSOLE__STRUCTURE str;
				return str;
			}
			bool Console::InitConsole(size_t collums, size_t rows, size_t pixelsperRow, size_t pixelsperCollum)
			{
				__CONSOLE__STRUCTURE& console = ConsoleStructure();
				console.collums = collums;
				console.pixelsPerRow = pixelsperRow;
				console.pixxelsPerCollum = pixelsperCollum;
				console.rows = rows;
				return true;
			}
			void Console::WriteLine(const char* str)
			{
				__CONSOLE__STRUCTURE& console = ConsoleStructure();
				size_t x = console.pixxelsPerCollum * console.cursorPosCollums;
				size_t y = console.pixelsPerRow * console.cursorPosRows;
				RENDER::SIMPLE::SimpleDrawString(str, (unsigned int)x,(unsigned int) y);
				console.cursorPosRows++;

			}
			void Console::WriteF(const char* format, ...)
			{
			}
			void Console::Write(const char* str)
			{
			}
			ConsoleInfo QueryConsoleInfo()
			{
				return ConsoleInfo();
			}
			void Console::ClearConsole()
			{
			}
			void Console::Flush()
			{
			}
			void Console::ToggleANSICodes()
			{
			}
		}
	}
}