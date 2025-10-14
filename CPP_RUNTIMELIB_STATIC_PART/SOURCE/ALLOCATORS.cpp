// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "INIT_RUNTIME.h"
#include "CPPRUNTIME.h"
#include "ALLOCATORS.h"
#define MEMSET(dest,val,size)\
for(size_t i = 0; i < size; ++i) {\
	((char*)dest)[i] = (char)val;\
	}
constexpr size_t PAGE_SIZE = 4096; // assuming a page size of 4096 bytes, this can be changed based on the system architecture.
#pragma warning(push)
#pragma warning(disable:4820)
struct HeapNode; // forward declaration of HeapNode to use it in MemoryNode
/// <summary>
/// represents a single memory allocation inside a consecutive  block of pages (can be more than one page).
/// </summary>
struct  MemoryNode {
	MemoryNode* prev = nullptr;
	MemoryNode* next = nullptr;
	HeapNode* parrent = nullptr;
	size_t size = 0; // size of the memory block.
#ifdef _DEBUG
	void* data = nullptr; // pointer to the data block, used for debugging purposes.
#endif
	bool isFree = true; // indicates if the memory block is free or allocated.
};

/// <summary>
// similar to memory node the size is in pages instead of bytes.
/// </summary>
struct HeapNode {
	HeapNode* prev = nullptr; // pointer to the previous heap node.
	HeapNode* next = nullptr; // pointer to the next heap node.
	size_t size = 0; // size of the heap node in pages.
	MemoryNode* first = nullptr; // pointer to the first memory node in the heap node.
	MemoryNode* last = nullptr; // pointer to the last memory node in the heap node.
	size_t freeBytes = 0;
	size_t usedSize = 0; // total size of the used memory in bytes.
};
struct Heap {
	HeapNode* first = nullptr; // pointer to the first heap node.
	HeapNode* last = nullptr; // pointer to the last heap node.
	size_t totalSize = 0; // total size of the heap in pages.
	size_t usedSize = 0; // total size of the used memory in pages.
};
Heap heap; // global heap variable to store the heap state.
/// <summary>
/// splits the node passed in into two. the first of the two nodes will be the size passed in.
/// returns a pointer to the first node. updates the prev and next nodes of the original node and the new node.
/// </summary>
/// <param name="Node"></param>
/// <param name="size">the size of the Data</param>
/// <returns></returns>
static MemoryNode* SplittHeapNode(HeapNode heapNode, MemoryNode* Node, size_t size) {
	//Handle Special case where only one node Exists.
	__assume(Node != nullptr); 
	if ((Node->prev == nullptr) && (Node->next == nullptr)) {
		MemoryNode* NewNodeSecond = reinterpret_cast<MemoryNode*>(reinterpret_cast<char*>(Node) + size + (sizeof(MemoryNode)));// + sizeof(HeapNode) to account for the size of the HeapNode itself
		MEMSET(NewNodeSecond, 0, sizeof(MemoryNode));
		MemoryNode* NewNodeFirst = Node;
		const size_t OldSize = Node->size;
		NewNodeFirst->size = size;
		NewNodeFirst->next = NewNodeSecond;
		NewNodeFirst->isFree = true;// actual setting will be done in alloc
		NewNodeFirst->data = nullptr;
		NewNodeSecond->parrent = NewNodeFirst->parrent; // set the parent of the new node to the parent of the old node

		NewNodeSecond->size = OldSize - size - sizeof(MemoryNode);
		NewNodeSecond->next = nullptr;//in this special case this will always be the last node
		NewNodeSecond->prev = NewNodeFirst;
		NewNodeSecond->isFree = true;
		NewNodeSecond->data = nullptr;

		//update heap tail
		heapNode.last = NewNodeSecond;
		heapNode.first = NewNodeFirst;//might be unnecessary but just in case
		return NewNodeFirst;
	}

	MemoryNode* NewNodeFirst = Node;
	const size_t oldSize = NewNodeFirst->size;

	NewNodeFirst->size = size;
	//get the split point and assign it to the new node
	MemoryNode* NewNodeSecond = reinterpret_cast<MemoryNode*>(reinterpret_cast<char*>(Node) + (sizeof(MemoryNode)) + size);// + sizeof(HeapNode) to account for the size of the HeapNode itself
	NewNodeSecond->size = oldSize - size - sizeof(HeapNode);
	NewNodeSecond->prev = NewNodeFirst;
	if (NewNodeFirst->next == nullptr) {
		// this is the last node in the current heap node
		NewNodeSecond->next = nullptr;
		heapNode.last = NewNodeSecond; // update the tail of the heapnode
	}
	else {
		NewNodeSecond->next = NewNodeFirst->next;
	}
	NewNodeFirst->next = NewNodeSecond;
	NewNodeSecond->isFree = true;

	return NewNodeFirst;
}
static MemoryNode* MergeNodes(HeapNode heapNode, MemoryNode* Node1, MemoryNode* Node2) {
	// merge two nodes into one
	__assume(Node1 != nullptr);
	__assume(Node2 != nullptr);
	Node1->size += Node2->size + sizeof(HeapNode); // add the size of the second node and the size of the HeapNode itself
	if (Node2->next == nullptr) {
		// this is the last node in the heap
		heapNode.last = Node1; // update the tail of the heap
		Node1->next = nullptr; // set the next pointer to NULL
	}
	else {
		Node1->next = Node2->next;
	}
	//no need tho check if node1.prev is nullptr as its data will just be kept as it is

	Node1->isFree = true; // mark the merged node as free
	return Node1;
}
/// <summary>
/// allocates a "size" amount of pages from the os and inserts the created heap node into the heap structure in the end
/// </summary>
/// <param name="size"></param>
/// <returns></returns>
static HeapNode* AllocateFromOS(size_t size) {
	
	HeapNode* const node = reinterpret_cast<HeapNode* const>(initParameters.callbacks.allocatePages(size + 1));//+1 because part of the page is reserved for the heap structure and we need to allocate at leas as much as requested
	if (!node) {
		return nullptr; // if the allocation failed, return nullptr
	}
	MemoryNode* const memNode = reinterpret_cast<MemoryNode* const>(reinterpret_cast<char*>(node) + sizeof(HeapNode));
	size_t diff = reinterpret_cast<size_t>(node) % PAGE_SIZE;//this gives us the offset from thestart of the page to account for a possible page header inserted by the os
	node->freeBytes = (size * PAGE_SIZE) - diff - sizeof(HeapNode); // calculate the free bytes in the heap node
	node->usedSize = 0; // initialize the used size to 0
	node->size = size; // set the size of the heap node in pages
	node->first = memNode; // set the first memory node to the memory node
	node->last = memNode; // set the last memory node to the memory node
	node->prev = heap.last;
	node->next = nullptr;

	memNode->prev = nullptr;
	memNode->next = nullptr;
	memNode->size = (size * PAGE_SIZE) - diff - sizeof(MemoryNode) - sizeof(HeapNode);
	memNode->isFree = true;
	memNode->data = nullptr;
	memNode->parrent = node; // set the parent of the memory node to the heap node
	heap.last = node; // set the last heap node to the new node
	heap.totalSize += size; // increase the total size of the heap
	if (heap.first == nullptr) {
		heap.first = node; // if this is the first heap node, set it as the first node
	}
	else {
		heap.last->next = node; // link the new node to the end of the heap
	}
	return node; // return the new heap node
}
static bool FreeToOS(HeapNode* ptr) {
	HeapNode* next = ptr->next; // get the next node in the heap
	HeapNode* prev = ptr->prev; // get the previous node in the heap
	if (next) {
		next->prev = prev; // link the next node to the previous node
	}
	if (prev) {
		prev->next = next; // link the previous node to the next node
	}

	bool sucsess =initParameters.callbacks.freePages(ptr);
	heap.totalSize -= ptr->size; // decrease the total size of the heap
	return sucsess;
}


