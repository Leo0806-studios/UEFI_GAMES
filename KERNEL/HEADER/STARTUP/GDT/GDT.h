#pragma once
#include "HEADER/SAL/SAL_F.h"
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
			private:
				unsigned __int64 packedData = 0; //packed data for the GDT entry
			public:
				/// <summary>
				/// Sets the limit value for a GDT (Global Descriptor Table) entry.
				/// limit must only have the lower 20 bits set.
				/// returns false on failure.
				/// </summary>
				/// <param name="limit">The new limit value to set for the GDT entry.</param>
				/// <returns>Returns true if the limit was set successfully; otherwise, returns false.</returns>
				NODISCARD_MSG("discarding the return of this function can lead to a corrupted GDT entry")
					bool SetLimit(unsigned int limit);

				/// <summary>
				/// Sets the base address for a GDT entry.
				/// </summary>
				/// <param name="base">The base address to set for the GDT entry.</param>
				/// <returns>True if the base was set successfully; false otherwise. Discarding the return value can lead to a corrupted GDT entry.</returns>
				NODISCARD_MSG("discarding the return of this function can lead to a corrupted GDT entry")
				bool SetBase(unsigned __int32 base);

				/// <summary>
				/// Sets the access byte for a GDT entry.
				/// returns false on failure.
				/// DPL is only allowed to the lowest 2 bits set.(so 0-3 are valid values)
				/// </summary>
				/// <param name="present"></param>
				/// <param name="dpl"></param>
				/// <param name="segmentType"></param>
				/// <param name="executable"></param>
				/// <param name="directionOrConforming"></param>
				/// <param name="readWrite"></param>
				/// <param name="accessed"></param>
				/// <returns></returns>
				NODISCARD_MSG("discarding the return of this function can lead to a corrupted GDT entry")
					bool SetAcccsesByte(bool present, unsigned char DPL, bool segmentType, bool executable, bool directionOrConforming, bool readWrite, bool accessed);

				/// <summary>
				/// sets the flags for a GDT entry.
				/// returns false on failure.
				/// </summary>
				/// <param name="granularity"></param>
				/// <param name="size"></param>
				/// <param name="longMode"></param>
				/// <returns></returns>
				NODISCARD_MSG("discarding the return of this function can lead to a corrupted GDT entry")
				bool SetFlags(bool granularity, bool size, bool longMode);
			};
			class GDT {
				unsigned char PackedData[sizeof(void*) + sizeof(short)];//packed data. a pointer to this will actually be passed to lgdt
				GDT_ENTRY* GDT_Entries = nullptr; //pointer to the GDT entries
				size_t GDT_Entries_Size = 0; //size of the GDT entries in bytes
				size_t GDT_Entries_Count = 0; //number of GDT entries
				size_t GDT_Amount_Pages = 0; //number of pages allocated for the GDT

			public:
				/// <summary>
				/// allocates the GDT
				/// will allocate physical memory that needs to be taken care of when paging is enabled later on.
				/// </summary>
				/// <returns> success</returns>
				static bool CreateGDT();


				/// <summary>
				/// / Loads the GDT into the CPU.
				/// </summary>
				/// <returns></returns>
				NODISCARD_MSG("discarding the return of the value can leave the system in an undetected invalid state")
				 bool LoadGDT(void* gdtPtr);
			};
		}
	}
}