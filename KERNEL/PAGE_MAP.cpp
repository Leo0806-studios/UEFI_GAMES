// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

extern"C" {
#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
}
#include "../../../HEADER/SYTEM_INFO/SYSTEM_INFO.h"
#include "../../../HEADER/STARTUP/PAGE_MAP/PAGE_MAP.h"
namespace SYSTEM {
	namespace STARTUP {
		namespace PAGING {

			EFI_MEMORY_DESCRIPTOR__ UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_ = nullptr;
			size_t UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE = 0;
			size_t UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_SIZE = 0;
			size_t QueryInstalledRam() {
				static size_t ram = 0;
				if (ram == 0) {
				const 	EFI_MEMORY_DESCRIPTOR* memoryMap = static_cast<EFI_MEMORY_DESCRIPTOR*>(UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_); //-V3546
					//ram = memoryMap->NumberOfPages * EFI_PAGE_SIZE;
					const size_t descriptorSize = UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_SIZE;
					size_t conventionlaRAM = 0;
					for (UINTN i = 0; i < UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE / descriptorSize; i++) {
						const EFI_MEMORY_DESCRIPTOR* Descriptor = reinterpret_cast<const EFI_MEMORY_DESCRIPTOR*>(reinterpret_cast<const UINT8*>(memoryMap) + i * descriptorSize); //-V3539
						if (Descriptor->Type == EfiConventionalMemory) {
							//Print(L"Conventional Memory found ");
							conventionlaRAM += Descriptor->NumberOfPages * EFI_PAGE_SIZE;
						}
						//Print(L"Type: %d, PhysicalStart: %lx, NumberOfPages: %lx  ", Descriptor->Type, Descriptor->PhysicalStart, Descriptor->NumberOfPages);
						//SystemTable->BootServices->Stall(1000000); // Stall for 1 second to allow reading the output
						//Print(L"\n");
					}
					ram = conventionlaRAM;
				}
				return ram;
			}
			bool GlobalPageMap::AllocatePageMap()
			{
			const	size_t Ram = SYSTEM::SYSTEM_INFO::SystemInfo::GetInstance().installedRam;
				if (Ram == 0) {
					return false; // RAM not initialized
				}
				constexpr size_t pageSize = EFI_PAGE_SIZE;
				const size_t pageCount = Ram / pageSize;
				const size_t pageMapSize = pageCount * sizeof(PageMapEntry);
				//find a region in the UEFI memory map large enough to fit the page map
				const size_t DescriptorSize = UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_SIZE;
				//size_t memorymapSize = UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE;
				size_t PhysicalStartOfMap = 0;
				size_t PagesUsedByMap = 0;

				const EFI_MEMORY_DESCRIPTOR* memoryMap = static_cast<EFI_MEMORY_DESCRIPTOR*>(UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_);//necessary here as we deal with physical memory //-V3546
				//enumerate the map
				for (UINTN i = 0; i < UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE / DescriptorSize; i++) {
					const EFI_MEMORY_DESCRIPTOR* Descriptor = reinterpret_cast<const EFI_MEMORY_DESCRIPTOR*>(reinterpret_cast<const UINT8*>(memoryMap) + i * DescriptorSize); //-V3539
					if (Descriptor->Type == EfiConventionalMemory && Descriptor->NumberOfPages * EFI_PAGE_SIZE >= pageMapSize) {
						PhysicalStartOfMap = Descriptor->PhysicalStart;
						PagesUsedByMap = pageMapSize / pageSize;
						break;
					}
				}
				if (PhysicalStartOfMap == 0 || PagesUsedByMap == 0) {
					return false; // No suitable memory region found
				}
				//now that we have a location for the map we can start recording the physical pages into it
				//this involves treating the found region as an array of PageMapEntry 
				//to make these entries useful well enumerate the map again and record already used regions into the map and recording their type.

				SYSTEM::SYSTEM_INFO::GlobalPageMap.Entrys = reinterpret_cast<PageMapEntry*>(PhysicalStartOfMap); //-V3546
				SYSTEM::SYSTEM_INFO::GlobalPageMap.sizeBytes = pageMapSize;
				SYSTEM::SYSTEM_INFO::GlobalPageMap.size_pages = PagesUsedByMap;
				SYSTEM::SYSTEM_INFO::GlobalPageMap.etries = pageCount;
				for (size_t i = 0; i < pageCount; i++) {
					SYSTEM::SYSTEM_INFO::GlobalPageMap.Entrys[i].physicalStart = PhysicalStartOfMap + i * pageSize; //-V3539
					SYSTEM::SYSTEM_INFO::GlobalPageMap.Entrys[i].Type = PageType::Free; // Initialize as free //-V3539
					SYSTEM::SYSTEM_INFO::GlobalPageMap.Entrys[i].isFree = true; //-V3539
				}
				//now we will enumerate the map again and record already used regions into the map and recording their type.
				for (UINTN i = 0; i < UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE / DescriptorSize; i++) {
					const EFI_MEMORY_DESCRIPTOR* Descriptor = reinterpret_cast<const EFI_MEMORY_DESCRIPTOR*>(reinterpret_cast<const UINT8*>(memoryMap) + i * DescriptorSize);  //-V3539
					//base stuff necessary for all memory types

					const UINT32 DescriptorType = Descriptor->Type;

					void* PhysicalBaase = reinterpret_cast<void*>(Descriptor->PhysicalStart);
					const size_t numPages = Descriptor->NumberOfPages;
					const size_t FirstPage = reinterpret_cast<uintptr_t>(PhysicalBaase) / pageSize;
					switch (DescriptorType) { //-V2520 for now i falltrough all the reserved memory types to the same case
						//first all the Reserved Memory stuff. they'll all falltrough to the same thing to make it easier

					case EfiReservedMemoryType: { __fallthrough; }
					case EfiRuntimeServicesCode: { __fallthrough; }
					case EfiRuntimeServicesData: { __fallthrough; }
					case EfiUnusableMemory: { __fallthrough; }
					case EfiACPIReclaimMemory: { __fallthrough; }
					case EfiACPIMemoryNVS: { __fallthrough; }
					case EfiPalCode: { __fallthrough; }
								   //all of the reserved memory descriptors falltrough to this
					case EfiPersistentMemory: {
						//ill iterate trough all the pages and set the pages in the page table to be reserved
						for (size_t page = FirstPage; page < FirstPage + numPages; page++) {
							PageMapEntry& curr = SYSTEM_INFO::GlobalPageMap.Entrys[page]; //-V3539
							curr.isFree = false;
							//curr.physicalStart=PhysicalBaase //not necessary to reassign the physical base as this is done in the step before
							curr.Type = PageType::EFI_Reserved;// for now i don't differentiate between reserved memory types
						}


						break;
					}
											//now il handle the mapped IO stuff
					case EfiMemoryMappedIO: { __fallthrough; }
					case EfiMemoryMappedIOPortSpace: {
						for (size_t page = FirstPage; page < FirstPage + numPages; page++) {

							PageMapEntry& curr = SYSTEM_INFO::GlobalPageMap.Entrys[page]; //-V3539
							curr.isFree = false;//obviously MMIO isn't free
							curr.Type = PageType::MMIO;
						}
						break;
					}
												   //and now ill handle usable memory
					case EfiConventionalMemory: {
						for (size_t page = FirstPage; page < FirstPage + numPages; page++) {

							PageMapEntry& curr = SYSTEM_INFO::GlobalPageMap.Entrys[page]; //-V3539
							curr.isFree = true; //this is free space
							curr.Type = PageType::Free;//and its free
						}
						break;
					}
					default: {
						//if the type is not one of the above i will just set it to reserved
						//this is a catch all for every other type of memory that i don't care about
						for (size_t page = FirstPage; page < FirstPage + numPages; page++) {
							PageMapEntry& curr = SYSTEM_INFO::GlobalPageMap.Entrys[page]; //-V3539
							if (curr.isFree) { //only set the type if the page is free
								curr.isFree = false;
								curr.Type = PageType::EFI_Reserved; //for now i don't differentiate between reserved memory types
							}
						}
					}
					}
				}
				return true;

			}
		}
	}
}