_Check_return_ _Ret_maybenull_ _Post_writable_byte_size_(size)

 _declspec(restrict) void* malloc(_In_ size_t size) {
	DebugPrint(L"ALLOCATING MEMORY");
	//walk the HeapNode list and the internal list of memory nodes
#ifdef _DEBUG
	if (!AS_BOOL(heap.first)) {
		DebugPrint(L"HEAP INVALID");
		return nullptr; // heap not initialized
	}
#endif // _DEBUG

	HeapNode* currentHeapNode = heap.first;
	MemoryNode* ptr = nullptr;
	while (currentHeapNode->next) {
		DebugPrint(L"CHECKING HEAP NODE");
		MemoryNode* currentNode = currentHeapNode->first;
		if (!AS_BOOL(currentNode)) {
			DebugPrint(L"NODE INVALID");
		}
		if (currentHeapNode->freeBytes < size) {
			DebugPrint(L"NOT ENOUGH SPACE IN CURRENT HEAP NODE");
			currentHeapNode = currentHeapNode->next;
			continue; // not enough free bytes in this heap node, go to the next one
		}
		DebugPrint(L"AAAAAAAAAAAAAAAAAAAAAAAaa");
		DebugPrint(L"AAAAAAAAAAAAAAAAAAAAAAAaa");
		while (AS_BOOL(currentNode->next))
		{
			DebugPrint(L"CHECKING MEMORY NODE");
			if (currentNode->isFree && currentNode->size >= size) {
				DebugPrint(L"FOUND MEMORY NODE");
				// found a free node with enough size
				ptr = SplittHeapNode(*currentHeapNode, currentNode, size);
				currentHeapNode->freeBytes -= size;
				currentHeapNode->usedSize += size;
				ptr->data = reinterpret_cast<char*>(ptr) + sizeof(MemoryNode); // set the data pointer to the start of the data block
				ptr->isFree = false; // mark the node as allocated

				heap.usedSize += size;
				break; // exit the inner loop
			}
			if (ptr) {
				break; // exit the outer loop if we found a suitable node
			}
			currentNode = currentNode->next; // go to the next node
		}
		DebugPrint(L"BBBBBBBBBBBBBBBBBBBBBBBBBb");
	}
	DebugPrint(L"WALKED HEAP ");
	if (!ptr) {

		HeapNode* newNode = nullptr;
		DebugPrint(L"ALLOCATING FROM OS");
		if (size < PAGE_SIZE / 2) {
			newNode = AllocateFromOS((size / PAGE_SIZE) + 1); //-V1064
		}
		else {
			newNode = AllocateFromOS((size / PAGE_SIZE) + 2);
		}
		if (!newNode) {
			return nullptr; // if the allocation failed, return nullptr
		}
		MemoryNode* currentNode = newNode->first;

		while (currentNode->next)
		{
			if (currentNode->isFree && currentNode->size >= size) {
				// found a free node with enough size
				ptr = SplittHeapNode(*currentHeapNode, currentNode, size);
				currentHeapNode->freeBytes -= size;
				currentHeapNode->usedSize += size;
				ptr->data = reinterpret_cast<char*>(ptr) + sizeof(MemoryNode); // set the data pointer to the start of the data block
				ptr->isFree = false; // mark the node as allocated

				heap.usedSize += size;
				break; // exit the inner loop
			}
			if (ptr) {
				break; // exit the outer loop if we found a suitable node
			}
			currentNode = currentNode->next; // go to the next node
		}
	}
	DebugPrint(L"ALLOCATED MEMORY");
	return ptr ? ptr->data : nullptr; // return the data pointer of the allocated node or nullptr if no suitable node was found
}
void free(void* _Block) {
	if (!_Block) {
		return;
	}
	MemoryNode* node = reinterpret_cast<MemoryNode*>(reinterpret_cast<char*>(_Block) - sizeof(MemoryNode)); // get the memory node from the pointer
	if (!(node->data == _Block)) {
		return; //invalid ptr. returning
	}
	node->isFree = true; // mark the node as free
	node->data = nullptr;
	if (node->next && node->next->isFree) {
		// merge with next node if it is free
		node = MergeNodes(*node->parrent, node, node->next);
	}
	if (node->prev && node->prev->isFree) {
		// merge with previous node if it is free
		node = MergeNodes(*node->parrent, node->prev, node);
	}
	node->parrent->freeBytes += node->size; // increase the free bytes in the heap node
	node->parrent->usedSize -= node->size; // decrease the used size in the heap node
	heap.usedSize -= node->size; // decrease the used size in the heap
}



