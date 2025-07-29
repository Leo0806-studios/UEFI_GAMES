#pragma once
#include "HEADER/TYPE_TRAITS/TYPE_TRAITS_F.h"
#include "HEADER/STRING/STRING_F.h"
#include "HEADER/UTILLITY/UTILLITY_F.h"
#include "HEADER/INTERNAL/MACROS.h"
namespace STD {
	template <typename T>
	class vector {

		using AllocatorFunc = T * (*)(size_t size);
		using DeallocatorFunc = void (*)(T* ptr, size_t size);
		using GlobalAllocatorFunc = void* (*)(size_t size);
		using GlobalDeallocatorFunc = void (*)(void* ptr, size_t size);


		AllocatorFunc allocator = nullptr;
		DeallocatorFunc deallocator = nullptr;

		/// <summary>
		/// points to the first element in the vector.
		/// </summary>
		T* start = nullptr;
	
		size_t size = 0;

		size_t capacity = 0;

		static inline  GlobalAllocatorFunc globalAllocator(GlobalAllocatorFunc func = nullptr) {
			static GlobalAllocatorFunc globalAllocator = nullptr;
			if (func) {
				globalAllocator = func;
			}
			return globalAllocator;
		}
		static inline  GlobalDeallocatorFunc globalDeallocator(GlobalDeallocatorFunc func = nullptr) {
			static GlobalDeallocatorFunc globalDeallocator = nullptr;
			if (func) {
				globalDeallocator = func;
			}
			return globalDeallocator;

		}

		T* allocate(size_t size) {
			if (allocator) {
				return allocator(size);
			}
			else if (const auto alloc =globalAllocator()) {
				return static_cast<T*>(alloc(size * sizeof(T)));
			}
			else {
				return nullptr;
			}
		}
		bool deallocate(T* ptr, size_t size) {
			if (deallocator) {
				deallocator(ptr, size);
				return true;
			}
			else if (const auto dealloc = globalDeallocator()) {
				dealloc(ptr, size * sizeof(T));
				return true;
			}
			return false;
		}


		/// <summary>
		/// Creates a copy of the elements from another vector and returns a pointer to the newly allocated array.
		/// will always try to use the allocator from the other vector.
		/// </summary>
		/// <param name="other">The vector whose elements are to be copied.</param>
		/// <returns>A pointer to the newly allocated array containing the copied elements, or nullptr if allocation fails.</returns>
		T* copy(const vector& other) {

			T* newStart = other.allocate(other.size);
			if(!newStart) {
				return nullptr; // Allocation failed
			}
			if constexpr (STD::is_trivially_copyable_v<T>) {
				STD::memcpy(this, &other, other.size);
			}
			else {
				for(size_t i = 0; i < (other.size); ++i) {
					newStart[i] = other.start[i];
				}
			}
			return newStart;
		}


		vector(STD::_MoveOut) {
		//just a  noop to make it compatible with my extension for STD::exchange
			//it allows for things like  something(other&& ): vec(STD::exchange(other.vec,STD::move_out)) {} to work
		}
	public:
		static void SetGlobalAllocator(GlobalAllocatorFunc alloc) {
			globalAllocator  (alloc);
		}
		static void SetGlobalDeallocator(GlobalDeallocatorFunc dealloc) {
			globalDeallocator( dealloc);
		}
		void SetAllocator(AllocatorFunc alloc) {
			this->allocator = alloc;
		}
		void SetDeallocator(DeallocatorFunc dealloc) {
			this->deallocator = dealloc;
		}

		vector(AllocatorFunc alloc = nullptr, DeallocatorFunc dealloc = nullptr) 
			: allocator(alloc), deallocator(dealloc) {}
		vector(const vector& other) {
			this->allocator = other.allocator;
			this->deallocator = other.deallocator;
			if (other.size == 0) {
				this->size = 0;
				this->capacity = 0;
				this->start = nullptr; // No elements to copy
				return;
			}
			if (!(this->start = copy(other))) {
				this->size = 0;
				this->capacity = 0;
				this->start = nullptr;
				return; // Allocation failed, handle accordingly
			}

			this->size = other.size;
			this->capacity = other.size;
		}
		vector (vector&& other)  
			: allocator(STD::exchange(other.allocator, nullptr)),
			deallocator(STD::exchange(other.deallocator, nullptr)),
			start(STD::exchange(other.start, nullptr)), 
			size(STD::exchange(other.size,0)),
			capacity(STD::exchange(other.capacity, 0)) {}
		vector& operator=(const vector& other) {
			if(this == &other) {
				return *this; // Self-assignment, do nothing
			}
			if(other.size == 0) {
				if (this->start) {
					if (!deallocate(this->start, this->capacity)) {
						//idk what to do here , deallocation failed
						STD_ASSERT(false, "Deallocation failed in vector assignment.");
						
					}
				}
				this->size = 0;
				this->capacity = 0;
				this->start = nullptr; // No elements to copy
				return *this;
			}
			auto  newStart = copy(other);
			if (!newStart) {
				if (!deallocate(this->start, this->capacity)) {
					//idk what to do here , deallocation failed
					STD_ASSERT(false, "Deallocation failed in vector assignment.");

				}
				this->size = 0;
				this->capacity = 0;
				this->start = nullptr;
				return this; // Allocation failed, handle accordingly
			}
			if (!deallocate(this->start, this->capacity)) {
				//idk what to do here , deallocation failed
				STD_ASSERT(false, "Deallocation failed in vector assignment.");

			}
			this->allocator = other.allocator;
			this->deallocator = other.deallocator;
			this->size = other.size;
			this->capacity = other.size;
			return *this;
		}

