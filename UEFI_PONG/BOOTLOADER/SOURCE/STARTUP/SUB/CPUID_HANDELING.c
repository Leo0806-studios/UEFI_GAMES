#include <intrin.h>
#include "CPUID_LEAFS.h"
#include "BOOT_DATA_INTERFACE.h"
#include"SUB_FORWARD_DECLARATIONS.h"




/// <summary>
/// allocates the memory for num+1 subleafs and zeros the memory
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
static int* AllocateSubLeafsZ(unsigned int num) {
	if (num > UINT32_MAX / 2) {
		return NULL; // Prevent overflow
	}
	void* ptr = Alloc((sizeof(uint32_t) * 4) * (num + 1));
	if (!ptr) {
		return NULL; // Allocation failed
	}
	MEMSET((char*)ptr, 0, (sizeof(uint32_t) * 4) * (num + 1));
	return ptr;

}

static	int buff[(sizeof(CPUIDLeafs) / sizeof(int)) / 4][4] = { 0 };//EAX, EBX, ECX, and EDX registers (in that order) (from https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=msvc-170#remarks)
static EFI_STATUS QueryInnerCPUIDLoop(const int leaf, uint32_t* index) {

	switch (leaf) {
		case 0x04: {
			int localBuff[4] = { 0,0,0,0 };
			int SubLeafCount = 0;
			for (unsigned int i = 0;; i++) {
				__cpuidex(localBuff, leaf, i);
				SubLeafCount++;
				if (!((CPUIDLeaf0x04SubLeaf*)localBuff)->CACHE_LEVEL) {
					break;
				}
			}
			int* AllocatedSubLeafs = AllocateSubLeafsZ(SubLeafCount);
			if (!AllocatedSubLeafs) {
				Print(L"Failed to allocate memory for CPUID Leaf 0x04 subleafs\n");
				return EFI_ABORTED;
			}


			for (unsigned int i = 0; i < SubLeafCount; i++) {

				__cpuidex(&localBuff[0], leaf, i);
				Print(L"SubLeaf: %d, EAX %d, EBX %d, ECX %d, EDX %d\n", i, localBuff[0], localBuff[1], localBuff[2], localBuff[3]);
				MEMCPY(&AllocatedSubLeafs[i * 4], localBuff, sizeof(localBuff));

			}

			((CPUIDLeaf0x04*)buff[*index])->subleafs = AllocatedSubLeafs;
			break;
		}
		case 0x07: {
			int localbuff[4] = { 0,0,0,0 };
			__cpuidex(localbuff, leaf, 0);
			uint32_t maxLeaf = ((CPUIDLeaf0x07SubLeaf0*)localbuff)->MAX_SUBLEAF;
			//const int a = 
			if (maxLeaf >= sizeof(CPUIDLeaf0x07) / sizeof(uint32_t) / 4) {
				Print(L"more subleafs than currently supported\n");
				return EFI_ABORTED;
			}
			for (uint32_t subLeaf = 0; subLeaf <= maxLeaf; subLeaf++) {

				__cpuidex(buff[*index], leaf, subLeaf);
				Print(L"SubLeaf: %d, EAX %d, EBX %d, ECX %d, EDX %d\n", subLeaf, buff[*index][0], buff[*index][1], buff[*index][2], buff[*index][3]);

				(*index)++;
			}

			break;
		}
		case 0x0B: {
			int localBuff[4] = { 0,0,0,0 };
			int SubLeafCount = 0;
			for (unsigned int i = 0;; i++) {
				__cpuidex(localBuff, leaf, i);
				SubLeafCount++;
				if (!((CPUIDLeaf0x0BSubLeaf*)localBuff)->LOCAL_NEXT_LEVEL_NUM_LP) {
					break;
				}
			}
			int* AllocatedSubLeafs = AllocateSubLeafsZ(SubLeafCount);
			if (!AllocatedSubLeafs) {
				Print(L"Failed to allocate memory for CPUID Leaf 0x04 subleafs\n");
				return 0;
			}
			for (unsigned int i = 0; i < SubLeafCount; i++) {

				__cpuidex(&localBuff[0], leaf, i);
				Print(L"SubLeaf: %d, EAX %d, EBX %d, ECX %d, EDX %d\n", i, localBuff[0], localBuff[1], localBuff[2], localBuff[3]);
				MEMCPY(&AllocatedSubLeafs[i * 4], localBuff, sizeof(localBuff));

			}

			((CPUIDLeaf0x0B*)buff[*index])->subleafs = AllocatedSubLeafs;
			break;
		}
		case 0x0D: {
			break;
		}
		case 0x0F: {
			break;
		}
		case 0x10: {
			break;
		}
		case 0x12: {
			break;
		}
		case 0x14: {
			break;
		}
		case 0x17: {
			break;
		}
		case 0x18: {
			break;
		}
		case 0x1d: {
			break;
		}
		case 0x1F: {
			break;
		}
		case 0x20: {
			break;
		}
		case 0x23: {
			break;
		}
		case 0x24: {
			break;
		}
		default: {
			int localbuff[4] = { 0,2,3,4 };

			__cpuidex(localbuff, leaf, 0);

			Print(L"Leaf %d, EAX %d, EBX %d, ECX %d, EDX %d \n", leaf, localbuff[0], localbuff[1], localbuff[2], localbuff[3]);
			MEMCPY(buff[*index], localbuff, sizeof(localbuff));
			//Print(L"Leaf %d, EAX %d, EBX %d, ECX %d, EDX %d \n", leaf, buff[*index][0], buff[*index][1], buff[*index][2], buff[*index][3]);

			(*index)++;
			break;
		}


	}
	return EFI_SUCCESS;

}


CPUID ParseCPUID(void) {
	static_assert(sizeof(int) == 4, "This code assumes that sizeof(int) is 4 bytes");
	static_assert(sizeof(buff) == sizeof(buff), " buff and CPUID must be the same size");
	static_assert(sizeof(CPUIDLeafs) % 4 == 0, "CPUIDLeafs must be a multiple of 4 bytes in size");
	MEMSET(buff, 0, sizeof(buff));
	int localbuff[4] = { 0,0,0,0 };
	__cpuid(buff[0], 0);
	__cpuid(&localbuff[0], 0);
	CPUIDLeafs allLeafs = { 0 };
	Print(L"Leaf %d, EAX %d, EBX %d, ECX %d, EDX %d \n", 0, buff[0][0], buff[0][1], buff[0][2], buff[0][3]);

	const int maxCPUIDLeaves = buff[0][0];

	char venStr[4 * 3 + 1] = { 0 };
	((int*)venStr)[0] = localbuff[1];
	((int*)venStr)[1] = localbuff[3];
	((int*)venStr)[2] = localbuff[2];
	venStr[12] = 0x0;
	Print(L"Vendor ID str: %s", venStr);
	Print(L"Max leafs supported: %d", maxCPUIDLeaves);
	uint32_t index = 1;
	for (int leaf = 1; leaf < maxCPUIDLeaves; leaf++) {
		Print(L"Querying CPUID Leaf %d\n", leaf);
		QueryInnerCPUIDLoop(leaf, &index);
	}
	MEMCPY(&allLeafs, buff, sizeof(CPUIDLeafs));
	return (CPUID) {
		.Version = CPUID_VERSION, .Size = sizeof(CPUID), .Leafs = allLeafs
	};
}