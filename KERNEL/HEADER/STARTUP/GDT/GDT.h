#pragma once
namespace SYSTEM {
	namespace STARTUP {
		/// <summary>
		/// everything GDT related is contained in this namespace.
		/// this include the GDT entries and functions like creating it and loading it.
		/// the GDT itself is stored in the N SYSTEM_INFO namespace.
		/// </summary>
		namespace GDT {
			/// <summary>
			/// entry inside the GDT.
			/// 
			/// </summary>
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