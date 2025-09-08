// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

//###########################################
//				EXCEPTIONS.cpp 
//	Copyright (C) Leo0806 2025
//	Created -
//	Created by Leo0806
//	Last modified 04.09.2025
//	This file is part of the cpp runtime
//	implemntation of exception handling for the freestanding c++ runtime
//############################################


//################Exception support for my freestanding c++ runtie.
// explanation to linter disables.
// -NOSTD  : this warnign is disabled because not standart library is availible for the runtime
// -NOCASTWARN : warning disabled because Reinterpret_cast is required for the runtime
//


#include "INIT_RUNTIME.h"
#include "CPPRUNTIME.h"
#include "EH_DATA.h"
#include <intrin.h>//NOLINT(llvmlibc-restrict-system-libc-headers)
#include <stdint.h>//NOLINT
#pragma warning(push)
#pragma warning (disable:26457)

#define _RT__LIKELY [[likely]] //NOLINT
#define _RT__UNLIKELY [[unlikely]]
//NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MEMCOPY(dest,src,size)\
for(size_t i = 0; i < (size); ++i) {\
	(reinterpret_cast<unsigned char*>(dest))[i] = (reinterpret_cast<unsigned const char*>(src))[i];\
}
//NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MEMSET(dest,val,size)\
for(size_t i =0;i<(size);i++){\
(reinterpret_cast<unsigned char*>(dest))[i] =val;\
}










static __forceinline void CaptureThrowSiteContext(CONTEXT64& ctx) {
	void* ret = _ReturnAddress();                  // RIP after the call to _CxxThrowException
	ctx.Rip = reinterpret_cast<uint64_t>(ret);
	// In the caller, the return address is at [RSP], so the caller's RSP after "ret" would be +8
	void** addrOfRet = static_cast<void**>(_AddressOfReturnAddress()); //NOSONAR -NOCASTWARN
	ctx.Rsp = reinterpret_cast<uint64_t>(addrOfRet + 1);   // point past the return address //NOLINT(clang-diagnostic-unsafe-buffer-usage)
	// Rbp is whatever it is; non-volatiles are unknown yet (unwind will reconstruct as needed)
}
/// <summary>
/// looks up a RUNTIME_FUNCTION entry for the given rva in the given table.
///the rva is RIP - ImageBase
/// </summary>
/// <param name="rva"></param>
/// <param name="table"></param>
/// <param name="count"></param>
/// <returns></returns>
static const RUNTIME_FUNCTION* LookupFunctionEntry(unsigned int rva, const RUNTIME_FUNCTION table[], size_t count) {//NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
	size_t lo = 0;
	size_t hi = count;
	while (lo < hi) {
		const size_t mid = (lo + hi) / 2; //NOSONAR 
		const auto& e = table[mid];//NOLINT(clang-diagnostic-unsafe-buffer-usage)
		if (rva < e.BeginAddress) {
			hi = mid;
		}
		else if (rva >= e.EndAddress) {
			lo = mid + 1;
		}
		else {
			return &e; // found
		}
	}
	return nullptr; // leaf function (no unwind info)
}


#pragma section(".xcata", read)
#pragma warning (push)
#pragma warning (disable:26814)
__declspec(allocate(".xcata")) const int __unwinddata_start = 0;

