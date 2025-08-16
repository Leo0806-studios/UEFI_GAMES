#include "CPPRUNTIME.h"
#include "HEADER/EXCEPTIONS/EXCEPTIONS.h"
void* operator new(size_t size) {

	void* ptr = malloc(size);


#ifndef NO_NEW_THROW

	if (!ptr) {
		throw STD::bad_alloc();
	}
#endif // NO_NEW_THROW

	return ptr;
}