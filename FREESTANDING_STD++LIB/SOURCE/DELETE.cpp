#include <HEADER/CPPRUNTIME.h>
#include "HEADER/EXCEPTIONS/EXCEPTIONS.h"
void operator delete(void* ptr) {
#ifdef _DEBUG
	_free_dbg(ptr);
#else
	free(ptr);
#endif // _DEBUG
}