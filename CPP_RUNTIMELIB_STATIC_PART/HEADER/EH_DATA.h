//###########################################
//				[FILENAME] 
//	Copyright (C) Leo0806 2025
//	Created [DATE] (if known)
//	Created by Leo0806
//	Last modified [DATE]
//	This file is part of [PROJECT_NAME]
//	[DESCRIPTION/SUMMARY]
//############################################


#pragma message(__FILE__ " never include this with <utility> direct or indriect as it will triger odr violations.")//NOLINT
#ifdef _UTILITY_
#error "cant be included wiht <utility> as this contains a smal stub and would trigger odr violations"
#endif // _UTILITY_
#define _UTILITY_
#ifdef __clang__
struct _ThrowInfo;//dummy forward declaration to make clangtidy happy
#endif // __clang__

#include "STRING.h"
#include <ehdata_forceinclude.h>//NOLINT(llvmlibc-restrict-system-libc-headers)
#include <intrin.h>//NOLINT(llvmlibc-restrict-system-libc-headers)
#include <stdint.h>//NOLINT(llvmlibc-restrict-system-libc-headers)

namespace std {
	//minimal type stub so that the using in the next header is satisfied
	 template <class _Ty1, class _Ty2>
	 struct pair {};// store a pair of values

}// namespace std

enum unwind_info_flags : unsigned char { //NOSONAR
	UNW_FLAG_NHANDLER = 0b000,
	UNW_FLAG_EHANDLER = 0b001,
	UNW_FLAG_UHANDLER = 0b010,
	UNW_FLAG_CHAININFO = 0b100
};

using RVA = uint32_t;
using Register = size_t;
 struct _RUNTIME_FUNCTION {//NOSONAR
	RVA BeginAddress;		// The address of the first instruction in the function
	RVA EndAddress;		// The address of the first instruction after the function
	RVA UnwindData;		// The address of the unwind data for this function
} ;


 struct UNWIND_CODE {//NOSONAR
	//NOSONAR
	unsigned char code_offset; // offset in prolog where the operation is located
	unsigned char unwind_op : 4; // the operation to be performed
	unsigned char op_info : 4; // additional information about the operation
} ;
using UBYTE = unsigned char;
namespace FH4 {
	struct FuncInfo4;
}//namespace FH4
#pragma warning(push)
#pragma warning (disable:4820)
/// <summary>
/// this is not an packed representation but an in memory representation as i cant model the VLA in c++
/// </summary>
 struct _UNWIND_INFO { //NOSONAR
	UBYTE version : 3;
	UBYTE flags : 5;
	UBYTE size_of_prolog;
	UBYTE count_of_codes;
	UBYTE frame_register : 4;
	UBYTE frame_offset : 4;
	UNWIND_CODE* unwind_code_array;//inline vla
	union flagDependant {
		RVA exceptionHandler; // RVA to exception handler 
		RVA chainedFunction; // RVA to next function unwind info 
	}flag_dependant;//NOSONAR
	FH4::FuncInfo4* optionalData; // optional data//variable elngth
};


#pragma warning(pop)

 struct RUNTIME_FUNCTION {
	 RVA BeginAddress;
	 RVA EndAddress;
	 RVA UnwindData;  // RVA to .xdata entry
 };
 using PRUNTIME_FUNCTION = RUNTIME_FUNCTION*;
struct CONTEXT64 {
	// minimal fields you'll actually use; or use Windows CONTEXT if you can.
	Register Rip, Rsp, Rbp; //NOSONAR
	Register Rbx, Rsi, Rdi, R12, R13, R14, R15; // non-volatiles //NOSONAR
	// ... XMM non-volatiles if you care, flags, etc.
};


#//pragma warning(pop)
#define _INC_STDLIB
#define _INC_STRING
using BYTE = unsigned char;
#define FAST_FAIL_FATAL_APP_EXIT UINT32_MAX
#include <ehdata4_export.h>//NOLINT(llvmlibc-restrict-system-libc-headers)

#undef _INC_STDLIB 
#undef _INC_STRING