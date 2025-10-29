// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#pragma warning (disable:4702)

extern"C" {

#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
}
#include "HEADER/SUBSYSTEMS/RENDER/RENDER.h"
#include "HEADER/STARTUP/PAGE_MAP/PAGE_MAP.h"
#include "HEADER/SUBSYSTEMS/CONSOLE/CONSOLE.h"
#include "HEADER/SYTEM_INFO/SYSTEM_INFO.h"
#include <intrin.h>
#include <HEADER/UTILLITY/UTILLITY_F.h>
#include "INIT_RUNTIME.h"
#include "HEADER/SUBSYSTEMS/ALLOCATION/ALLOCATION.h"
#include "CPPRUNTIME.h"
#include "SUBSYSTEMS/WATCHDOG/WATCHDOG.h"
#include "STARTUP/PE_PARSER/PE_PARSER.h"
static uint32_t get_cpu_base_freq_mhz() {
	int cpuInfo[4] = {};
	__cpuid(&cpuInfo[0], 0x16); //-V3546 // IDK why PVS is complaining

	if (cpuInfo[0] == 0) {
		return 0; // Unsupported
	}

	return static_cast<unsigned int>(cpuInfo[0]);  // EAX = Base frequency in MHz
}
static void stall_us(uint64_t microseconds, uint64_t cpu_mhz) {
	const uint64_t start = __rdtsc();
	const uint64_t ticks = microseconds * cpu_mhz;

	while ((__rdtsc() - start) < ticks) {
		_mm_pause(); // hint to CPU: spin-wait loop
	}
}
using Console = SYSTEM::SUBSYSTEMS::CONSOLE::Console;
struct R {
	R() {
		Print(L"Hello World from Global Object Constructor.\n");

	}
};

