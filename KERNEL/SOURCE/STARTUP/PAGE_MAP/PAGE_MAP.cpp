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
					EFI_MEMORY_DESCRIPTOR* memoryMap = reinterpret_cast<EFI_MEMORY_DESCRIPTOR*>(UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_);
					ram = memoryMap->NumberOfPages * EFI_PAGE_SIZE;
					size_t descriptorSize = UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_SIZE;
					size_t conventionlaRAM = 0;
					for (UINTN i = 0; i < UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE / descriptorSize; i++) {
						EFI_MEMORY_DESCRIPTOR* Descriptor = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)memoryMap + i * descriptorSize);
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
			bool GlobalPageMap::AllocatePAgeMap()
			{
				size_t Ram = SYSTEM::SYSTEM_INFO::SystemInfo::GetInstance().installedRam;
				if (Ram == 0) {
					return false; // RAM not initialized
				}
				constexpr size_t pageSize = EFI_PAGE_SIZE;
				size_t pageCount = Ram / pageSize;
				size_t pageMapSize = pageCount * sizeof(PageMapEntry);
				//find a region in the uefi memory map large enough to fit the page map
				size_t DescriptorSize = UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_SIZE;
				//size_t memorymapSize = UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE;
				size_t PhysicalStartOfMap = 0;
				size_t PagesUsedByMap = 0;

				EFI_MEMORY_DESCRIPTOR* memoryMap = reinterpret_cast<EFI_MEMORY_DESCRIPTOR*>(UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_);
				//enumerate the map
				for (UINTN i = 0; i < UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE / DescriptorSize; i++) {
					EFI_MEMORY_DESCRIPTOR* Descriptor = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)memoryMap + i * DescriptorSize);
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
				//to make these entrys usefull well enumerate the map again and record allready used regions into the map and recording their type.

				SYSTEM::SYSTEM_INFO::GlobalPageMap.Entrys = reinterpret_cast<PageMapEntry*>(PhysicalStartOfMap);
				SYSTEM::SYSTEM_INFO::GlobalPageMap.sizeBytes = pageMapSize;
				SYSTEM::SYSTEM_INFO::GlobalPageMap.size_pages = PagesUsedByMap;
				SYSTEM::SYSTEM_INFO::GlobalPageMap.etries = pageCount;
				for (size_t i = 0; i < pageCount; i++) {
					SYSTEM::SYSTEM_INFO::GlobalPageMap.Entrys[i].physicalStart = PhysicalStartOfMap + i * pageSize;
					SYSTEM::SYSTEM_INFO::GlobalPageMap.Entrys[i].Type = PageType::Free; // Initialize as free
					SYSTEM::SYSTEM_INFO::GlobalPageMap.Entrys[i].isFree = true;
				}
				//now we will enumerate the map again and record allready used regions into the map and recording their type.
				for (UINTN i = 0; i < UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE / DescriptorSize; i++) {
					EFI_MEMORY_DESCRIPTOR* Descriptor = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)memoryMap + i * DescriptorSize);
					//base stuff necesary for all memory types

					UINT32 DescriptorType = Descriptor->Type;

					void* PhysicalBaase = reinterpret_cast<void*>(Descriptor->PhysicalStart);
					size_t numPages = Descriptor->NumberOfPages;
					size_t FirstPage = reinterpret_cast<uintptr_t>(PhysicalBaase) / pageSize;
					switch (DescriptorType) {
						//first all the Reserved Memory stuuf. theyll all falltrough to the same thing to make it easier

					case EfiReservedMemoryType: { __fallthrough; }
					case EfiRuntimeServicesCode: { __fallthrough; }
					case EfiRuntimeServicesData: { __fallthrough; }
					case EfiUnusableMemory: { __fallthrough; }
					case EfiACPIReclaimMemory: { __fallthrough; }
					case EfiACPIMemoryNVS: { __fallthrough; }
					case EfiPalCode: { __fallthrough; }
								   //all of the reserved memory descriptors falltrough to this
					case EfiPersistentMemory: {
						//ill itterato trough all the pages and set thepaesin the oage table to be reserved
						for (size_t page = FirstPage; page < FirstPage + numPages; page++) {
							PageMapEntry& curr = SYSTEM_INFO::GlobalPageMap.Entrys[page];
							curr.isFree = false;
							//curr.physicalStart=PhysicalBaase //not neceary to reassign the physical base as this is done in the step before
							curr.Type = PageType::EFI_Reserved;// for now i dont differenciate between reserved memory types
						}

					
						break;
					}
											//now il handle the maped io stuff
					case EfiMemoryMappedIO: { __fallthrough; }
					case EfiMemoryMappedIOPortSpace: {
						for (size_t page = FirstPage; page < FirstPage + numPages; page++) {

							PageMapEntry& curr = SYSTEM_INFO::GlobalPageMap.Entrys[page];
							curr.isFree = false;//obviously mmio isnt free
							curr.Type = PageType::MMIO;
						}
						break;
					}
												   //and now ill handle usable memory
					case EfiConventionalMemory: {
						for (size_t page = FirstPage; page < FirstPage + numPages; page++) {

							PageMapEntry& curr = SYSTEM_INFO::GlobalPageMap.Entrys[page];
							curr.isFree = true; //this is free space
							curr.Type = PageType::Free;//and its frre
						}
						break;
					}
					}
				}
				return true;

			}
		}
	}
}
