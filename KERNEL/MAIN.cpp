extern"C" {

#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
}
#include "HEADER/SUBSYSTEMS/RENDER/RENDER.h"
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
	
	//dont return but shutdown instead
	Status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
	//fill framebuffer with red color
	UINT32* FrameBuffer = (UINT32*)GraphicsOutput->Mode->FrameBufferBase;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferBase = FrameBuffer;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferHeight = GraphicsOutput->Mode->Info->VerticalResolution;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::PixelsPerScanline = GraphicsOutput->Mode->Info->PixelsPerScanLine;
	UINTN FrameBufferSize = GraphicsOutput->Mode->Info->HorizontalResolution * GraphicsOutput->Mode->Info->VerticalResolution;
	for (UINTN i = 0; i < FrameBufferSize; i++) {
		FrameBuffer[i] = 0xFF0000; // Red color in ARGB format
	}
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(100,100, 'H');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(110,100, 'E');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(120,100, 'L');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(130,100, 'L');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(140,100, 'O');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(160,100, 'W');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(170,100, 'O');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(180,100, 'R');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(190,100, 'L');
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::SimpleDrawChar(200,100 ,'D');


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