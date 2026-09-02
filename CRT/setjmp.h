#pragma once
#ifndef __SETJMP__
#define __SETJMP__
#include "CRT_CORE.h"
#include "CRT_MACROS.h"
__CRT_START
typedef struct __FLOAT128 {
	unsigned __int8 Part[16];
} __FLOAT128;
typedef unsigned __int64 __Register;
typedef struct __JUMP_BUFFER {
	__Register Frame;
	__Register RBX;
	__Register RSP;
	__Register RBP;
	__Register RSI;
	__Register RDI;
	__Register R12;
	__Register R13;
	__Register R14;
	__Register R15;
	__Register RIP;
	unsigned long MxCsr;
	unsigned short FpCsr;
	unsigned short Reserved;
	__FLOAT128 Xmm6;
	__FLOAT128 Xmm7;
	__FLOAT128 Xmm8;
	__FLOAT128 Xmm9;
	__FLOAT128 Xmm10;
	__FLOAT128 Xmm11;
	__FLOAT128 Xmm12;
	__FLOAT128 Xmm13;
	__FLOAT128 Xmm14;
	__FLOAT128 Xmm15;
} __JUMP_BUFFER;

typedef unsigned __int8 jmp_buf[16 * 16];
__CRT_API int __CRT_CONV  __setjmp(jmp_buf _Buf);
__CRT_API __declspec(noreturn) void __CRT_CONV __longjmp(jmp_buf _Buf, int _Value);
#define setjmp(buf) __setjmp(buf)
#define longjmp(buf, val) __longjmp(buf, val)


__CRT_END
#endif // !__SETJMP__
