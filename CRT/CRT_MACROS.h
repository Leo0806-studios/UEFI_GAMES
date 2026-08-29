#pragma once
#define __DISABLE_CRT_WARNINGS 4005 4514 4820
#define __CRT_C_BITCAST(to,_val) __builtin_bit_cast(to, _val);
#ifdef  __cplusplus


//dummy to make the crt compile under c++ since restrict is not a thing in c++ and i want to leave it as a hint to the programer
#define restrict
#define __CRT_START extern "C" {
#define __CRT_END }
#else
#define __CRT_START



#if defined(_DLL) && !defined(__CRT_BUILD)
#	define __CRT_API __declspec(dllimport) 
#elif defined(_DLL) && defined(__CRT_BUILD)
#	define __CRT_API __declspec(dllexport)
#elif !defined(_DLL) && defined(__CRT_BUILD)
#	define  __CRT_API 
#else
#	define  __CRT_API 
#endif



#define __CRT_CONV __cdecl
#define __CRT_END

#endif //  __cplusplus