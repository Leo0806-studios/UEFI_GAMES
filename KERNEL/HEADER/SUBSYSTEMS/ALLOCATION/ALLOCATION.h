#pragma once
#include "SAL/SAL.h"
namespace SYSTEM {
	namespace SUBSYSTEMS {
		/// <summary>
		/// everything related to memory allocation is inside this namespace
		/// </summary>
		namespace ALLOCATION{
			/// <summary>
			/// header inserted onto a allocation allocated with the physical allocator. is in the beginning of the first page of the allocation.
			/// do not rely on its internal layout (for byte wise access)
			/// </summary>
			struct PageHeader {
				/// <summary>
				/// default to zero for error catching
				/// </summary>
				size_t amoutofPages = 0;
				/// <summary>
				/// pointer to the usable area. this is used for error checking
				/// </summary>
				void* usable = nullptr;

			};
			/// <summary>
			/// handles allocation of physical pages.
			/// for now relatively empty. its just in a class for later if i want to add specific state
			/// </summary>
			class PhysicalAllocator {
				constexpr static size_t PageSize = 4096; // size of a single page
			public:
				/// <summary>
				/// allocates a single page
				/// faster than AllocatePages for a single page as it does not have to do size checking
				/// returns a pointer to the first valid address inside the page.
				/// the offset from the beginning of the page is not stable and might change based on the format and size of the header information. don't rely on it
				/// </summary>
				/// <returns></returns>
				[[nodiscard("discarding the address of the allocated page will lead to a memory leak as it cant be freed anymore")]] 
				void* AllocatePAge();
				/// <summary>
				/// frees a single page. faster than FreePages as it doesn't have to do bounds checking.
				/// performs a check if the pointer is a valid page pinter. (at a offset that corresponds to the sizeof the header and some other checks).
				/// if the pointer is invalid it will return false. 
				/// nullptr is also invalid
				/// </summary>
				/// <param name="ptr"></param>
				[[nodiscard("discarding the return of this function can lead to leaks or undetected memory corruption")]]
				bool FreePage(void* ptr);


				/// <summary>
				/// Allocates a number of pages.
				/// returns a pointer to the first valid address inside the first page.
				/// the offset from the beginning of the page is not stable and might change based on the format and size of the header information. don't rely on it
				/// </summary>
				/// <param name="amountOfPages"></param>
				/// <returns></returns>
				NODISCARD_MSG("discarding the address of the allocated pages will lead to a memory leak as it cant be freed anymore")
				void* AllocatePages(size_t amountOfPages);

				/// <summary>
				/// Frees memory pages previously allocated and returns whether the operation was successful.
				/// a return of false can also indicate memory corruption or an invalid pointer.
				/// </summary>
				/// <param name="ptr">Pointer to the memory pages to be freed.</param>
				/// <returns>true if the pages were successfully freed; false otherwise.</returns>
				NODISCARD_MSG("discarding the return of this function can lead to leaks or undetected memory corruption")
				bool FreePages(void* ptr);
			};
		}
	}
}