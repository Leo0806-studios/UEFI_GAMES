#pragma once
namespace SYSTEM {
	namespace SUBSYSTEMS {
		/// <summary>
		/// everything related to memory allocation is inside this namespace
		/// </summary>
		namespace ALLOCATION{
			/// <summary>
			/// header inserted onto a allocation allocated with the physical allocator. is in the beginning of the first page of the allocation.
			/// do not rely on its internal layout (for byte wise accses)
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
			/// for now relatively epthy. its just in a class for later if i want to add specific state
			/// </summary>
			class PhysicalAllocator {
			public:
				/// <summary>
				/// allocates a single page
				/// faster thann AllocatePages for a single page as it does not have to do size checking
				/// returns a pointer to the first valid adress inside the page.
				/// the offset from the beginning of the page is not stable and might change based on the format and size of the header information. dont rely on it
				/// </summary>
				/// <returns></returns>
				[[nodiscard("discarding the adress of the allocated page will lead to a memory leak as it cant be freed anymore")]] 
				void* AllocatePAge();
				/// <summary>
				/// frees asingle page. faster than FreePages as it doesnt have to do bounds checking.
				/// performs a check if the pointer is a valid page pinter. (at a offset that coresponnds to the sizeof the header and some other checks).
				/// if the poiner is invalid it will return false. 
				/// nullptr is also invalid
				/// </summary>
				/// <param name="ptr"></param>
				[[nodiscard("discarding the retun of this function can lead to leaks or undetected memory coruption")]]
				bool FreePage(void* ptr);
			};
		}
	}
}