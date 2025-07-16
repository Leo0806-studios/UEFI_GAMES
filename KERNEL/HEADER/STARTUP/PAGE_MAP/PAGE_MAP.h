#pragma once
using  EFI_MEMORY_DESCRIPTOR__=void*;

namespace SYSTEM {
	namespace STARTUP {
		namespace PAGING {
			extern size_t QueryInstalledRam();
				class UEFI_LEFTOWER {
				public:
static 					EFI_MEMORY_DESCRIPTOR__ EFI_MEMORY_DESCRIPTOR_;
static size_t EFI_MEMORY_MAP_SIZE;
static size_t EFI_MEMORY_DESCRIPTOR_SIZE;
				};
				enum class PageType {
					Free =0,
					MMIO=1,
					Allocated=2,
					EFI_Reserved=3

				};
			class PageMapEntry {
				friend class GlobalPageMap;
				size_t physicalStart;
				PageType Type;
				bool isFree = false;

			};
			class GlobalPageMap {
				size_t sizeBytes = 0;
				size_t size_pages = 0;
				size_t etries = 0;
				PageMapEntry* Entrys;
			public:
				static bool AllocatePAgeMap();
				
			};
		}
	}
}