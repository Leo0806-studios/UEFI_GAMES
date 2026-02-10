#pragma once

extern "C" {
	 unsigned long long __stdcall MakeHypercall(unsigned long long ID, void** args,unsigned long long argCount)
}
namespace LLL {
	using HypercallStatus = long long;
	inline HypercallStatus MakeHypercall(unsigned long long ID, void** args, unsigned long long argCount) {
		return ::MakeHypercall(ID,args,argCount);
	}
}