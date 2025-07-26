// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

extern "C" {
#include <gnu-efi/inc/efi.h>
}
#include "HEADER/SUBSYSTEMS/ALLOCATION/ALLOCATION.h"
#include "HEADER/SYTEM_INFO/SYSTEM_INFO.h"
#include "HEADER/STARTUP/PAGE_MAP/PAGE_MAP.h"
namespace SYSTEM::SUBSYSTEMS::ALLOCATION{
	using PageType = SYSTEM::STARTUP::PAGING::PageType;
	void* PhysicalAllocator::AllocatePAge()
	{
		auto& GlobalPageMap = SYSTEM::SYSTEM_INFO::GlobalPageMap;
		//iterate over the map and find a single free page.
		SYSTEM::STARTUP::PAGING::PageMapEntry* entry = nullptr;
		for (size_t index = 0; index < GlobalPageMap.etries; index++) {
			auto& curr = GlobalPageMap.Entrys[index];
			if (curr.Type == PageType::Free) {
				entry = &curr;
				entry->isFree = false;
				entry->Type = PageType::Allocated;
				break;
			}
		}
		if (!entry) {
			return nullptr; //no free page found
		}
		PageHeader* page = reinterpret_cast<PageHeader*>(entry);
		page->amoutofPages = 1;
		page->usable = entry + sizeof(PageHeader);
		
		return page;;
	}
	bool PhysicalAllocator::FreePage(void* ptr)
	{
		if (!ptr) { return false; }
		void* fullpage = reinterpret_cast<void*>(reinterpret_cast<size_t>(ptr) - sizeof(PageHeader));

		PageHeader* header = reinterpret_cast<PageHeader*>(fullpage); //-V3546 necessary here as we deal with physical memory
		if (header->usable != ptr) { return false; } //not a valid pointer
		size_t pagenum = reinterpret_cast<size_t>(fullpage) / EFI_PAGE_SIZE;

		auto& globalMap = SYSTEM::SYSTEM_INFO::GlobalPageMap;
		if (pagenum > globalMap.etries) { return false; }// somehow the address is larger than max installed ram
		auto& Entry = globalMap.Entrys[pagenum]; //-V3539
		Entry.isFree = true;
		Entry.Type = PageType::Free;
		return true;
	}
	void* PhysicalAllocator::AllocatePages(size_t amountOfPages)
	{

		if (amountOfPages == 0) { return nullptr; }//no pages requested
		if (amountOfPages > (SYSTEM_INFO::SystemInfo::GetInstance().installedRam / PageSize) / 2) { return nullptr; }//i wont allow allocating more than half of the installed ram in one go.
		size_t pagesFound = 0;
		void* firstPage = nullptr;
		auto& GlobalPageMap = SYSTEM::SYSTEM_INFO::GlobalPageMap;
		for (size_t index = 0; index < GlobalPageMap.etries; index++) {
			auto& curr = GlobalPageMap.Entrys[index];
			if (curr.Type == PageType::Free) {
				if (pagesFound == 0) {
					firstPage = &curr;
				}
				pagesFound++;
				curr.isFree = false;
				curr.Type = PageType::Allocated;
			}
			else {
				pagesFound = 0; //reset if we hit a non free page
				firstPage = nullptr;
			}
			if (pagesFound == amountOfPages) {
				break; //we found enough pages
			}
		}
		if (pagesFound < amountOfPages) {
			return nullptr; //not enough pages found
		}
		PageHeader* header = reinterpret_cast<PageHeader*>(firstPage); //-V3546
		header->amoutofPages = amountOfPages; //-V522 //this allowed as this is a pre OS environment and nullptr might actually be a valid address here
		header->usable = reinterpret_cast<void*>(reinterpret_cast<size_t>(firstPage) + sizeof(PageHeader));
		//set the usable area to the first page after the header
		for (size_t i = 0; i < amountOfPages; i++) {
			auto& curr = GlobalPageMap.Entrys[(reinterpret_cast<size_t>(firstPage) / PageSize) + i]; //-V3539
			curr.isFree = false;
			curr.Type = PageType::Allocated;
		}
		return header->usable; //return the usable area
	}
	bool PhysicalAllocator::FreePages(void* ptr)
	{
		if (!ptr) { return false; }//null pointer is not valid
		void* fullpage = reinterpret_cast<void*>(reinterpret_cast<size_t>(ptr) - sizeof(PageHeader));
		PageHeader* header = reinterpret_cast<PageHeader*>(fullpage); //-V3546 necessary here as we deal with physical memory
		if (header->usable != ptr) { return false; } //not a valid pointer
		size_t pagenum = reinterpret_cast<size_t>(fullpage) / EFI_PAGE_SIZE;
		auto& globalMap = SYSTEM::SYSTEM_INFO::GlobalPageMap;
		if (pagenum + header->amoutofPages > globalMap.etries) { return false; } //somehow the address is larger than max installed ram
		for (size_t i = 0; i < header->amoutofPages; i++) {
			auto& Entry = globalMap.Entrys[pagenum + i]; //-V3539
			if (Entry.Type != PageType::Allocated) { return false; } //not a valid page
			Entry.isFree = true;
			Entry.Type = PageType::Free;
		}

		return true;
	}
}