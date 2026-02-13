#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
	/// <summary>
	/// performs the necesary initialisation of the bootloader
	/// such as
	/// clearing the screen
	/// printing startup info
	/// gathering system information
	/// locating all neccesary protocols and populating global variables with them
	/// also populates the BOOT_DATA_INTERFACE (Except for the CMD args)
	/// and other neccesary services for the boot enviroment and boot drivers (if applicable)
	/// </summary>
	/// <param name="ImageHandle"></param>
	/// <param name="SystemTable"></param>
	EFI_STATUS InitBootloader(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);



#ifdef __cplusplus
}
#endif // __cplusplus