void _free_dbg(void* ptr)
{
	if (!ptr) {
		return;
	}
#pragma warning(push)
#pragma warning (disable:26430 )
	MemoryNode* node = reinterpret_cast<MemoryNode*>(reinterpret_cast<char*>(ptr) - sizeof(MemoryNode)); // get the memory node from the pointer
#pragma warning(pop)
	if(node == nullptr) {
		return;
	}
	__assume(node != nullptr);
	if (!(node->data == ptr)) {
		//throw STD::Bad_free(); // throw an exception if the pointer is invalid
		return;
	}
	if (AS_BOOL(node->next) && AS_BOOL(node->next->isFree)) {
		// merge with next node if it is free
		node = MergeNodes(*node->parrent, node, node->next);
	}
	if (AS_BOOL(node->prev) && node->prev->isFree) {
		// merge with previous node if it is free
		node = MergeNodes(*node->parrent, node->prev, node);
	}
	node->isFree = true; // mark the node as free
	node->data = nullptr;
	node->parrent->freeBytes += node->size; // increase the free bytes in the heap node
	node->parrent->usedSize -= node->size; // decrease the used size in the heap node
	heap.usedSize -= node->size; // decrease the used size in the heap
}

bool CreateHeap(size_t initSize)
{
	initParameters.callbacks.WriteLine(L"CREATING HEAP");
	HeapNode* node = AllocateFromOS(initSize);
	if (!AS_BOOL(node)) {
		return false; // if the allocation failed, return false
	}
	heap.first = node;
	heap.last = node;
	heap.totalSize = initSize; // set the total size of the heap
	heap.usedSize = 0; // initialize the used size to 0
	initParameters.callbacks.WriteLine(L"HEAP CREATED");
	return true;
}
#pragma warning(pop)