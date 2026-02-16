#include <INPUT/INPUT.h>
#include <GLOBALS.h>


bool GetKey(Keys key,int tries)
{
	EFI_INPUT_KEY Key;
	for( ;tries > 0; tries--)
	{
		if (GlobalST->ConIn->ReadKeyStroke(GlobalST->ConIn, &Key) == EFI_SUCCESS)
		{
			if (Key.ScanCode == key) return true;
		}
	}
	GlobalST->ConIn->ReadKeyStroke(GlobalST->ConIn, &Key);
	if (Key.ScanCode == key) return true;
	return false;
}