		vector& operator=(vector&& other) noexcept {
			if (this == &other) {
				return *this; // Self-assignment, do nothing
			}
			if (this->start) {
				if (!deallocate(this->start, this->capacity)) {
					//idk what to do here , deallocation failed
					STD_ASSERT(false, "Deallocation failed in vector move assignment.");
				}
			}
			this->allocator = STD::exchange(other.allocator, nullptr);
			this->deallocator = STD::exchange(other.deallocator, nullptr);
			this->start = STD::exchange(other.start, nullptr);
			this->size = STD::exchange(other.size, 0);
			this->capacity = STD::exchange(other.capacity, 0);
			return *this;
		}


		~vector() {
			if (this->start) {
				if (!deallocate(this->start, this->capacity)) {
					//idk what to do here , deallocation failed
					STD_ASSERT(false, "Deallocation failed in vector destructor.");
				}
			}
			this->size = 0;
			this->capacity = 0;
			this->start = nullptr; 
		}

		T& operator[](size_t index) {
			if (index >= size) {
				STD_ASSERT(false, "Index out of bounds in vector access.");
			}
			return start[index];
		}
		const T& at(size_t index) const {
			if (index >= size) {
				STD_ASSERT(false, "Index out of bounds in vector access.");
			}
			return start[index];
		}
		/// <summary>
		/// Inserts a value at the specified index in the vector, shifting subsequent elements to the right and resizing the storage if necessary.
		/// </summary>
		/// <param name="value">The value to insert into the vector.</param>
		/// <param name="index">The position at which to insert the value. Must be less than or equal to the current size.</param>
		/// <returns>The index at which the value was inserted. If the index is out of bounds or allocation fails, returns the current size.</returns>
		size_t insert(const T& value,size_t index) {
			if (index > size) {
				STD_ASSERT(false, "Index out of bounds in vector insert.");
				return size; // Invalid index, do nothing
			}
			if (size >= capacity) {
				// Need to allocate more space
				size_t newCapacity = capacity == 0 ? 1 : capacity * 2;
				T* newStart = allocate(newCapacity);
				if (!newStart) {
					STD_ASSERT(false, "Allocation failed in vector insert.");
					return size; // Allocation failed, do nothing
				}
				if (start) {
					for (size_t i = 0; i < size; ++i) {
						if constexpr (STD::is_move_assignable_v<T>) {
						newStart[i] = STD::move(start[i]);
						}
						else if constexpr (STD::is_copy_assignable_v<T>) {
							newStart[i] = start[i]; // Use copy assignment if move is not available
						}
						else {
							STD_ASSERT(false, "Type T is neither move nor copy assignable.");
							deallocate(newStart, newCapacity);
							return size; // Cannot insert, type is not assignable
						}
					}
					deallocate(start, capacity);
				}
				start = newStart;
				capacity = newCapacity;
			}
			// Shift elements to the right
			for (size_t i = size; i > index; --i) {
				if constexpr(STD::is_move_assignable_v<T>) {
					start[i] = STD::move(start[i - 1]);
				}
				else if constexpr(STD::is_copy_assignable_v<T>) {
					start[i] = start[i - 1]; // Use copy assignment if move is not available
				}
				else {
					STD_ASSERT(false, "Type T is neither move nor copy assignable.");
					return size; // Cannot insert, type is not assignable
				}
			}
			if constexpr(STD::is_move_assignable_v<T>) {
				start[index] = STD::move(value);
			}
			else if constexpr (STD::is_copy_assignable_v<T>) {
				start[index] = value; // Use copy assignment if move is not available
			}
			else {
				STD_ASSERT(false, "Type T is neither move nor copy assignable.");
				return size; // Cannot insert, type is not assignable
			}
			++size;
			return index;

		}

		/// <summary>
		/// Appends an element to the end of the container.
		/// </summary>
		/// <param name="value">The element to be added to the end of the container.</param>
		/// <returns>The index at which the element was inserted.</returns>
		size_t push_back(const T& value) {
			return insert(value, size);
		}
		size_t push_front(const T& value) {
			return insert(value, 0);
		}
		bool erase(size_t index) {
			if (index >= size) {
				STD_ASSERT(false, "Index out of bounds in vector erase.");
				return false; // Invalid index, do nothing
			}
			for (size_t i = index; i < size - 1; ++i) {
				start[i] = std::move(start[i + 1]);
			}
			--size;
			return true;
		}
		T* data() {
			return start;
		}
		T* begin() {
			return start;
		}
		T* end() {
			return start + size;
		}
		size_t Size() const {
			return size;
		}


	};
}