R testglobal;
void d() {
	int i = 0;
	(void)i;

	throw R();

}
void TerminationHandler(int exitcode) {
	STD::ignore = exitcode; //-V1082
	Console::WriteLine(L"TERMINATION HANDLER CALLED SYSTEM WILL HALT");
	__halt();
}
void NullptrJmp() {
	Console::WriteLine(L"LOL YOU JUMPED TO NULLPTR     IDIOT");
}
extern "C"{
EFI_STATUS _KERNEL_MAIN(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	__assume(SystemTable != nullptr); 
	__assume(ImageHandle != nullptr);
	InitializeLib(ImageHandle, SystemTable);
	




	void* ImageBaseAdress = 0;
	EFI_STATUS ImgStatus = 0;
	EFI_LOADED_IMAGE* loadedImage = nullptr;
	EFI_GUID LoadedImageProtocoll = LOADED_IMAGE_PROTOCOL;
	ImgStatus = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3, ImageHandle, &LoadedImageProtocoll, reinterpret_cast<void**>(&loadedImage));
	if (EFI_ERROR(ImgStatus)) {
		Print(L"Failed to get image protocoll");
		return ImgStatus;
	}
	ImageBaseAdress = (loadedImage->ImageBase);
	Print(L"Hello, World!\n");
	//SystemTable->BootServices->Stall(10000000); // Stall for 1 second to allow reading the output

	STD::ignore = SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	//getMemory map
	EFI_MEMORY_DESCRIPTOR* MemoryMap = nullptr;
	UINTN MemoryMapSize = 0;
	UINTN MapKey = 0;
	UINTN DescriptorSize = 0;
	UINT32 DescriptorVersion = 0;

	//
	EFI_STATUS Status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
	if (Status == EFI_BUFFER_TOO_SMALL) {
		MemoryMapSize += DescriptorSize * 100; // Allocate more space than needed
		Status = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, reinterpret_cast<void**>(&MemoryMap));
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
	//saving the memory map to the UEFI leftover in the page map
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_ = MemoryMap;
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_MAP_SIZE = MemoryMapSize;
	SYSTEM::STARTUP::PAGING::UEFI_LEFTOWER::EFI_MEMORY_DESCRIPTOR_SIZE = DescriptorSize;
	Print(L"memory map Acquired");
	__assume(MemoryMap != nullptr);

	//print map
	bool a = false;
	for (UINTN i = 0; i < MemoryMapSize / DescriptorSize; i++) {
		__assume(MemoryMap != nullptr);
		const EFI_MEMORY_DESCRIPTOR* Descriptor = reinterpret_cast<const EFI_MEMORY_DESCRIPTOR*>(reinterpret_cast<const UINT8*>(MemoryMap) + i * DescriptorSize); //-V769 yeah there is a assume above but PVS is stoopid //-V3539
		__assume(Descriptor != nullptr); 
		Print(L"Type: %d, PhysicalStart: %lx, NumberOfPages: %lx  " , Descriptor->Type, Descriptor->PhysicalStart, Descriptor->NumberOfPages);//-V769 //the memory map should always be valid as we just got it //-V522
		if (a) {
			Print(L"\n");
		}
		a = !a;
	}

	size_t conventionlaRAM = 0;
	for (UINTN i = 0; i < MemoryMapSize / DescriptorSize; i++) {
		const EFI_MEMORY_DESCRIPTOR* Descriptor = reinterpret_cast<const EFI_MEMORY_DESCRIPTOR*>(reinterpret_cast<const UINT8*>(MemoryMap) + i * DescriptorSize); //-V3539
		__assume(Descriptor != nullptr);
		if (Descriptor->Type == EfiConventionalMemory) { //-V522 // in this situation nullptr might be a legitimate pointer to a valid memory map entry
			conventionlaRAM += Descriptor->NumberOfPages * EFI_PAGE_SIZE;
		}
		//Print(L"Type: %d, PhysicalStart: %lx, NumberOfPages: %lx  ", Descriptor->Type, Descriptor->PhysicalStart, Descriptor->NumberOfPages);
		//SystemTable->BootServices->Stall(1000000); // Stall for 1 second to allow reading the output
	}
	Print(L"Total Conventional Memory: %Lu bytes\n", conventionlaRAM); //-V111
	//print installed RAM size

	SYSTEM::SYSTEM_INFO::SystemInfo::InitSystemInfo();
	Print(L"Installed RAM: %Lu bytes\n", SYSTEM::SYSTEM_INFO::SystemInfo::GetInstance().installedRam); //-V111
	STD::ignore=SystemTable->BootServices->Stall(5000000); // Stall for 5 second to allow reading the output

	// get graphics protocol
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput = nullptr;
	Status = SystemTable->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, nullptr, reinterpret_cast<void**>(&GraphicsOutput));
	if (EFI_ERROR(Status)) {
		Print(L"Failed to locate Graphics Output Protocol.\n");
		return Status;
	}
	if(GraphicsOutput == nullptr) {
		Print(L"Graphics Output Protocol is NULL.\n");
		return EFI_NOT_FOUND;
	}
	Print(L"Graphics Output Protocol located.\n");
	const auto b = GraphicsOutput->Mode->Info->VerticalResolution;
	Print(L"Resolution: %d x %d\n", a, b);
	//=======================================EXIT_BOOT_SERVICES===============================================///
	Status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
	//fill framebuffer with red color
	UINT32* FrameBuffer = reinterpret_cast<UINT32*>(GraphicsOutput->Mode->FrameBufferBase); //-V3546 something something raw memory/physical memory
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferBase = FrameBuffer;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferHeight = GraphicsOutput->Mode->Info->VerticalResolution;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::FramebufferWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
	SYSTEM::SUBSYSTEMS::RENDER::SIMPLE::PixelsPerScanline = GraphicsOutput->Mode->Info->PixelsPerScanLine;
	const UINTN FrameBufferSize = static_cast<UINTN>(GraphicsOutput->Mode->Info->HorizontalResolution) * GraphicsOutput->Mode->Info->VerticalResolution;
	for (UINTN i = 0; i < FrameBufferSize; i++) {
	//	FrameBuffer[i] = 0xafa5ff; // Red color in ARGB format
		FrameBuffer[i] = 0xa000ff; // Red color in ARGB format //-V3539
	}
	if (!Console::InitConsole(40, 40, 16, 8)) {
		Print(L"Failed to initialize console.\n");
		return EFI_DEVICE_ERROR;
	}
	Console::WriteLine(L"  ");
	Console::WriteLine(L"CONSOLE STARTED");
	Console::WriteLine(L"CONSOLE TEST");
	Console::WriteLine(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	Console::WriteLine(L"abcdefghijklmnopqrstuvwxyz");
	Console::WriteLine(L"1234567890");
	Console::WriteLine(L"!§$%&/()=?,.-#+*<>");
	Console::WriteLine(L"KERNEL LOADING COMPLETE");
	Console::WriteLine(L"SETTING UP GLOBAL PAGE TABLE...");
	if (!SYSTEM::STARTUP::PAGING::GlobalPageMap::AllocatePageMap()) {
		Print(L"Failed to allocate global page map.\n");
		return EFI_OUT_OF_RESOURCES;
	}

	STD::ignore= SYSTEM::STARTUP::PE_PARSER::PePerser::CreateAndParse(ImageBaseAdress);
	Console::WriteLine(L"SETTING UP GDT...");
	//TODO insert call to setup of the GDT;
	SYSTEM::STARTUP::GDT::GDT::CreateGDT();
	//SYSTEM::STARTUP::GDT::GDT::LoadGDT();
	//############################## DONT REFER TO PREVIOUS STACK VARIABLES AS THEY MIGHT BE INVALID AFTER THIS POINT ##############################
	Console::WriteLine(L"GDT SETUP COMPLETE");

	Console::WriteLine(L"SETTING UP IDT...");
	//TODO insert call to setup of the IDT

	Console::WriteLine(L"IDT SETUP COMPLETE");
	Console::WriteLine(L"INITIALIZATION COMPLETE");
	Console::WriteLine(L"WELCOME TO USL OS");

	//###############################---C++ Runtime Init Here---###############################
	using PhysicalAllocator = SYSTEM::SUBSYSTEMS::ALLOCATION::PhysicalAllocator;
	RuntimeInitParameters Parameters = {
		.callbacks = {
			.TerminateProcess = TerminationHandler,
			.alocatePage = PhysicalAllocator::AllocatePage,
			.allocatePages = SYSTEM::SUBSYSTEMS::ALLOCATION::PhysicalAllocator::AllocatePages,
			.freePage = PhysicalAllocator::FreePage,
			.freePages = PhysicalAllocator::FreePages,
			.WriteLine = Console::WriteLine,
			.WatchDogStart = SYSTEM::SUBSYSTEMS::WATCHDOG::SIMPLE::StartWatchdog,
			.WatchdogCheck = SYSTEM::SUBSYSTEMS::WATCHDOG::SIMPLE::CheckWatchdg,
			.WatchdogStop=SYSTEM::SUBSYSTEMS::WATCHDOG::SIMPLE::StopWatchdog,
			
},
.initialHeapSize = 10,
		.imageBaseAddress = ImageBaseAdress,
		.pdata = {.offset = SYSTEM::STARTUP::PE_PARSER::PePerser::GetSectionOffsetByName(".pdata")},
	};
	Console::WriteLine(L"CXX RUNTIME INITIALIZATION STARTING");
	//############################################### AFTER THIS LINE GLOBAL AND STATIC VARS RESET! #####################################################
	initRuntime(Parameters);
	Console::WriteLine(L"CXX RUNTIME INITIALIZATION COMPLETE");
	Console::WriteLine(L"EXCEPTION HANDLING TEST STARTING");
	//std::terminate();
	try {
				d();

	}
	catch (R e) {
		Print(L"Exception caught: %d\n", e);
	}
	Console::WriteLine(L"EXCEPTION HANDLING TEST COMPLETE");
	size_t freq = 0;
	freq = get_cpu_base_freq_mhz();
	if (freq == 0) {
		//set to fallback 3,6ghz
		freq = 3600;
	}

	stall_us(10000000, freq);


	STD::ignore=SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, nullptr);
	return EFI_SUCCESS;

}
}