#pragma section(".xdata", read)
#pragma warning (pop)
static _UNWIND_INFO ReadUnwindInfo(_In_ const char* unwindData) {
	if (!AS_BOOL(unwindData)) {
		
		std::terminate();
	}
	(void)__unwinddata_start;
	size_t ofsettinStruct = 0;
	_UNWIND_INFO unwindInfo = {};
	MEMCOPY(&unwindInfo, unwindData, 4);//we can copy the first bytes as they are  of static size n the struct. //NOLINT(clang-diagnostic-unsafe-buffer-usage)
	ofsettinStruct += 4; // Move the offset to the next field
	unwindInfo.unwind_code_array = reinterpret_cast<UNWIND_CODE*>(reinterpret_cast<size_t>(unwindData) + ofsettinStruct);//NOSONAR -NOCASTWARN //NOLINT
	if (unwindInfo.count_of_codes % 2 == 0) {
		ofsettinStruct += unwindInfo.count_of_codes*sizeof(UNWIND_CODE);
	}
	else {
		constexpr unsigned char MaxCodes = 255U;
		if (unwindInfo.count_of_codes ==MaxCodes) {
			std::terminate();
		}
		ofsettinStruct += (static_cast<size_t>(unwindInfo.count_of_codes) + 1U)*sizeof(UNWIND_CODE); // Align to even number
	}
	switch (unwindInfo.flags) {
	case UNW_FLAG_NHANDLER:
	{
		return unwindInfo;
	}
	case UNW_FLAG_EHANDLER: {

		unwindInfo.flag_dependant.exceptionHandler = *reinterpret_cast<RVA*>(reinterpret_cast<size_t>(unwindData) + ofsettinStruct);//NOSONAR -NOCASTWARN
		break;
	}
	case UNW_FLAG_UHANDLER: {
		unwindInfo.flag_dependant.exceptionHandler = *reinterpret_cast<RVA*>(reinterpret_cast<size_t>(unwindData) + ofsettinStruct);//NOSONAR -NOCASTWARN
		break;
	}
	case (UNW_FLAG_EHANDLER ^ UNW_FLAG_UHANDLER): {
		unwindInfo.flag_dependant.exceptionHandler = *reinterpret_cast<RVA*>(reinterpret_cast<size_t>(unwindData) + ofsettinStruct);//NOSONAR -NOCASTWARN
		break;
	}
	case UNW_FLAG_CHAININFO: {
		unwindInfo.flag_dependant.chainedFunction = *reinterpret_cast<RVA*>(reinterpret_cast<size_t>(unwindData) + ofsettinStruct);//NOSONAR -NOCASTWARN
		break;
	}
	_RT__UNLIKELY default: {
		std::terminate();
		
	}
	}
	ofsettinStruct += 4; // Move past the flag dependant field
	if (unwindInfo.flags & UNW_FLAG_EHANDLER || unwindInfo.flags & UNW_FLAG_UHANDLER) {//NOLINT(readability-implicit-bool-conversion)
		unwindInfo.optionalData = reinterpret_cast<FH4::FuncInfo4*>(reinterpret_cast<size_t>(unwindData) + ofsettinStruct);//NOSONAR -NOCASTWARN
	}
	return unwindInfo;
}
/// <summary>
/// phase 1 of the exception process.
/// this function unwinds untill it finds a matching catch handler or the end of the stack.
/// returns false if no handler was found and true if a handler was found.
/// </summary>
/// <param name="ctx"></param>
/// <param name=""></param>
/// <returns></returns>
static bool UnwindSearch(const CONTEXT64& ctx, const ThrowInfo*const throwInfo) {
	(void)ctx;
	(void)throwInfo;
	return false;
}
#ifdef __INTELLISENSE__
struct _ThrowInfo; //this is just to disable the intellisense error it doesnt get the type injected

#endif // __INTELLISENSE__


#pragma warning (push)
#pragma warning(disable:4577)
extern "C" __declspec(noreturn) __declspec(noinline)  void __stdcall _CxxThrowException(
	void* pExceptionObject, _ThrowInfo* pThrowInfo) noexcept(false)
{
	
	(void)pExceptionObject; // Suppress unused parameter warning
	(void)pThrowInfo; // Suppress unused parameter warning
	CONTEXT64 ctx{};
	CaptureThrowSiteContext(ctx);
	const CONTEXT64 ctxCopy = ctx;
	const auto* const throwInfo = reinterpret_cast<ThrowInfo*>(pThrowInfo); //NOSONAR -NOCASTWARN
	size_t count = initParameters.pdata.size / sizeof(RUNTIME_FUNCTION);
	//const void*const rip = _ReturnAddress();
	auto rva = static_cast<RVA>(ctx.Rip - reinterpret_cast<size_t>(initParameters.imageBaseAddress));//NOSONAR -NOCASTWARN
	const RUNTIME_FUNCTION* const table = reinterpret_cast<const RUNTIME_FUNCTION*>(initParameters.pdata.offset + reinterpret_cast<size_t>(initParameters.imageBaseAddress));//NOSONAR -NOCASTWARN
	const RUNTIME_FUNCTION* const found = [&]() {
		const RUNTIME_FUNCTION* found_ = nullptr;
		while (!found_) {
			found_ = LookupFunctionEntry(rva, table, count);
			if (!found_) {
				ctx.Rsp += sizeof(Register);
				ctx.Rip = reinterpret_cast<uint64_t>(*reinterpret_cast<void**>(ctx.Rsp));//NOSONAR -NOCASTWARN
			}
		}
		return found_;

		}();


	const char* const unwindData = reinterpret_cast<const char* const>(found->UnwindData + reinterpret_cast<size_t>(initParameters.imageBaseAddress));//NOSONAR -NOCASTWARN //-V3546 //-V2571
	(void)unwindData;
	(void)throwInfo;






	std::terminate(); // For now, just terminate the program //this is here just to make msvc happy
}
#pragma warning (pop)
extern "C" {
	void* __CxxFrameHandler4(void* pExceptionRecord, void* pEstablisherFrame, void* pContextRecord, void* pDispatcherContext) { //NOLINT(bugprone-easily-swappable-parameters,clang-diagnostic-missing-prototypes)
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
#pragma warning(pop)