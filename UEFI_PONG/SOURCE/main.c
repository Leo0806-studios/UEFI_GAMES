/*
 * UEFI:SIMPLE - UEFI development made easy
 * Copyright ©️ 2014-2023 Pete Batard <pete@akeo.ie> - Public Domain
 * See COPYING for the full licensing terms.
 */
#pragma warning(push,0)
#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
#include <gnu-efi/inc/libsmbios.h>
#include <string.h>
#pragma warning(pop)

#include <GLOBALS.h>
#include "HEAP/HEAP.h"
#include <intrin.h>
#include <../GAME/HEADER/RENDER/RENDER.h>
#include <../GAME/HEADER/OBJECTS/BALL.h>
#include <../GAME/HEADER/OBJECTS/PLAYER.h>
#include "../GAME/HEADER/INPUT/INPUT.h"
#include "../HEADER/RNG/RNG.h"

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
	SMBIOS_STRUCTURE_POINTER Smbios = { 0 };
	SMBIOS_STRUCTURE_TABLE* SmbiosTable = { 0 };
	SMBIOS3_STRUCTURE_TABLE* Smbios3Table= { 0 };
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

bool cpu_supports_avx() {
	int info[4];
	__cpuid(info, 1);
	// ECX bit 28 indicates AVX support
	return (info[2] & (1 << 28)) != 0;
}
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


// Application entrypoint (must be set to 'efi_main' for gnu-efi crt0 compatibility)
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	GlobalST = SystemTable;
	UINTN Event;

#if defined(_GNU_EFI)
	InitializeLib(ImageHandle, SystemTable);
#endif

	// The platform logo may still be displayed → remove it
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	EFI_STATUS Status = { 0 };








