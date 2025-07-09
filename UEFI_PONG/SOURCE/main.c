/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2023 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
#include <gnu-efi/inc/libsmbios.h>
#include <string.h>
typedef  _Bool BOOL;
#if defined(_M_X64) || defined(__x86_64__)
static CHAR16* ArchName = L"x86 64-bit";
#elif defined(_M_IX86) || defined(__i386__)
static CHAR16* ArchName = L"x86 32-bit";
#elif defined (_M_ARM64) || defined(__aarch64__)
static CHAR16* ArchName = L"ARM 64-bit";
#elif defined (_M_ARM) || defined(__arm__)
static CHAR16* ArchName = L"ARM 32-bit";
#elif defined (_M_RISCV64) || (defined(__riscv) && (__riscv_xlen == 64))
static CHAR16* ArchName = L"RISC-V 64-bit";
#else
#  error Unsupported architecture
#endif
#define MiB(x) ((UINT64)(x) << 20)
#define GiB(x) ((UINT64)(x) << 30)

unsigned __int64 intPow(unsigned __int64 base, unsigned __int64 exp) {
	if (!exp)return 1;
	unsigned __int64 result = base;
	exp--;
	while (exp) {
		result *= base;
		exp--;
	}
	return result;
}
// Tri-state status for Secure Boot: -1 = Setup, 0 = Disabled, 1 = Enabled
INTN SecureBootStatus = 0;
EFI_SYSTEM_TABLE* GlobalSystemTable = NULL;
/*
 * Query SMBIOS to display some info about the system hardware and UEFI firmware.
 * Also display the current Secure Boot status.
 */
static EFI_STATUS PrintSystemInfo(VOID)
{
	EFI_STATUS Status;
	SMBIOS_STRUCTURE_POINTER Smbios;
	SMBIOS_STRUCTURE_TABLE* SmbiosTable;
	SMBIOS3_STRUCTURE_TABLE* Smbios3Table;
	UINT8 Found = 0, * Raw, * SecureBoot, * SetupMode;
	UINTN MaximumSize, ProcessedSize = 0;

	Print(L"UEFI v%d.%d (%s, 0x%08X)\n", gST->Hdr.Revision >> 16, gST->Hdr.Revision & 0xFFFF,
		gST->FirmwareVendor, gST->FirmwareRevision);

	Status = LibGetSystemConfigurationTable(&SMBIOS3TableGuid, (VOID**)&Smbios3Table);
	if (Status == EFI_SUCCESS) {
		Smbios.Hdr = (SMBIOS_HEADER*)Smbios3Table->TableAddress;
		MaximumSize = (UINTN)Smbios3Table->TableMaximumSize;
	}
	else {
		Status = LibGetSystemConfigurationTable(&SMBIOSTableGuid, (VOID**)&SmbiosTable);
		if (EFI_ERROR(Status))
			return EFI_NOT_FOUND;
		Smbios.Hdr = (SMBIOS_HEADER*)(UINTN)SmbiosTable->TableAddress;
		MaximumSize = (UINTN)SmbiosTable->TableLength;
	}

	while ((Smbios.Hdr->Type != 0x7F) && (Found < 2)) {
		Raw = Smbios.Raw;
		if (Smbios.Hdr->Type == 0) {
			Print(L"%a %a\n", LibGetSmbiosString(&Smbios, Smbios.Type0->Vendor),
				LibGetSmbiosString(&Smbios, Smbios.Type0->BiosVersion));
			Found++;
		}
		if (Smbios.Hdr->Type == 1) {
			Print(L"%a %a\n", LibGetSmbiosString(&Smbios, Smbios.Type1->Manufacturer),
				LibGetSmbiosString(&Smbios, Smbios.Type1->ProductName));
			Found++;
		}
		LibGetSmbiosString(&Smbios, -1);
		ProcessedSize += (UINTN)Smbios.Raw - (UINTN)Raw;
		if (ProcessedSize > MaximumSize) {
			Print(L"%EAborting system report due to noncompliant SMBIOS%N\n");
			return EFI_ABORTED;
		}
	}

	SecureBoot = LibGetVariable(L"SecureBoot", &EfiGlobalVariable);
	SetupMode = LibGetVariable(L"SetupMode", &EfiGlobalVariable);
	SecureBootStatus = ((SecureBoot != NULL) && (*SecureBoot != 0)) ? 1 : 0;
	// You'd expect UEFI platforms to properly clear SetupMode after they
	// installed all the certs... but most of them don't. Hence Secure Boot
	// disabled having precedence over SetupMode. Looking at you OVMF!
	if ((SetupMode != NULL) && (*SetupMode != 0))
		SecureBootStatus *= -1;
	// Wasteful, but we can't highlight "Enabled"/"Setup" from a %s argument...
	if (SecureBootStatus > 0)
		Print(L"Secure Boot status: %HEnabled%N\n");
	else if (SecureBootStatus < 0)
		Print(L"Secure Boot status: %ESetup%N\n");
	else
		Print(L"Secure Boot status: Disabled\n");

	return EFI_SUCCESS;
}

