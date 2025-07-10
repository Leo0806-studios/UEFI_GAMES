#pragma once
//this macro marks the pointer as non owning. the function may read or write to the object pointed to but will not deallocate or move it.
// the object is guaranteed to be valid afterwards
// can also be used to mark a variable as non-owning
#define __NON_OWNING(X) X
//this makro marks the parameter as a owning pointer . it represents a transfer of ownership to the passed to function
//the object is guarnateed to be valid afterwards
//can also be used to mark a variable as owning
#define __OWNING(X) X
#define __IN(X) X
//this macro marks a parameter as a out parameter
#define __OUT(X) X
//this macro marks a pointer as the ThisPointer. it signifies that the function will modify the pointer and may possible deallocate it.
//the object is not guaranteed to be valid afterwards
#define __THIS_PTR(X) X
#define __OWNERSHIP_TRANSFER(X) X