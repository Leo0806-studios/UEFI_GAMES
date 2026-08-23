#pragma once
#ifndef __THREADINGSHAREDSYMBOLS__
#define __THREADINGSHAREDSYMBOLS__
typedef struct call_once_flag {
	unsigned char flag;//fuck the winapi design. ill do it with interlock exchange and a single byte. no need for any external os support
} once_flag;
#define ONCE_FLAG_INIT { 0 }
void call_once(once_flag* flag, void (*func)(void));
#endif // !__THREADINGSHAREDSYMBOLS__
