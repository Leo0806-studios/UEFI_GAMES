#include <../GAME/HEADER/INPUT/INPUT.h>
#include <GLOBALS.h>


bool GetKey(Keys key,int tries)
{
	EFI_INPUT_KEY Key;
	GlobalST->ConIn->ReadKeyStroke(GlobalST->ConIn, &Key);
	if (Key.ScanCode == key) return true;
	return false;
}
