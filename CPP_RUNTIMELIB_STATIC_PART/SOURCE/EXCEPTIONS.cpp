//################Exception support for my freestanding c++ runtie.
// part of the code is taken from 
//#include "ehdata.h"
// and
//#include "ehdata_forceinclude.h"
// 
// 
// 
//
#if defined(_M_CEE_PURE) || defined(BUILDING_C1XX_FORCEINCLUDE)
#define _EH_RELATIVE_TYPEINFO 0
#define _EH_RELATIVE_FUNCINFO 0
#define _RTTI_RELATIVE_TYPEINFO 0
#elif defined(_CHPE_X86_ARM64_EH_)
#define _EH_RELATIVE_TYPEINFO 0
#define _EH_RELATIVE_FUNCINFO 1
#define _RTTI_RELATIVE_TYPEINFO 0
#elif defined(_M_ARM)
#define _EH_RELATIVE_TYPEINFO 1
#define _EH_RELATIVE_FUNCINFO 1
#define _RTTI_RELATIVE_TYPEINFO 0
#elif defined(_M_X64) || defined(_M_ARM64)
#define _EH_RELATIVE_TYPEINFO 1
#define _EH_RELATIVE_FUNCINFO 1
#define _RTTI_RELATIVE_TYPEINFO 1
#else
#define _EH_RELATIVE_TYPEINFO 0
#define _EH_RELATIVE_FUNCINFO 0
#define _RTTI_RELATIVE_TYPEINFO 0
#endif




//
// PMD - Pointer to Member Data: generalized pointer-to-member descriptor
//
typedef struct PMD
{
	int	mdisp;	// Offset of intended data within base
	int	pdisp;	// Displacement to virtual base pointer
	int	vdisp;	// Index within vbTable to offset of base
} PMD;

//
// PMFN - Pointer to Member Function
//
#if _EH_RELATIVE_TYPEINFO
typedef	int	PMFN;					// Image relative offset of Member Function
#else
typedef void(__cdecl* PMFN)(void*);
#endif

typedef void* (__stdcall* PGETWINRT_OOM_EXCEPTION)();

//
// TypeDescriptor - per-type record which uniquely identifies the type.
//
// Each type has a decorated name which uniquely identifies it, and a hash
// value which is computed by the compiler.  The hash function used is not
// important; the only thing which is essential is that it be the same for
// all time.
//
// The special type '...' (ellipsis) is represented by a null name.
//
#pragma warning(push)
#pragma warning(disable:4200)	// nonstandard extension used: array of runtime bound

#if defined(_M_X64) || defined(_M_ARM64) || defined(BUILDING_C1XX_FORCEINCLUDE)
#pragma pack(push, TypeDescriptor, 8)
#endif

typedef struct TypeDescriptor
{
#if defined(_WIN64) || defined(_RTTI) || defined(BUILDING_C1XX_FORCEINCLUDE)
	const void* pVFTable;	// Field overloaded by RTTI
#else
	unsigned long	hash;			// Hash value computed from type's decorated name
#endif
	void* spare;			// reserved, possible for RTTI
	char			name[];			// The decorated name of the type; 0 terminated.
} TypeDescriptor;

#if defined(_M_X64) || defined(_M_ARM64) || defined(BUILDING_C1XX_FORCEINCLUDE)
#pragma pack(pop, TypeDescriptor)
#endif
#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////
//
// Description of the thrown object.
//
// This information is broken down into three levels, to allow for maximum
// comdat folding (at the cost of some extra pointers).
//
// ThrowInfo is the head of the description, and contains information about
// 				the particular variant thrown.
// CatchableTypeArray is an array of pointers to type descriptors.  It will
//				be shared between objects thrown by reference but with varying
//				qualifiers.
// CatchableType is the description of an individual type, and how to effect
//				the conversion from a given type.
//
//---------------------------------------------------------------------------


//
// CatchableType - description of a type that can be caught.
//
// Note:  although isSimpleType can be part of ThrowInfo, it is more
//		  convenient for the run-time to have it here.
//
typedef const struct _s_CatchableType {
	unsigned int		properties;			// Catchable Type properties (Bit field)
#if _EH_RELATIVE_TYPEINFO
	int					pType;				// Image relative offset of TypeDescriptor
#else
	TypeDescriptor* pType;				// Pointer to the type descriptor for this type
#endif
	PMD 				thisDisplacement;	// Pointer to instance of catch type within thrown object.
	int					sizeOrOffset;		// Size of simple-type object or offset into
	//  buffer of 'this' pointer for catch object
	PMFN				copyFunction;		// Copy constructor or CC-closure
} CatchableType;

