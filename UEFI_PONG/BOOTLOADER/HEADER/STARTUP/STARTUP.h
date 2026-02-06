#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
#include <gnu-efi/inc/libsmbios.h>
	/// <summary>
	/// performs the necesary initialisation of the bootloader
	/// such as
	/// clearing the screen
	/// printing startup info
	/// gathering system information
	/// locating all neccesary protocols and populating global variables with them
	/// startup all AP
	/// and other neccesary services for the boot enviroment and boot drivers (if applicable)
	/// </summary>
	/// <param name="ImageHandle"></param>
	/// <param name="SystemTable"></param>
	EFI_STATUS InitBootloader(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);


	typedef struct {
		EFI_GRAPHICS_OUTPUT_PROTOCOL GOP;
		EFI_MP_PROTOCOL

	} BOOTLOADER_PROTOCOLS;
#ifdef __cplusplus
}
#endif // __cplusplus