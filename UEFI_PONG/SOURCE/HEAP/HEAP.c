#include <HEAP/HEAP.h>
#include <gnu-efi/inc/efi.h>
#include <GLOBALS.h>
#include <gnu-efi/inc/efilib.h>
#include <string.h>
Heap heap = { 0 };
void* CreatHeap(size_t size)
{
	Print(L"Creating heap of size %d\n", size);
	EFI_PHYSICAL_ADDRESS heapBase = 0;
	EFI_STATUS HepCreateStatus = GlobalST->BootServices->AllocatePages(AllocateAnyPages,EfiLoaderData, size / EFI_PAGE_SIZE, &heapBase);
	if (EFI_ERROR(HepCreateStatus)) {
		Print(L"error code %d\n", HepCreateStatus);
		Print(L"Failed to allocate heap memory\n");
		// Handle allocation failure
		return NULLPTR;
	}
	HeapNode* BaseNode = (HeapNode*)heapBase;
	BaseNode->size = size;
	BaseNode->next = NULLPTR;
	BaseNode->prev = NULLPTR;
	BaseNode->isFree = TRUE;
	heap.head = BaseNode;
	heap.tail = BaseNode;
	heap.totalSize = size;
	heap.usedSize = 0;
	Print(L"Heap created at %p with size %d\n", BaseNode, size);
	return BaseNode;
}
/// <summary>
/// splits the node passed in into two. the first of the two nodes will be the ize passed in.
/// returns a pointer to the first node. updates the prev and next nodes of the original node and the new node.
/// </summary>
/// <param name="Node"></param>
/// <param name="size">the size of the Data</param>
/// <returns></returns>
static HeapNode* SplittHeapNode(HeapNode* Node, size_t size) {
	//Handle Special Cae where only one node Exists. thsi is the case at the first allocation after heap creation.
	if (Node->prev == NULLPTR && Node->next == NULLPTR) {
		HeapNode* NewNodeSecond = ((char*)Node) + size + (sizeof(HeapNode));// + sizeof(HeapNode) to account for the size of the HeapNode itself
		memset(NewNodeSecond, 0, sizeof(HeapNode));
		HeapNode* NewNodeFirst = Node;
		NewNodeFirst->size = size;
		NewNodeFirst->next = NewNodeSecond;
		NewNodeFirst->isFree = TRUE;// actual setting will be done in alloc
		NewNodeFirst->data = NULLPTR;


		NewNodeSecond->size = Node->size - size - sizeof(HeapNode);
		NewNodeSecond->next = NULLPTR;//in this special cas this will allways be the last node
		NewNodeSecond->prev = NewNodeFirst;
		NewNodeSecond->isFree = TRUE;
		NewNodeSecond->data = NULLPTR;
		//update heap tail
		heap.tail = NewNodeSecond;
		heap.head = NewNodeFirst;//mithgt be uneccesary but just in case
		return NewNodeFirst;
	}

	HeapNode* NewNodeFirst = Node;
	NewNodeFirst->size = size;

	//get the split point and asign it to the new node
	HeapNode* NewNodeSecond = ((char*)Node) + size + sizeof(HeapNode);// + sizeof(HeapNode) to account for the size of the HeapNode itself
	NewNodeSecond->size = Node->size - size - sizeof(HeapNode);
	NewNodeSecond->prev = NewNodeFirst;
	if (NewNodeFirst->next == NULLPTR) {
		// this is the last node in the heap
		NewNodeSecond->next = NULLPTR;
		heap.tail = NewNodeSecond; // update the tail of the heap
	}
	else {
		NewNodeSecond->next = NewNodeFirst->next;
	}
	NewNodeFirst->next = NewNodeSecond;
	return NewNodeFirst;
}

static HeapNode* MergeNodes(HeapNode* Node1, HeapNode* Node2) {
	// merge two nodes into one
	Node1->size += Node2->size + sizeof(HeapNode); // add the size of the second node and the size of the HeapNode itself
	if (Node2->next == NULLPTR) {
				// this is the last node in the heap
		heap.tail = Node1; // update the tail of the heap
		Node1->next = NULLPTR; // set the next pointer to NULL
	}
	else{
		Node1->next = Node2->next;
	}
	//no need tho check if node1.prev is nullptr as its data will just be kept as it is

	Node1->isFree = TRUE; // mark the merged node as free
	return Node1;
}
void* Alloc(size_t size)
{
	HeapNode* HeapBase = heap.head;
	if (HeapBase == NULLPTR) {
		Print(L"Heap not initialized\n");
		return NULLPTR;
	}
	if( size == 0) {
		Print(L"Cannot allocate 0 bytes\n");
		return NULLPTR;
	}
	if (size > heap.totalSize - heap.usedSize) {
		Print(L"Not enough memory to allocate %d bytes\n", size);
		return NULLPTR;
	}
	// now walk through the heap to find a free node
	HeapNode* current = HeapBase;
	while (TRUE)
	{
		if(current->isFree&&current->size>= size+sizeof(HeapNode)) {
			// we found a free node that is big enough to hold the data
			if(current->size == size + sizeof(HeapNode)) {
				// the node is exactly the size we need, so we can just use it
				current->isFree = FALSE;
				current->data = ((char*)current) + sizeof(HeapNode); // point to the data area
				heap.usedSize += size + sizeof(HeapNode);
				return current->data;
			}
			else {
								// the node is bigger than we need, so we need to split it
				HeapNode* NewNode =SplittHeapNode(current, size);
				NewNode->isFree = FALSE;
				NewNode->data = ((char*)NewNode) + sizeof(HeapNode); // point to the data area
				heap.usedSize += size + sizeof(HeapNode);
				return NewNode->data;

			}

		}
		if(current->next == NULLPTR) {
			// we reached the end of the heap and did not find a free node
			Print(L"No free node found in heap\n");
			return NULLPTR;
		}
		current = current->next;
	}
}

void DeAlloc(void* ptr)
{
	//check if the pointer is NULLPTR or if it is a valid pointer in the heap
	if(ptr == NULLPTR) {
		Print(L"Cannot deallocate NULL pointer\n. the minimum valid addres is sizeof(HeapNode) %d",heap.head);
		return;
	}
	if(((char*)ptr) < ((char*)heap.head) + sizeof(HeapNode)) {
		Print(L"Pointer is not valid\n");
		return;
	}


	volatile HeapNode* node = (HeapNode*)(((char*)ptr) - sizeof(HeapNode));

	//check if the prev or next node are free and merge them if so
	if (node->prev != NULLPTR && ((HeapNode*)node->prev)->isFree) {
		// merge with previous node
		node = MergeNodes(node->prev, node);
	}
	if (node->next != NULLPTR && ((HeapNode*)node->next)->isFree) {
		// merge with next node
		node = MergeNodes(node, node->next);
	}


}
