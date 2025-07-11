extern"C" {

#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
EFI_STATUS _KERNEL_MAIN(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable);

	Print(L"Hello, World!\n");
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	return EFI_SUCCESS;

}
}