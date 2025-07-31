#include "HEADER/STARTUP/GDT/GDT.h"
#include "HEADER/SUBSYSTEMS/ALLOCATION/ALLOCATION.h"
using PhysicalAllocator = SYSTEM::SUBSYSTEMS::ALLOCATION::PhysicalAllocator;
namespace SYSTEM {
	namespace STARTUP {
		namespace GDT {
			bool GDT::CreateGDT()
			{
				if (GDT_Entries != nullptr) {
					// GDT already created
					return true;
				}
				//as i will be using long mode and paging for protection the GDT will only ever contain 6 entries. it will always fit in one page.
				GDT_ENTRY* first = reinterpret_cast<GDT_ENTRY*>(PhysicalAllocator::AllocatePage());


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
				return false;
			}
		}
	}
}	