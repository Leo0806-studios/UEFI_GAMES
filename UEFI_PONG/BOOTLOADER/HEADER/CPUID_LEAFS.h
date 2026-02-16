#pragma once
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdint.h>
#ifdef __cplusplus
}
#endif

#define BitfieldWidth(top,bottom) ((top) - (bottom)) +1
typedef struct {
	uint32_t maxLeafs;
	char VENDOR_ID_1[4];
	char VENDOR_ID_2[4];
	char VENDOR_ID_3[4];
}CPUIDLeaf0;
typedef struct {
	//EAX

	uint32_t steppingID : 3 - 0 + 1;
	uint32_t modelID : 7 - 4 + 1;
	uint32_t familyID : 11 - 8 + 1;
	uint32_t processorType : 13 - 12 + 1;
uint32_t: 15 - 14 + 1;
	uint32_t extendetModelID : 19 - 16 + 1;
	uint32_t extendetFamilyID : 27 - 20 + 1;
uint32_t: 31 - 28 + 1;

	//EBX
	uint32_t brandIndex : 7 - 0 + 1;
	uint32_t cFlushLineSize : 15 - 8 + 1;
	uint32_t apicIDSpace : 23 - 16 + 1;
	uint32_t LOCAL_initialApicID : 31 - 24 + 1;

	//ECX
	uint32_t SSE3 : BitfieldWidth(0, 0);
	uint32_t PCLMULQDQ : BitfieldWidth(1, 1);
	uint32_t DTES64 : BitfieldWidth(2, 2);
	uint32_t MONITOR : BitfieldWidth(3, 3);
	uint32_t DS_CPL : BitfieldWidth(4, 4);
	uint32_t VMX : BitfieldWidth(5, 5);
	uint32_t SMX : BitfieldWidth(6, 6);
	uint32_t EIST : BitfieldWidth(7, 7);
	uint32_t TM2 : BitfieldWidth(8, 8);
	uint32_t SSSE3 : BitfieldWidth(9, 9);
	uint32_t L1_CONTEXT_ID : BitfieldWidth(10, 10);
	uint32_t DEBUG_INTERFACE : BitfieldWidth(11, 11);
	uint32_t FMA : BitfieldWidth(12, 12);
	uint32_t CMPXCHG16B : BitfieldWidth(13, 13);
	uint32_t XTPR_UPDATE_CONTROL : BitfieldWidth(14, 14);
	uint32_t PERF_CAPABILITIES : BitfieldWidth(15, 15);
uint32_t:BitfieldWidth(16, 16);
	uint32_t PCID : BitfieldWidth(17, 17);
	uint32_t DCA : BitfieldWidth(18, 18);
	uint32_t SSE4_1 : BitfieldWidth(19, 19);
	uint32_t SSE4_2 : BitfieldWidth(20, 20);
	uint32_t X2APIC : BitfieldWidth(21, 21);
	uint32_t MOVBE : BitfieldWidth(22, 22);
	uint32_t POPCNT : BitfieldWidth(23, 23);
	uint32_t TSC_DEADLINE : BitfieldWidth(24, 24);
	uint32_t AESNI : BitfieldWidth(25, 25);
	uint32_t XSAVE : BitfieldWidth(26, 26);
	uint32_t LOCAL_OSXSAVE : BitfieldWidth(27, 27);
	uint32_t AVX : BitfieldWidth(28, 28);
	uint32_t F16C : BitfieldWidth(29, 29);
	uint32_t RDRAND : BitfieldWidth(30, 30);
uint32_t: BitfieldWidth(31, 31);


} CPUIDLeaf1;
typedef struct {
	uint32_t tmp;
} CPUIDLeaf2;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf3;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf4;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf5;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf6;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf7;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf8;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf9;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf10;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf11;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf12;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf13;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf14;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf15;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf16;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf17;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf18;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf19;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf20;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf21;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf22;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf23;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf24;


typedef struct {
	CPUIDLeaf0 leaf0;
	CPUIDLeaf1 leaf1;
	CPUIDLeaf2 leaf2;
	CPUIDLeaf3 leaf3;
	CPUIDLeaf4 leaf4;
	CPUIDLeaf5 leaf5;
	CPUIDLeaf6 leaf6;
	CPUIDLeaf7 leaf7;
	CPUIDLeaf8 leaf8;
	CPUIDLeaf9 leaf9;
	CPUIDLeaf10 leaf10;
	CPUIDLeaf11 leaf11;
	CPUIDLeaf12 leaf12;
	CPUIDLeaf13 leaf13;
	CPUIDLeaf14 leaf14;
	CPUIDLeaf15 leaf15;
	CPUIDLeaf16 leaf16;
	CPUIDLeaf17 leaf17;
	CPUIDLeaf18 leaf18;
	CPUIDLeaf19 leaf19;
	CPUIDLeaf20 leaf20;
	CPUIDLeaf21 leaf21;
	CPUIDLeaf22 leaf22;
	CPUIDLeaf23 leaf23;
	CPUIDLeaf24 leaf24;
}CPUIDLeafs;


