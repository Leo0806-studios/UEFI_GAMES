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
		//itterate over the map and find asingle free page.
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
		if (!ptr) return false;
		void* fullpage = reinterpret_cast<void*>(reinterpret_cast<size_t>(ptr) - sizeof(PageHeader));

		PageHeader* header = reinterpret_cast<PageHeader*>(fullpage);
		if (header->usable != ptr)return false; //not a valid pointer
		size_t pagenum = reinterpret_cast<size_t>(fullpage) / EFI_PAGE_SIZE;

		auto& globalMap = SYSTEM::SYSTEM_INFO::GlobalPageMap;
		if (pagenum > globalMap.etries)return false;// somehow the adress is larger than max installed ram
		auto& Entry = globalMap.Entrys[pagenum];
		Entry.isFree = true;
		Entry.Type = PageType::Free;
		return true;
	}
}