//	EFI_HANDLE* fsHandles;
//	UINTN fsHandleCount;
//	EFI_FILE_IO_INTERFACE* FileSystem;
//	EFI_FILE_HANDLE RootDir;
//
//	EFI_STATUS status = uefi_call_wrapper(BS->LocateHandleBuffer, 5,
//		ByProtocol,
//		&gEfiSimpleFileSystemProtocolGuid,
//		NULL,
//		&fsHandleCount,
//		&fsHandles);
//
//	if (EFI_ERROR(status)) {
//		Print(L"Unable to find any file system handles.\n");
//		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
//		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
//	}
//
//	// Just pick the first one for now (or loop if needed)
//	status = uefi_call_wrapper(BS->HandleProtocol, 3,
//		fsHandles[0],
//		&gEfiSimpleFileSystemProtocolGuid,
//		(void**)&FileSystem);
//
//	if (EFI_ERROR(status)) {
//		Print(L"HandleProtocol failed\n");
//		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
//		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
//	}
//
//	status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &RootDir);
//
//	if (EFI_ERROR(status)) {
//		Print(L"Failed to open volume.\n");
//		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
//		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
//	}



	EFI_LOADED_IMAGE* LoadedImage;

	EFI_STATUS status = uefi_call_wrapper(BS->HandleProtocol, 3,
		ImageHandle,
		&gEfiLoadedImageProtocolGuid,
		(void**)&LoadedImage);

	if (EFI_ERROR(status)) {
		Print(L"Failed to get LoadedImageProtocol\n");
		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	}
	Print(L"Loaded Image Protocol: %p\n", LoadedImage);

	EFI_FILE_IO_INTERFACE* FileSystem;

	status = uefi_call_wrapper(BS->HandleProtocol, 3,
		LoadedImage->DeviceHandle,
		&gEfiSimpleFileSystemProtocolGuid,
		(void**)&FileSystem);

	if (EFI_ERROR(status)) {
		Print(L"Failed to get FileSystem from boot device\n");
		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	}
	Print(L"FileSystem Protocol: %p\n", FileSystem);

	EFI_FILE_HANDLE Root, CurrentDir;

	// Open volume
	status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
	if (EFI_ERROR(status)) {
		Print(L"Failed to open volume\n");
		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	}
	Print(L"Root Directory: %p\n", Root);
	EFI_FILE_HANDLE KernelFile;

	status = uefi_call_wrapper(Root->Open, 5, Root, &CurrentDir,
		L"efi\\boot", EFI_FILE_MODE_READ, 0);
	if (EFI_ERROR(status)) {
		Print(L"Failed to open EFI\\BOOT\n");
		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	}
	Print(L"Current Directory: %p\n", CurrentDir);
	status = uefi_call_wrapper(CurrentDir->Open, 5, CurrentDir, &KernelFile,
		L"KERNEL.exe", EFI_FILE_MODE_READ, 0);
	if (EFI_ERROR(status)) {
		Print(L"KERNEL.exe not found\n");
		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	}
	Print(L"Kernel File: %p\n", KernelFile);
	EFI_DEVICE_PATH* KernelPath;
	KernelPath = FileDevicePath(LoadedImage->DeviceHandle, L"EFI\\BOOT\\KERNEL.exe");

	EFI_HANDLE KernelImage;
	status = uefi_call_wrapper(BS->LoadImage, 6, FALSE, ImageHandle,
		KernelPath, NULL, 0, &KernelImage);

	if (EFI_ERROR(status)) {
		Print(L"Failed to load KERNEL.exe\n");
		SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
		SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	}
	Print(L"Kernel Image Handle: %p\n", KernelImage);
	Print(L"transferring control to KERNEL.exe\n");
	status = uefi_call_wrapper(BS->StartImage, 3, KernelImage, NULL, NULL);
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);

	//EFI_FILE_IO_INTERFACE* FileSystem;
	//EFI_FILE_HANDLE RootFS, KernelFile;

	// Get file system protocol
	//Status =uefi_call_wrapper(BS->HandleProtocol, 3,
	//	ImageHandle,
	//	&gEfiSimpleFileSystemProtocolGuid,
	//	(void**)&FileSystem);
	//if (EFI_ERROR(Status)) {
	//	Print(L"%EError: Could not get file system protocol: %r%N\n", Status);
	//	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	//	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	//}
	//Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &RootFS);
	//if (EFI_ERROR(Status)) {
	//	Print(L"%EError: Could not open root file system: %r%N\n", Status);
	//	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	//	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	//}
	//Status = uefi_call_wrapper(RootFS->Open, 5, RootFS, &KernelFile,
	//	L"KERNEL.exe", EFI_FILE_MODE_READ, 0);
	//if( EFI_ERROR(Status)) {
	//	Print(L"%EError: Could not open kernel file: %r%N\n", Status);
	//	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	//	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	//}
	//EFI_HANDLE KernelImageHandle;
	////EFI_STATUS status;
	//
	//status = uefi_call_wrapper(BS->LoadImage, 6,
	//	FALSE,
	//	ImageHandle,
	//	NULL,         // optional device path
	//	NULL, 0,      // no buffer; we're loading from file
	//	&KernelImageHandle
	//);
	//if (EFI_ERROR(status)) {
	//	Print(L"%EError: Could not load kernel image: %r%N\n", status);
	//	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	//	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);;
	//}
	//status = uefi_call_wrapper(BS->StartImage, 3,
	//	KernelImageHandle, NULL, NULL);
	//
	CreatHeap(MiB(10));

	InitRender();
	InitPhysics();
	InitRNG(80085);
	//test RNG
	Print(L"RNG Test uint64: %d\n" ,GetRandomUint());
	Print(L"RNG Test int: %d\n", GetRandomInt());
	Print(L"RNG Test float: %f\n", GetRandomFloat());
	Print(L"RNG Test double: %f\n", GetRandomDouble());
	Print(L"RNG Test int range: %d\n", GetRandomIntRange(10, 100));
	Print(L"RNG Test float range: %f\n", GetRandomFloatRange(10.0f, 100.0f));
	Print(L"RNG Test double range: %f\n", GetRandomDoubleRange(10.0, 100.0));
	Vector2 a = { 100, 50 };
	DrawPixel(a, 0xff0000);

	Vector2 b = { 500, 1000 };
	DrawLine(a, b, 0xff0000);
	/*
	 * In addition to the standard %-based flags, Print() supports the following:
	 *   %N       Set output attribute to normal
	 *   %H       Set output attribute to highlight
	 *   %E       Set output attribute to error
	 *   %r       Human readable version of a status code
	 */
	Print(L"\n%H*** (%s) ***%N\n\n", ArchName);
	SystemTable->BootServices->AllocatePages(
		AllocateAnyPages, // Allocate any pages
		EfiLoaderData,    // Allocate as loader data
		1,                // Allocate 1 page (4 KiB)
		&Event            // Store the allocated address in Event
	);
	Print(L"Does the CPU support AVX? %s\n", cpu_supports_avx() ? L"Yes" : L"No");
	AVXEnabled = cpu_supports_avx();
	PrintSystemInfo();
	Print(L"Screen Might Flicker a lot. Be warned\n");
	Print(L"Pres up and down to moeve the right player.\n");
	Print(L"press F1 and F2 to move the Left player\n");
	Print(L"Spam Esc to exit (couldnt find a better way for now\n");

	Print(L" press any Key To Start Game");
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);

	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);

	GlobalST->BootServices->Stall(1000000);
	PrintHeap();

	Ball* ball = CreateBall(200, 100, 30, 10, 10);
	PrintHeap();
	if (!ball) {
		Print(L"Ball Creation faliled");
		return -1;
	}
	Print(L" Ball position x %f, y %f \n", ball->position.x, ball->position.y);
	CreatePlayers((Vector2) { 30, 400 });
	Print(L"Players Created\n");
	bool dontExit = true;
	//PrintHeap();
	while (dontExit) {
		if (GetKey(SCAN_ESC, 1)) { dontExit = false; }
		ClearScreen();
		UpdateBall(ball,0.5f);
	UpdatePlayers();
		UpdatePhysics();
		DrawBall(ball);
		DrawPlayers();
		RefreshScreen();
	//	GlobalST->BootServices->Stall(100);
	}

	DestroyBall(ball);
	PrintScores();
	Print(L"\n%EPress any key to exit.%N\n");
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Event);
#if defined(_DEBUG)
	// If running in debug mode, use the EFI shut down call to close QEMU
	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
#endif

	return EFI_SUCCESS;
}
// Global variables
EFI_SYSTEM_TABLE* GlobalST = NULL;
Framebuffer GlobalFramebuffer = { 0, 0, 0, NULLPTR };
bool AVXEnabled = false;
