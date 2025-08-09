#include "HEADER/STARTUP/GDT/GDT.h"
#include "HEADER/SUBSYSTEMS/ALLOCATION/ALLOCATION.h"
#include "HEADER/SYTEM_INFO/SYSTEM_INFO.h"
#include "HEADER/UTILLITY/UTILLITY_F.h"
#include <intrin.h>
using PhysicalAllocator = SYSTEM::SUBSYSTEMS::ALLOCATION::PhysicalAllocator;
extern "C" {
	void  LoadGDT(void* gdt, unsigned int size);//call into custom assembly to load the GDT
}
namespace SYSTEM {
	namespace STARTUP {
		namespace GDT {
			bool GDT::CreateGDT()
			{
				if (SYSTEM_INFO::GlobalGDT.GDT_Entries != nullptr) {
					// GDT already created
					return true;
				}
				//as i will be using long mode and paging for protection the GDT will only ever contain 6 entries. it will always fit in one page.
				//i know it is wasteful to allocate a whole page for a structure that wont be larger than like 6*8 bytes but it is easier to keep track of when i later enable paging.

				GDT_ENTRY* first = reinterpret_cast<GDT_ENTRY*>(PhysicalAllocator::AllocatePage());
				if(!first) {
					// allocation failed
					//xor rdx, rdx
					//lidt 0
					//div rdx, rdx
					return false;
				}
				//the first entry is the null descriptor

				STD::ignore =first[0].SetAcccsesByte(0, 0, 0, 0, 0, 0, 0); 
				STD::ignore =first[0].SetBase(0); 
				STD::ignore =first[0].SetLimit(0);
				STD::ignore =first[0].SetFlags(0, 0, 0); 

				//second is the kernel code segment

				STD::ignore =first[1].SetBase(0); 
				STD::ignore =first[1].SetLimit(0xFFFFF); //doesnt matter in long mode but ill set it to the maximum value
				STD::ignore =first[1].SetAcccsesByte(1, 0, 1, 1, 0, 1, 0); //present, dpl 0, code segment, executable, direction/conforming, read/write, accessed
				STD::ignore =first[1].SetFlags(1, 0, 1); //granularity, db, long mode

				//third is the kernel data segment
				STD::ignore =first[2].SetBase(0);
				STD::ignore =first[2].SetLimit(0xFFFFF); //doesnt matter in long mode but ill set it to the maximum value
				STD::ignore =first[2].SetAcccsesByte(1, 0, 1, 0, 0, 1, 0); //present, dpl 0, data segment, not executable, direction/conforming, read/write, accessed
				STD::ignore =first[2].SetFlags(1, 1, 0); //granularity, db, long mode

				// fourth is the user code segment
				STD::ignore = first[3].SetBase(0);
				STD::ignore = first[3].SetLimit(0xFFFFF); //doesnt matter in long mode but ill set it to the maximum value
				STD::ignore = first[3].SetAcccsesByte(1, 3, 1, 1, 0, 1, 0); //present, dpl 3, code segment, executable, direction/conforming, read/write, accessed
				STD::ignore = first[3].SetFlags(1, 0, 1); //granularity, db, long mode

				// fifth is the user data segment
				STD::ignore = first[4].SetBase(0);
				STD::ignore = first[4].SetLimit(0xFFFFF); //doesnt matter in long mode but ill set it to the maximum value
				STD::ignore = first[4].SetAcccsesByte(1, 3, 1, 0, 0, 1, 0); //present, dpl 3, data segment, not executable, direction/conforming, read/write, accessed
				STD::ignore = first[4].SetFlags(1, 1, 0); //granularity, db, long mode

				// put everything in the packed data
				SYSTEM_INFO::GlobalGDT.GDT_Entries = first; // set the GDT entries pointer to the first entry
				SYSTEM_INFO::GlobalGDT.GDT_Entries_Size = sizeof(GDT_ENTRY) * 6; // size of the GDT entries in bytes
				SYSTEM_INFO::GlobalGDT.GDT_Amount_Pages = 1;//always one 
				SYSTEM_INFO::GlobalGDT.GDT_Entries_Count = 6; // number of GDT entries

				return false;
			}
			bool GDT::LoadGDT(void* gdtPtr)
			{
				if (GDT_Entries == nullptr) {
					// GDT not created
					return false;
				}
				return false;
			}
			bool GDT_ENTRY::SetLimit(unsigned int limit)
			{
				constexpr unsigned int LimitBitCountMask = 0xFFFFF; // mask for the lower 20 bits
				if (limit > LimitBitCountMask) {
					// limit must only have the lower 20 bits set
					return false;
				}
				// set the limit in the packed data
				packedData &= 0xFFFF0000;//clear lowest 15 bits
				unsigned __int64 LowPartofLimit = limit & 0x0000FFFF; //extract the low 15 bits from the limit
				packedData |= LowPartofLimit; // set the lower 15 bits of the limit


				packedData &= 0xFFF0FFFFFFFFFFFF; // clear bits 48-51
				unsigned __int64 highPartofLimit = static_cast<unsigned __int64>((limit & 0xF0000)) << 36; // extract the high 4 bits of the limit and position it correctly
				// set bits 48-51 in the packed data 
				packedData |= highPartofLimit ;

				return true;
			}
			bool GDT_ENTRY::SetBase(unsigned __int32 base)
			{
				unsigned __int64 lowPArtofBase = base & 0xFFFF; // extract the lower 16 bits of the base address
				unsigned __int64 midPartofBase = static_cast<unsigned __int64>((base & 0xFF0000)) << 16; // extract the middle 8 bits of the base address and position it correctly
				unsigned __int64 highPartofBase = static_cast<unsigned __int64>((base & 0xFF000000)) << 32; // extract the high 8 bits of the base address and position it correctly
				unsigned __int64 baseAddress = lowPArtofBase | midPartofBase | highPartofBase; // combine the three parts of the base address
				// clear the base address bits in the packed data
				packedData &= 0xFFFFF00000FFFF;
				// set the base address bits in the packed data
				packedData |= baseAddress;
				return true;
			}
			bool GDT_ENTRY::SetAcccsesByte(bool present, unsigned char dpl, bool segmentType, bool executable, bool directionOrConforming, bool readWrite, bool accessed)
			{
				if(dpl > 3) {
					// DPL must be between 0 and 3
					return false;
				}
				unsigned char accessByte = 0;
				present ? accessByte |= 0x80 : accessByte &= 0x7F; // set the present bit (bit 7)
				accessByte |= (dpl & 0x03) << 5; // set the DPL bits (bits 6 and 5)
				segmentType ? accessByte |= 0x10 : accessByte &= 0xEF; // set the segment type bit (bit 4)
				executable ? accessByte |= 0x08 : accessByte &= 0xF7; // set the executable bit (bit 3)
				directionOrConforming ? accessByte |= 0x04 : accessByte &= 0xFB; // set the direction/conforming bit (bit 2)
				readWrite ? accessByte |= 0x02 : accessByte &= 0xFD; // set the read/write bit (bit 1)
				accessed ? accessByte |= 0x01 : accessByte &= 0xFE; // set the accessed bit (bit 0)
				// clear the access byte bits in the packed data
				packedData &= 0xFFFF00FFFFFFFFFF;
				// set the access byte bits in the packed data
				packedData |= static_cast<unsigned __int64>(accessByte) << 40; // position the access byte correctly in the packed data
				return true;
			}
			bool GDT_ENTRY::SetFlags(bool granularity, bool db, bool longMode)
			{
				unsigned char flags = 0;
				if (db && longMode) {
					return false;// db and long mode cannot be set at the same time
				}
				granularity ? flags |= 0x08 : flags &= 0xF7; // set the granularity bit (bit 3)
				db ? flags |= 0x04 : flags &= 0xFB; // set the db bit (bit 2)
				longMode ? flags |= 0x02 : flags &= 0xFD; // set the long mode bit (bit 1)
				// clear the flags bits in the packed data
				packedData &= 0xFF0FFFFFFFFFFFFF;
				// set the flags bits in the packed data
				packedData |= static_cast<unsigned __int64>((flags&0xF)) << 52; // position the flags correctly in the packed data

				return true;
			}
		}
	}
}	