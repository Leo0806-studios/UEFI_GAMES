#include<GLOBALS.h>

#include <gnu-efi/inc/efilib.h>
#include <gnu-efi/inc/efi.h>
#include "../../HEADER/RNG/RNG.h"

EFI_RNG_PROTOCOL* RngProtocol = NULL;




#define LCG_A 1664525
#define LCG_C 1013904223

// Simple LCG state (seeded below)
static UINT32 lcg_state = 0;

// LCG-based pseudo-random number generator
UINT32 fallback_random() {
	lcg_state = LCG_A * lcg_state + LCG_C;
	return lcg_state;
}

// Seed the fallback RNG
void seed_fallback_rng(UINT32 seed) {
	lcg_state = seed ? seed : 1;  // Avoid 0 seed
}


void InitRNG(UINT32 seed){
	EFI_STATUS Status;
	// Locate the Graphics Output Protocol
	EFI_GUID rngProtocolGuid = EFI_RNG_PROTOCOL_GUID;
	Status = uefi_call_wrapper(GlobalST->BootServices->LocateProtocol,
		3,
		&rngProtocolGuid,
		NULL,
		(VOID**)&RngProtocol);


	if (EFI_ERROR(Status)) {
		Print(L"Failed to locate RNG protocol: %r\n", Status);
		Print(L"Using fallback RNG implementation.\n");
		// Initialize the fallback RNG with a seed
		seed_fallback_rng(seed);
		return;
	}
	Print(L"RNG Protocol initialized successfully.\n");
}

void ShutdownRNG()
{
}
static UINT32 Getrnd32()
{
	if(!RngProtocol) {
		UINT32 fallbackValue = fallback_random();
		return fallbackValue;
	}
	UINT32 randomValue
		= 0;
	EFI_STATUS status;
	status = uefi_call_wrapper(RngProtocol->GetRNG, 4, RngProtocol, NULL, sizeof(UINT32), (UINT8*)&randomValue);
	if (EFI_ERROR(status)) {
		Print(L"GetRNG failed: %r\n", status);
		return 0;
	}
	return randomValue;
}
size_t GetRandomUint()
{

	size_t lowhalf = 0;
	size_t highhalf = 0;
	lowhalf = Getrnd32();
	highhalf = Getrnd32();
	// Combine the two halves to form a 64-bit random number
	__int64 randomValue = ((__int64)highhalf << 32) | lowhalf;


	return randomValue;
}

size_t GetRandomUintRange(size_t min, size_t max)
{

	if (min >= max) {
		Print(L"Invalid range: min should be less than max.\n");
		return 0;
	}
	size_t range = (max - min + 1);
	size_t randomValue = GetRandomUint();
	randomValue = (randomValue % range) + min;
	return randomValue;
}

__int64 GetRandomInt()
{

	size_t LowPart = 0;
		size_t HighPart = 0;
	LowPart = Getrnd32();
	HighPart = Getrnd32();
	__int64 randomValue = ((__int64)HighPart << 32) | LowPart;
	return randomValue;
}

__int64 GetRandomIntRange(__int64 min, __int64 max)
{

	if (min >= max) {
		Print(L"Invalid range: min should be less than max.\n");
		return 0;
	}
	size_t range = (max - min + 1);
	size_t randomValue = Getrnd32();
	randomValue = (randomValue % range) + min; // Scale to the desired range

	return randomValue;
}

float GetRandomFloat()
{

	//floats are32-bit 
	UINT32 randomValue = Getrnd32();

	return (float)randomValue / (float)UINT32_MAX; // Normalize to [0.0, 1.0]
}

float GetRandomFloatRange(float min, float max)
{

	if (min >= max) {
		Print(L"Invalid range: min should be less than max.\n");
		return 0.0f;
	}
	float range = (max - min);
	float randomValue = GetRandomFloat();
	randomValue = (randomValue * range) + min; // Scale to the desired range

	return randomValue;
}

double GetRandomDouble()
{

	// Doubles are 64-bit, so we can use two 32-bit random values
	size_t lowPart = Getrnd32();
	size_t highPart = Getrnd32();
	// Combine the two parts to form a 64-bit double
	__int64 randomValue = ((__int64)highPart << 32) | lowPart;
	// Normalize to [0.0, 1.0]
	return (double)randomValue / (double)UINT64_MAX; // Normalize to [0.0, 1.0]

}

double GetRandomDoubleRange(double min, double max)
{

	if (min >= max) {
		Print(L"Invalid range: min should be less than max.\n");
		return 0.0;
	}
	double range = (max - min);
	double randomValue = GetRandomDouble();
	randomValue = (randomValue * range) + min; // Scale to the desired range
	return randomValue;
}

void GetRandomBytes(void* buffer, size_t size)
{
///
	///if (buffer == NULL || size == 0) {
///		Print(L"Invalid buffer or size.\n");
///		return;
	///}
	///EFI_STATUS status = uefi_call_wrapper(RngProtocol->GetRNG, 4, RngProtocol, NULL, size, (UINT8*)buffer);
	///if (EFI_ERROR(status)) {
///		Print(L"GetRNG failed: %r\n", status);
///		return;
	///}
	///if (size % 4 == 0) {
///		//great we can just use the rng protocol to get 32-bit random values
///		UINT32* uintBuffer = (UINT32*)buffer;
///		size_t numValues = size / sizeof(UINT32);
///		for (size_t i = 0; i < numValues; i++) {
///			uintBuffer[i] = Getrnd32();
///		}
	///}
	///else {
///		//not great. weel just take the bottom 8 bits of the 32-bit random value and treat it like a char array
///		UINT8* byteBuffer = (UINT8*)buffer;
///		size_t numBytes = size / sizeof(UINT8);
///		for (size_t i = 0; i < numBytes; i++) {
///			UINT32 randomValue = Getrnd32();
///			byteBuffer[i] = (UINT8)(randomValue & 0xFF); // Take the bottom 8 bits
///		}
///
	///}
}


