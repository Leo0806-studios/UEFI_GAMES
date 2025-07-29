#pragma once


#define NODISCARD [[nodiscard]]
#define NODISCARD_MSG(msg) [[nodiscard(msg)]]
#define FALLTHROUGH [[fallthrough]]
#define MAYBE_UNUSED [[maybe_unused]]
#define LIKELY [[likely]]
#define UNLIKELY [[unlikely]]
#define NO_RETURN [[noreturn]]
#define DEPRECATED [[deprecated]]
#define DEPRECATED_MSG(msg) [[deprecated(msg)]]

/**
* a pointer variable declared with this claims full ownership of the object it points to.
* a assignment on declaration signifies a transfer of ownership.
* can also be applied to a function parameter to indicate that the function takes ownership of the pointer.
* type: the type of the object pointed to.
* name: the name of the pointer variable.
*/
#define CLAIMS(type,name) type * const name
/**
* used to mark a return of a function as transfer of ownership.
* the pointer returned is owning
* the pointer is expected to be freed by the caller.
* type: the type of the object pointed to.
*/
#define RELEASE(type) [[nodiscard]] type * const


/**
* 
* a pointer variable declared with this claims no ownership of the object it points to.
* the pointer is expected to be used only for reading the object.
* can also be applied to a function parameter to indicate that the function does not take ownership of the pointer.
* type: the type of the object pointed to.
* name: the name of the pointer variable.
* This is used to indicate that the pointer is read-only and should not be modified.
*/
#define READONLY(type,name) const type * const name
