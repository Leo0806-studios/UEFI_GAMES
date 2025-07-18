extern"C" {

#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
}
#include "HEADER/SUBSYSTEMS/RENDER/RENDER.h"
#include "HEADER/STARTUP/PAGE_MAP/PAGE_MAP.h"
#include "HEADER/SUBSYSTEMS/CONSOLE/CONSOLE.h"
#include "HEADER/STARTUP/PAGE_MAP/PAGE_MAP.h"
#include "HEADER/SYTEM_INFO/SYSTEM_INFO.h"
#include <intrin.h>
uint32_t get_cpu_base_freq_mhz() {
	int cpuInfo[4];
	__cpuid(cpuInfo, 0x16);

	if (cpuInfo[0] == 0) {
		return 0; // Unsupported
	}

	return static_cast<unsigned int>(cpuInfo[0]);  // EAX = Base frequency in MHz
}
void stall_us(uint64_t microseconds, uint64_t cpu_mhz) {
	uint64_t start = __rdtsc();
	uint64_t ticks = microseconds * cpu_mhz;

	while ((__rdtsc() - start) < ticks) {
		_mm_pause(); // hint to CPU: spin-wait loop
	}
}
using Console = SYSTEM::SUBSYSTEMS::CONSOLE::Console;
extern "C"{
EFI_STATUS _KERNEL_MAIN(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable);

	Print(L"Hello, World!\n");
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	//getMemory map
	EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
	UINTN MemoryMapSize = 0;
	UINTN MapKey = 0;
	UINTN DescriptorSize = 0;
	UINT32 DescriptorVersion = 0;

	//
	EFI_STATUS Status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
	if (Status == EFI_BUFFER_TOO_SMALL) {
		MemoryMapSize += DescriptorSize * 100; // Allocate more space than needed
		Status = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, (void**)&MemoryMap);
		if (EFI_ERROR(Status)) {
			Print(L"Failed to allocate memory for the memory map.\n");
			return Status;
		}
		Status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
		if (EFI_ERROR(Status)) {
			Print(L"Failed to get memory map.\n");
			return Status;
		}
	}
	//saving the memory map to the uefi leftover in the page map
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_ = MemoryMap;
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE = MemoryMapSize;
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_SIZE = DescriptorSize;
	Print(L"memory map Aquired");
	//print mamp
	bool a = false;
	for (UINTN i = 0; i < MemoryMapSize / DescriptorSize; i++) {
		EFI_MEMORY_DESCRIPTOR* Descriptor = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)MemoryMap + i * DescriptorSize);
		Print(L"Type: %d, PhysicalStart: %lx, NumberOfPages: %lx  " , Descriptor->Type, Descriptor->PhysicalStart, Descriptor->NumberOfPages);
		if (a) {
			Print(L"\n");
		}
		a = !a;
	}

	size_t conventionlaRAM = 0;
	for (UINTN i = 0; i < MemoryMapSize / DescriptorSize; i++) {
		EFI_MEMORY_DESCRIPTOR* Descriptor = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)MemoryMap + i * DescriptorSize);
		if (Descriptor->Type == EfiConventionalMemory) {
			conventionlaRAM += Descriptor->NumberOfPages * EFI_PAGE_SIZE;
		}
		//Print(L"Type: %d, PhysicalStart: %lx, NumberOfPages: %lx  ", Descriptor->Type, Descriptor->PhysicalStart, Descriptor->NumberOfPages);
		//SystemTable->BootServices->Stall(1000000); // Stall for 1 second to allow reading the output
	}
	Print(L"Total Conventional Memory: %d bytes\n", conventionlaRAM);
	//print installed RAM size
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_ = reinterpret_cast<void*>(MemoryMap);
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE = MemoryMapSize;
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_SIZE = DescriptorSize;
	SYSTEM::SYSTEM_INFO::SystemInfo::InitSystemInfo();
	Print(L"Installed RAM: %d bytes\n", SYSTEM::SYSTEM_INFO::SystemInfo::GetInstance().installedRam);
	SystemTable->BootServices->Stall(5000000); // Stall for 5 second to allow reading the output

	// get graphics protocol
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput = NULL;
	Status = SystemTable->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&GraphicsOutput);
	if (EFI_ERROR(Status)) {
		Print(L"Failed to locate Graphics Output Protocol.\n");
		return Status;
	}
	Print(L"Graphics Output Protocol located.\n");
	auto b = GraphicsOutput->Mode->Info->VerticalResolution;
	Print(L"Resolution: %d x %d\n", a, b);
	//=======================================EXIT_BOOT_SERVICES===============================================///
	Status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
	//fill framebuffer with red color
	UINT32* FrameBuffer = (UINT32*)GraphicsOutput->Mode->FrameBufferBase;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferBase = FrameBuffer;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferHeight = GraphicsOutput->Mode->Info->VerticalResolution;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::PixelsPerScanline = GraphicsOutput->Mode->Info->PixelsPerScanLine;
	UINTN FrameBufferSize = GraphicsOutput->Mode->Info->HorizontalResolution * GraphicsOutput->Mode->Info->VerticalResolution;
	for (UINTN i = 0; i < FrameBufferSize; i++) {
	//	FrameBuffer[i] = 0xafa5ff; // Red color in ARGB format
		FrameBuffer[i] = 0xa000ff; // Red color in ARGB format
	}
	Console::InitConsole(40, 40, 16, 8);

	Console::WriteLine(L"CONSOLE STARTED");
	Console::WriteLine(L"CONSOLE TEST");
	Console::WriteLine(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	Console::WriteLine(L"abcdefghijklmnopqrstuvwxyz");
	Console::WriteLine(L"1234567890");
	Console::WriteLine(L"!§$%&/()=?,.-#+*<>");
	Console::WriteLine(L"KERNEL LOADING COMPLETE");
	Console::WriteLine(L"SETTING UP GLOBAL PAGE TABLE...");
	SYSTEM::STARTUP::PAGING::GlobalPageMap::AllocatePAgeMap();


	Console::WriteLine(L"SETTING UP GDT...");
	//TODO insert call to setup of the GDT;
	Console::WriteLine(L"GDT SETUP COMPLETE");

	Console::WriteLine(L"SETTING UP IDT...");
	//TODO insert call to setup of the IDT

	Console::WriteLine(L"IDT SETUP COMPLETE");
	size_t freq = 0;
	freq = get_cpu_base_freq_mhz();
	if (freq == 0) {
		//set to fallback 3,6ghz
		freq = 3600;
	}
	stall_us(10000000, freq);


	SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;

}
}