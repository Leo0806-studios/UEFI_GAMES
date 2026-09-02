#include "CRT_STARTUP.h"
#include "string.h"
#include "assert.h"

 void __cdecl __wassertImpl(wchar_t const* _Message, wchar_t const* _File, unsigned _Line)
{
	(void)_Message;
	(void)_File;
	(void)_Line;

	
}