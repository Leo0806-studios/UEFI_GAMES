#pragma once
using  EFI_MEMORY_DESCRIPTOR__=void*;

namespace SYSTEM {
	namespace STARTUP {
		namespace PAGING {

				class UEFI_LEFTOWER {
				public:
static 					EFI_MEMORY_DESCRIPTOR__ EFI_MEMORY_DESCRIPTOR_;
				};
				enum class PageType {

				};
			class PageMapEntry {
				friend class GlobalPageMap;
				size_t physicalStart;
				PageType Type;
				bool isFree = false;

			};
			class GlobalPageMap {
				size_t size = 0;
				size_t capacity = 0;
				PageMapEntry* Entrys;
			public:

			};
		}
	}
}