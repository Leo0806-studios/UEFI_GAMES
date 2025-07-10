#pragma once
#pragma warning (push,0)
#include <stddef.h>
#pragma warning(pop)
struct HeapNode;
typedef struct {
	void* next; 
	void* prev;
	size_t size; 
	_Bool isFree;
	void* data; // Pointer to the actual data block
}HeapNode;

typedef struct {
	HeapNode* head; // Pointer to the first node in the heap
	HeapNode* tail; // Pointer to the last node in the heap
	size_t totalSize; // Total size of the heap
	size_t usedSize; // Size of used memory in the heap
}Heap;
extern Heap heap; // Global heap variable

/// <summary>
/// Allocates the Heap using the uefi memory services.
/// heap functionality is only available after this function is called.
/// </summary>
/// <param name="size"></param>
/// <returns></returns>
void* CreatHeap(size_t size);
/// <summary>
/// Allocates a block of memory of the specified size.
/// </summary>
/// <param name="size">The number of bytes to allocate.</param>
/// <returns>A pointer to the beginning of the allocated memory block, or nullptr if the allocation fails.</returns>
void* Alloc(size_t size); // Allocates a block of memory from the heap
/// <summary>
/// Frees a block of memory previously allocated by Alloc.
/// </summary>
/// <param name="ptr"></param>
void DeAlloc(void* ptr); // Frees a block of memory in the heap

/// <summary
///  Reallocates a block of memory to a new size.
/// </summary> 
/// <param name="ptr"></param
/// <param name="newSize"></param>
/// <returns></returns>
void* ReAlloc(void* ptr, size_t newSize);
/// <summary>
/// allocates a block of memory and initializes it to zero.
/// </summary>
/// <param name="size"></param>
/// <returns></returns>
void* ZeroAlloc(size_t size); 
/// <summary>
/// Prints the current state of the heap, including allocated and free blocks.
/// </summary>
/// <param name=""></param>
void PrintHeap(void);