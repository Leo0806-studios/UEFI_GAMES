#pragma once
#include <stddef.h>
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

//allocates the heap.
//returns a pointer to the HeapBase (the first node in the list)
void* CreatHeap(size_t size);
void* Alloc(size_t size); // Allocates a block of memory from the heap
void DeAlloc(void* ptr); // Frees a block of memory in the heap