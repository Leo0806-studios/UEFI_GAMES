#pragma once
namespace SYSTEM {
	namespace STARTUP {
		namespace GDT {
			struct GDT_ENTRY {
				unsigned short limit;
				unsigned short base;
				unsigned char _base;
				unsigned char accses_byte;
				unsigned char limit_flags;
				unsigned  char __base;

				GDT_ENTRY() = default;
			};
		}
	}
}