enum Key {
	a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, zero, one, two, three, four, five, six, seven, eight, nine, space, enter, backspace, escape, left, right, up, down
};
BOOL WasKeyPressed() {
	EFI_INPUT_KEY key;
	EFI_STATUS status;

	// Try to read a key without waiting
	status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key);
	if (status == EFI_SUCCESS) {
		return TRUE; // A key was pressed
	}
	else if (status == EFI_NOT_READY) {
		return FALSE; // No key pressed, still waiting
	}
	else {
		Print(L"Error reading key: %r\n", status);
		return FALSE; // An error occurred
	}

}

EFI_INPUT_KEY GetKey() {
	EFI_STATUS status;
	EFI_INPUT_KEY key;
	memset(&key, 0, sizeof(key));
	status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key);
	if (status == EFI_SUCCESS) {
		Print(key.ScanCode);
		Print(L"(%d)", key.UnicodeChar);
		return key; // A key was pressed
	}
	else if (status == EFI_NOT_READY) {
		return key; // No key pressed, still waiting
	}
	else {
		Print(L"Error reading key: %r\n", status);
		return key; // An error occurred
	}
}
// Application entrypoint (must be set to 'efi_main' for gnu-efi crt0 compatibility)
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	GlobalSystemTable = SystemTable;
	UINTN Event;

#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif

	// The platform logo may still be displayed → remove it
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	EFI_STATUS Status;

	/*
	 * In addition to the standard %-based flags, Print() supports the following:
	 *   %N       Set output attribute to normal
	 *   %H       Set output attribute to highlight
	 *   %E       Set output attribute to error
	 *   %r       Human readable version of a status code
	 */
	Print(L"\n%H*** (%s) ***%N\n\n", ArchName);
	//CreateHeap(0b1000000000000000000000000000000000000000000000000000000000000000ULL, GiB(1));//crete heap at half high (addres have highest bit set) with 10 mb
	PrintSystemInfo();
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput = NULL;
	// Locate the Graphics Output Protocol
	Status = uefi_call_wrapper(
		SystemTable->BootServices->LocateProtocol,
		3,
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		(void**)&GraphicsOutput
	);
	if (EFI_ERROR(Status)) {
		Print(L"Failed to locate GOP: %r\n", Status);
		return Status;
	}
	UINT32 Width = GraphicsOutput->Mode->Info->HorizontalResolution;
	UINT32 Height = GraphicsOutput->Mode->Info->VerticalResolution;
	UINT32 PixelsPerScanLine = GraphicsOutput->Mode->Info->PixelsPerScanLine;

	UINT32* FrameBuffer = (UINT32*)GraphicsOutput->Mode->FrameBufferBase;

	// Draw a white pixel at (100, 50)
	UINTN x = 100;
	UINTN y = 50;
	int colour = 0;
	int incr = 0;
	unsigned short counter = 0;
	counter++;
	while (1) {
		if (GetKey().ScanCode) break;;
		incr++;
		counter++;
		if (incr == 100) {
			// Reset the colour every 100 iterations
			incr = 0;


		}
		colour++;
		for (UINTN collumn = 0; collumn < x; collumn++) {
			for (UINTN row = 0; row < y; row++) {
				// Fill the screen with a blue color
				//FrameBuffer[row * PixelsPerScanLine + collumn] = colour;
			}
		}
	}
	for (int i = 0; i < 50; i++) {
		Print(L"mem at (%d) is (%d)\n", i, *(char*)i);
	}

	FrameBuffer[y * PixelsPerScanLine + x] = 0x00FFFFFF;
	Print(L"\n%EPress any key to exit.%N\n");
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
#if defined(_DEBUG)
	// If running in debug mode, use the EFI shut down call to close QEMU
	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
#endif

	return EFI_SUCCESS;
}
