#pragma once
#define __DISABLE_CRT_WARNINGS 4005 4514
#ifdef  __cplusplus


//dummy to make the crt compile under c++ since restrict is not a thing in c++ and i want to leave it as a hint to the programer
#define restrict
#define __CRT_START extern "C" {
#define __CRT_END }
#else
#define __CRT_START
#define __CRT_END
#endif //  __cplusplus