//
// CatchableTypeArray - array of pointers to catchable types, with length
//
#pragma warning (push)
#pragma warning (disable:4200)	// nonstandard extension used: array of runtime bound
typedef const struct _s_CatchableTypeArray {
	int	nCatchableTypes;
#if _EH_RELATIVE_TYPEINFO
	int				arrayOfCatchableTypes[];	// Image relative offset of Catchable Types
#else
	CatchableType* arrayOfCatchableTypes[];
#endif
} CatchableTypeArray;
#pragma warning (pop)

//
// ThrowInfo - information describing the thrown object, statically built
// at the throw site.
//
// pExceptionObject (the dynamic part of the throw; see below) is always a
// reference, whether or not it is logically one.  If 'isSimpleType' is true,
// it is a reference to the simple type, which is 'size' bytes long.  If
// 'isReference' and 'isSimpleType' are both false, then it's a UDT or
// a pointer to any type (i.e. pExceptionObject points to a pointer).  If it's
// a pointer, copyFunction is NULL, otherwise it is a pointer to a copy
// constructor or copy constructor closure.
//
// The pForwardCompat function pointer is intended to be filled in by future
// versions, so that if say a DLL built with a newer version (say C10) throws,
// and a C9 frame attempts a catch, the frame handler attempting the catch (C9)
// can let the version that knows all the latest stuff do the work.
//
typedef const struct _s_ThrowInfo {
	unsigned int	attributes;							// Throw Info attributes (Bit field)
	PMFN			pmfnUnwind;							// Destructor to call when exception has been handled or aborted
#if _EH_RELATIVE_TYPEINFO && !defined(BUILDING_C1XX_FORCEINCLUDE)
	int				pForwardCompat;						// Image relative offset of Forward compatibility frame handler
	int				pCatchableTypeArray;				// Image relative offset of CatchableTypeArray
#else
	int(__cdecl* pForwardCompat)(...);				// Forward compatibility frame handler
	CatchableTypeArray* pCatchableTypeArray;			// Pointer to list of pointers to types
#endif
} ThrowInfo;



enum unwind_info_flags: unsigned char  {
	UNW_FLAG_EHANDLER,
	UNW_FLAG_UHANDLER,
	UNW_FLAG_CHAININFO
};


typedef struct _s_runtime_function {
	unsigned long BeginAddress;		// The address of the first instruction in the function
	unsigned long EndAddress;		// The address of the first instruction after the function
	unsigned long UnwindData;		// The address of the unwind data for this function
} _RUNTIME_FUNCTION;

using UBYTE = unsigned char;



//typedef struct _s_unwind_info {
//	UBYTE version : 3;
//	UBYTE flags : 5;
//	UBYTE size_of_prolog;	
//	UBYTE count_of_codes;
//	UBYTE frame_register : 4;
//	UBYTE frame_offset : 4;
//	unsigned short* unwind_code_array;
//	union flagDependant {
//		struct exceptionHandler {
//			unsigned long exception_handler;	
//		} exceptionHandler;
//		struct chainedUnwindInfo{
//			unsigned long begin_address;	
//			unsigned long end_address;
//			unsigned long unwind_info_address;	
//			unsigned long long Placeholder;;// this is only there so the size is compatible with the compiler generated format. this field is windows specific and so unused
//		} chainedUnwindInfo;
//	}flag_dependant;
//}_UNWIND_INFO;





extern "C" __declspec(noreturn) void __stdcall _CxxThrowException(
	void* pExceptionObject, _ThrowInfo* pThrowInfo) noexcept(false)
{
	(void)pExceptionObject; // Suppress unused parameter warning
	(void)pThrowInfo; // Suppress unused parameter warning
	ThrowInfo* const throwInfo = reinterpret_cast<ThrowInfo*>(pThrowInfo);
	(void)throwInfo;
}

extern "C" {
	void* __CxxFrameHandler4(void* pExceptionRecord, void* pEstablisherFrame, void* pContextRecord, void* pDispatcherContext) {
	// This function is a placeholder for the C++ exception handling mechanism.
	// It is typically used by the C++ runtime to handle exceptions thrown in the code.
	// The actual implementation would be provided by the C++ standard library or runtime.
	
	// For static linking, this function may not be needed, but it is included here
	// to satisfy the linker requirements for exception handling.
	(void)pExceptionRecord; // Suppress unused parameter warning
	(void)pEstablisherFrame; // Suppress unused parameter warning
	(void)pContextRecord; // Suppress unused parameter warning
	(void)pDispatcherContext; // Suppress unused parameter warning

	return nullptr; // Return nullptr as a placeholder
}
 }