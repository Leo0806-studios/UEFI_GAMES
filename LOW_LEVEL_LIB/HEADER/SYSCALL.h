#pragma once
extern "C" long long __stdcall MakeSyscall(unsigned long long syscallNumber,void*args,unsigned long long argCount);

namespace LLL
{
	using SyscallStatus = long long;
	SyscallStatus MakeSyscall(unsigned long long syscallNumber,void*args,unsigned long long argCount){
		return ::MakeSyscall(syscallNumber,args,argCount);
	}
} // namespace LLL
