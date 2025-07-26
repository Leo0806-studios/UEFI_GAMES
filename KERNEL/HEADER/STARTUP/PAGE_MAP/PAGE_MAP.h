#pragma once
using  EFI_MEMORY_DESCRIPTOR__ = void*;
namespace SYSTEM::SUBSYSTEMS::ALLOCATION {
	class PhysicalAllocator;
}
namespace SYSTEM {
	namespace STARTUP {
		/// <summary>
		/// contains everything related to paging. also contains definition for the global page map
		/// </summary>
		namespace PAGING {

			/// <summary>
			/// queries the installed ram. 
			///first run is slower than subsequent runs.
			/// </summary>
			/// <returns>amount of installed ram in bytes</returns>
			extern size_t QueryInstalledRam();

			/// <summary>
			/// UEFI leftovers are all contained in there. this mainly includes information about the memory map
			/// </summary>
			class UEFI_LEFTOWER {
			public:
				/// <summary>
				/// a void pointer to the EFI_MEMORY_DESCRIPTOR structure.
				/// </summary>
				static EFI_MEMORY_DESCRIPTOR__ EFI_MEMORY_DESCRIPTOR_;
				/// <summary>
				/// the size of the EFI memory map in bytes.
				/// </summary>
				static size_t EFI_MEMORY_MAP_SIZE;
				/// <summary>
				/// the size of the EFI_MEMORY_DESCRIPTOR structure in bytes.
				/// </summary>
				static size_t EFI_MEMORY_DESCRIPTOR_SIZE;
			};
			/// <summary>
			/// types of pages in the global page table.
			/// everything above the specified types is reserved.
			/// don't depend on the order of the types as they may change in the future.
			/// </summary>
			enum class PageType {
				Free = 0,
				/// <summary>
				/// memory mapped IO.
				/// this includes efi_memorymapedIO_port and port_space
				/// </summary>
				MMIO = 1,
				/// <summary>
				/// simple allocated memory.
				/// </summary>
				Allocated = 2,
				/// <summary>
				/// this is for now a catch all for every reserved type in the UEFI memory map translated into the global page table
				/// </summary>
				EFI_Reserved = 3

			};
			/// <summary>
			/// Represents an entry in a page map, containing information about a physical memory region.
			/// </summary>
			class PageMapEntry { //-V730 // not necessary as objects of this type will never  be created directly, but only as part of the global page map.
				friend class GlobalPageMap;
				friend SYSTEM::SUBSYSTEMS::ALLOCATION::PhysicalAllocator;

				size_t physicalStart;
				PageType Type;
				/// <summary>
				/// quick flag to indicate if the page is free. might be removed in the future as the information is already contained in the Type field.
				/// </summary>
				bool isFree = false;

			};
			/// <summary>
			/// structure of the global page map.
			
			/// </summary>
			class GlobalPageMap {
				size_t sizeBytes = 0;
				size_t size_pages = 0;
				size_t etries = 0;
				/// <summary>
				/// an array of PageMapEntry structures representing the page map.
				/// </summary>
				PageMapEntry* Entrys=nullptr;

				friend SYSTEM::SUBSYSTEMS::ALLOCATION::PhysicalAllocator;
			public:
				/// <summary>
				/// allocated the global page map and translates the UEFI memory map into it.
				/// every that is not EfiConventionalMemory is considered reserved.
				/// all memory-mapped IO is considered MMIO.
				/// </summary>
				/// <returns>success is true</returns>
				static bool AllocatePageMap();

				

			};
		}
	}
}