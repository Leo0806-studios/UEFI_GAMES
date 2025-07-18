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
			///first runn is slower than subsequent runs.
			/// </summary>
			/// <returns>amount of installed ram in bytes</returns>
			extern size_t QueryInstalledRam();

			/// <summary>
			/// uefi leftovers are all contained in there. thismainly includes infomation about the memory map
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
			/// dont depend on the order of the types as they may change in the future.
			/// </summary>
			enum class PageType {
				Free = 0,
				/// <summary>
				/// memory mapped io.
				/// this includes efi_memorymapedIO_port and port_space
				/// </summary>
				MMIO = 1,
				/// <summary>
				/// simple allocated memory.
				/// </summary>
				Allocated = 2,
				/// <summary>
				/// this is for naow a catch all for every reserved tpye in the uefi memory map translated into the global page table
				/// </summary>
				EFI_Reserved = 3

			};
			/// <summary>
			/// Represents an entry in a page map, containing information about a physical memory region.
			/// </summary>
			class PageMapEntry {
				friend class GlobalPageMap;
				friend SYSTEM::SUBSYSTEMS::ALLOCATION::PhysicalAllocator;

				size_t physicalStart;
				PageType Type;
				/// <summary>
				/// quick flag to indicate if the page is free. migth be removed in the future as the information is already contained in the Type field.
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
				PageMapEntry* Entrys;

				friend SYSTEM::SUBSYSTEMS::ALLOCATION::PhysicalAllocator;
			public:
				/// <summary>
				/// allocated the global pagemap and translates the uefi memory map into it.
				/// every that is not EfiConventionalMemory is considered reserved.
				/// all memorymaped io is considered MMIO.
				/// </summary>
				/// <returns>sucsess is true</returns>
				static bool AllocatePAgeMap();

				

			};
		}
	}
}