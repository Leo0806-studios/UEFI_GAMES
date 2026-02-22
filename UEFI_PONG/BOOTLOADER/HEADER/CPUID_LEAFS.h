#pragma once
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdint.h>
#ifdef __cplusplus
}
#endif
//
// Edge cases which might need post loop fix-up
// 0x04
// 0x07
// 0x0B
// 0x0D
// 
//



#define BitfieldWidth(top,bottom) ((top) - (bottom)) +1
#define BitFieldWitdhEAX(top, bottom) BitfieldWidth(top, bottom)
#define BitFieldWitdhEBX(top, bottom) BitfieldWidth(top, bottom)
#define BitFieldWitdhECX(top, bottom) BitfieldWidth(top, bottom)
#define BitFieldWitdhEDX(top, bottom) BitfieldWidth(top, bottom)
typedef struct {
	uint32_t MAX_LEAF;
	char VENDOR_ID_1[4];
	char VENDOR_ID_2[4];
	char VENDOR_ID_3[4];
}CPUIDLeaf0x00;
typedef struct {
	//EAX

	uint32_t STEPPING_ID : BitFieldWitdhEAX(3, 0);
	uint32_t MODEL_ID : BitFieldWitdhEAX(7, 4);
	uint32_t FAMILY_ID : BitFieldWitdhEAX(11, 8);
	uint32_t PROCESSOR_TYPE : BitFieldWitdhEAX(13, 12);
uint32_t: 15 - 14 + 1;
	uint32_t EXTENDED_MODEL_ID : BitFieldWitdhEAX(19, 16);
	uint32_t EXTENDED_FAMILY_ID : BitFieldWitdhEAX(27, 20);
uint32_t: 31 - 28 + 1;

	//EBX
	uint32_t BRAND_INDEX : BitFieldWitdhEBX(7, 0);
	uint32_t CLFLUSH_LINE_SIZE : BitFieldWitdhEBX(15, 8);
	uint32_t APIC_ID_SPACE : BitFieldWitdhEBX(23, 16);
	uint32_t LOCAL_INITIAL_APIC_ID : BitFieldWitdhEBX(31, 24);

	//ECX
	uint32_t SSE3 : BitFieldWitdhECX(0, 0);
	uint32_t PCLMULQDQ : BitFieldWitdhECX(1, 1);
	uint32_t DTES64 : BitFieldWitdhECX(2, 2);
	uint32_t MONITOR : BitFieldWitdhECX(3, 3);
	uint32_t DS_CPL : BitFieldWitdhECX(4, 4);
	uint32_t VMX : BitFieldWitdhECX(5, 5);
	uint32_t SMX : BitFieldWitdhECX(6, 6);
	uint32_t EIST : BitFieldWitdhECX(7, 7);
	uint32_t TM2 : BitFieldWitdhECX(8, 8);
	uint32_t SSSE3 : BitFieldWitdhECX(9, 9);
	uint32_t L1_CONTEXT_ID : BitFieldWitdhECX(10, 10);
	uint32_t DEBUG_INTERFACE : BitFieldWitdhECX(11, 11);
	uint32_t FMA : BitFieldWitdhECX(12, 12);
	uint32_t CMPXCHG16B : BitFieldWitdhECX(13, 13);
	uint32_t XTPR_UPDATE_CONTROL : BitFieldWitdhECX(14, 14);
	uint32_t PERF_CAPABILITIES : BitFieldWitdhECX(15, 15);
uint32_t:BitFieldWitdhECX(16, 16);
	uint32_t PCID : BitFieldWitdhECX(17, 17);
	uint32_t DCA : BitFieldWitdhECX(18, 18);
	uint32_t SSE4_1 : BitFieldWitdhECX(19, 19);
	uint32_t SSE4_2 : BitFieldWitdhECX(20, 20);
	uint32_t X2APIC : BitFieldWitdhECX(21, 21);
	uint32_t MOVBE : BitFieldWitdhECX(22, 22);
	uint32_t POPCNT : BitFieldWitdhECX(23, 23);
	uint32_t TSC_DEADLINE : BitFieldWitdhECX(24, 24);
	uint32_t AESNI : BitFieldWitdhECX(25, 25);
	uint32_t XSAVE : BitFieldWitdhECX(26, 26);
	uint32_t LOCAL_OSXSAVE : BitFieldWitdhECX(27, 27);
	uint32_t AVX : BitFieldWitdhECX(28, 28);
	uint32_t F16C : BitFieldWitdhECX(29, 29);
	uint32_t RDRAND : BitFieldWitdhECX(30, 30);
uint32_t: BitFieldWitdhECX(31, 31);

	//EDX
	uint32_t FPU : BitFieldWitdhEDX(0, 0);
	uint32_t VME : BitFieldWitdhEDX(1, 1);
	uint32_t DE : BitFieldWitdhEDX(2, 2);
	uint32_t PSE : BitFieldWitdhEDX(3, 3);
	uint32_t TSC : BitFieldWitdhEDX(4, 4);
	uint32_t MSR : BitFieldWitdhEDX(5, 5);
	uint32_t PAE : BitFieldWitdhEDX(6, 6);
	uint32_t MCE : BitFieldWitdhEDX(7, 7);
	uint32_t CMPXCHG8B : BitFieldWitdhEDX(8, 8);
	uint32_t APIC : BitFieldWitdhEDX(9, 9);
uint32_t: BitFieldWitdhEDX(10, 10);
	uint32_t SEP : BitFieldWitdhEDX(11, 11);
	uint32_t MTRR : BitFieldWitdhEDX(12, 12);
	uint32_t PGE : BitFieldWitdhEDX(13, 13);
	uint32_t MCA : BitFieldWitdhEDX(14, 14);
	uint32_t CMOV : BitFieldWitdhEDX(15, 15);
	uint32_t PAT : BitFieldWitdhEDX(16, 16);
	uint32_t PSE_36 : BitFieldWitdhEDX(17, 17);
	uint32_t PSN : BitFieldWitdhEDX(18, 18);
	uint32_t CLFSH : BitFieldWitdhEDX(19, 19);
uint32_t: BitFieldWitdhEDX(20, 20);
	uint32_t DEBUG_STORE : BitFieldWitdhEDX(21, 21);
	uint32_t ACPI : BitFieldWitdhEDX(22, 22);
	uint32_t MMX : BitFieldWitdhEDX(23, 23);
	uint32_t FXSR : BitFieldWitdhEDX(24, 24);
	uint32_t SSE : BitFieldWitdhEDX(25, 25);
	uint32_t SSE2 : BitFieldWitdhEDX(26, 26);
	uint32_t SELF_SNOOP : BitFieldWitdhEDX(27, 27);
	uint32_t HTT : BitFieldWitdhEDX(28, 28);
	uint32_t TM : BitFieldWitdhEDX(29, 29);
uint32_t: BitFieldWitdhEDX(30, 30);
	uint32_t PBE : BitFieldWitdhEDX(31, 31);
} CPUIDLeaf0x01;

typedef struct {
	//EAX
uint32_t:BitFieldWitdhEAX(7, 0);
	uint32_t DESCRIPTOR_1 : BitFieldWitdhEAX(15, 8);
	uint32_t DESCRIPTOR_2 : BitFieldWitdhEAX(23, 16);
	uint32_t DESCRIPTOR_3 : BitFieldWitdhEAX(31, 24);
	//EBX
	uint32_t DESCRIPTOR_4 : BitFieldWitdhEBX(7, 0);
	uint32_t DESCRIPTOR_5 : BitFieldWitdhEBX(15, 8);
	uint32_t DESCRIPTOR_6 : BitFieldWitdhEBX(23, 16);
	uint32_t DESCRIPTOR_7 : BitFieldWitdhEBX(31, 24);
	//ECX
	uint32_t DESCRIPTOR_8 : BitFieldWitdhECX(7, 0);
	uint32_t DESCRIPTOR_9 : BitFieldWitdhECX(15, 8);
	uint32_t DESCRIPTOR_10 : BitFieldWitdhECX(23, 16);
	uint32_t DESCRIPTOR_11 : BitFieldWitdhECX(31, 24);
	//EDX
	uint32_t DESCRIPTOR_12 : BitFieldWitdhEDX(7, 0);
	uint32_t DESCRIPTOR_13 : BitFieldWitdhEDX(15, 8);
	uint32_t DESCRIPTOR_14 : BitFieldWitdhEDX(23, 16);
	uint32_t DESCRIPTOR_15 : BitFieldWitdhEDX(31, 24);
} CPUIDLeaf0x02;

//likely compleatly \0 reserved on all supported processors but ill parse it just in case
typedef struct {
	//EAX
uint32_t:BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t:BitFieldWitdhEBX(31, 0);
	//ECX
	uint32_t PSN_31_0 : BitFieldWitdhECX(31, 0);
	//EDX
	uint32_t PSN_63_32 : BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x03;


//likely not very usefull bc most of the content is Logical Processor specific but ill parse it just in case. this means that other cores will have to call cpuid for the info themselfs
typedef struct {
	//EAX
	uint32_t CACHE_TYPE : BitFieldWitdhEAX(4, 0);
	uint32_t CACHE_LEVEL : BitFieldWitdhEAX(7, 5);
	uint32_t SELF_INITIALIZING : BitFieldWitdhEAX(8, 8);
	uint32_t FULLY_ASSOCIATIVE : BitFieldWitdhEAX(9, 9);
uint32_t: BitFieldWitdhEAX(13, 10);
	uint32_t MAX_LP_ADDRESSABLE_IDS : BitFieldWitdhEAX(25, 14);
	uint32_t MAX_CORES_ADDRESSABLE_IDS_PKG : BitFieldWitdhEAX(31, 26);
	//EBX
	uint32_t LINE_SIZE : BitFieldWitdhEBX(11, 0);
	uint32_t PHYS_LINE_PARTITIONS : BitFieldWitdhEBX(21, 12);
	uint32_t NUM_WAYS : BitFieldWitdhEBX(31, 22);
	//ECX
	uint32_t NUM_SETS : BitFieldWitdhECX(31, 0);
	//EDX
	uint32_t NOT_LWR_CACHE_FLUSH : BitFieldWitdhEDX(0, 0);
	uint32_t INCLUSIVE_CACHE : BitFieldWitdhEDX(1, 1);
	uint32_t COMPLEX_CACHE_INDEXING : BitFieldWitdhEDX(2, 2);
uint32_t: BitFieldWitdhEDX(31, 3);
} CPUIDLeaf0x04SubLeaf;
typedef struct {
	CPUIDLeaf0x04SubLeaf* subleafs;
uint64_t:BitfieldWidth(63, 0);
} CPUIDLeaf0x04;

typedef struct {
	//EAX
	uint32_t SMALLEST_MONITOR_LINE_SIZE : BitFieldWitdhEAX(15, 0);
uint32_t: BitFieldWitdhEAX(31, 16);
	//EBX
	uint32_t LARGEST_MONITOR_LINE_SIZE : BitFieldWitdhEBX(15, 0);
uint32_t: BitFieldWitdhEBX(31, 16);
	//ECX
	uint32_t MONITOR_MWAIT_EXTENSIONS : BitFieldWitdhECX(0, 0);
	uint32_t INTERUPT_AS_BREAK_EVENT : BitFieldWitdhECX(1, 1);
uint32_t: BitFieldWitdhECX(31, 2);
	//EDX
	uint32_t C0_SUB_STATE : BitFieldWitdhEDX(3, 0);
	uint32_t C1_SUB_STATE : BitFieldWitdhEDX(7, 4);
	uint32_t C2_SUB_STATE : BitFieldWitdhEDX(11, 8);
	uint32_t C3_SUB_STATE : BitFieldWitdhEDX(15, 12);
	uint32_t C4_SUB_STATE : BitFieldWitdhEDX(19, 16);
	uint32_t C5_SUB_STATE : BitFieldWitdhEDX(23, 20);
	uint32_t C6_SUB_STATE : BitFieldWitdhEDX(27, 24);
	uint32_t C7_SUB_STATE : BitFieldWitdhEDX(31, 28);

} CPUIDLeaf0x05;

typedef struct {
	uint32_t DIGITAL_TEMP_SENSOR : BitFieldWitdhEAX(0, 0);
	uint32_t TURBO_BOOST : BitFieldWitdhEAX(1, 1);
	uint32_t ALWAYS_RUNNING_APIC_TIMER : BitFieldWitdhEAX(2, 2);
uint32_t:BitFieldWitdhEAX(3, 3);
	uint32_t POWER_LIMIT_NOTIFY : BitFieldWitdhEAX(4, 4);
	uint32_t EXT_CLOCK_MOD : BitFieldWitdhEAX(5, 5);
	uint32_t PKG_THERM_MGMT : BitFieldWitdhEAX(6, 6);
	uint32_t HWP : BitFieldWitdhEAX(7, 7);
	uint32_t HWP_INTERRUPT : BitFieldWitdhEAX(8, 8);
	uint32_t HWP_ACTIVITY_WINDOW : BitFieldWitdhEAX(9, 9);
	uint32_t HWP_EPP : BitFieldWitdhEAX(10, 10);
	uint32_t HWP_REQUEST_PKG : BitFieldWitdhEAX(11, 11);
uint32_t:BitFieldWitdhEAX(12, 12);
	uint32_t HDC : BitFieldWitdhEAX(13, 13);
	uint32_t TURBO_BOOST_MAX : BitFieldWitdhEAX(14, 14);
	uint32_t HWP_CAP : BitFieldWitdhEAX(15, 15);
	uint32_t HWP_PECI_OVERRIDE : BitFieldWitdhEAX(16, 16);
	uint32_t FLEXIBLE_HWP : BitFieldWitdhEAX(17, 17);
	uint32_t HWP_REQUEST_FAST_ACCESS : BitFieldWitdhEAX(18, 18);
	uint32_t HW_FEEDBACK : BitFieldWitdhEAX(19, 19);
	uint32_t HWP_REQUEST_IGNORE_IDLE : BitFieldWitdhEAX(20, 20);
uint32_t:BitFieldWitdhEAX(21, 21);
	uint32_t HWP_CTL : BitFieldWitdhEAX(22, 22);
	uint32_t THREAD_DIRECTOR : BitFieldWitdhEAX(23, 23);
uint32_t:BitFieldWitdhEAX(31, 24);
	uint32_t DTS_NUM_INT_THRESHOLDS : BitFieldWitdhEBX(3, 0);
uint32_t:BitFieldWitdhEBX(31, 4);
	uint32_t HW_FEEDBACK_CAP : BitFieldWitdhECX(0, 0);
uint32_t:BitFieldWitdhECX(2, 1);
	uint32_t ENERGY_PERF_BIAS : BitFieldWitdhECX(3, 3);
uint32_t:BitFieldWitdhECX(7, 4);
	uint32_t HW_FEEDBACK_NUM_CLASSES : BitFieldWitdhECX(15, 8);
uint32_t:BitFieldWitdhECX(31, 16);
	uint32_t HW_FEEDBACK_CAPS : BitFieldWitdhEDX(7, 0);
	uint32_t HW_FEEDBACK_TABLE_SIZE : BitFieldWitdhEDX(11, 8);
uint32_t:BitFieldWitdhEDX(15, 12);
	uint32_t LOCAL_HW_FEEDBACK_TABLE_INDEX : BitFieldWitdhEDX(31, 16);
} CPUIDLeaf0x06;


typedef struct {
	uint32_t MAX_SUBLEAF : BitFieldWitdhEAX(31, 0);
	//EBX
	uint32_t FSGSBASE : BitFieldWitdhEBX(0, 0);
	uint32_t TSC_ADJUST : BitFieldWitdhEBX(1, 1);
	uint32_t SGX : BitFieldWitdhEBX(2, 2);
	uint32_t BMI1 : BitFieldWitdhEBX(3, 3);
	uint32_t HLE : BitFieldWitdhEBX(4, 4);
	uint32_t AVX2 : BitFieldWitdhEBX(5, 5);
	uint32_t FDP_EXCPTN_ONLY : BitFieldWitdhEBX(6, 6);
	uint32_t SMEP : BitFieldWitdhEBX(7, 7);
	uint32_t BMI2 : BitFieldWitdhEBX(8, 8);
	uint32_t ENH_REP_MOVSB_STOSB : BitFieldWitdhEBX(9, 9);
	uint32_t INVPCID : BitFieldWitdhEBX(10, 10);
	uint32_t RTM : BitFieldWitdhEBX(11, 11);
	uint32_t RDT_M : BitFieldWitdhEBX(12, 12);
	uint32_t FCS_FDS_DEPRECATION : BitFieldWitdhEBX(13, 13);
	uint32_t MPX : BitFieldWitdhEBX(14, 14);
	uint32_t RDT_A : BitFieldWitdhEBX(15, 15);
	uint32_t AVX512F : BitFieldWitdhEBX(16, 16);
	uint32_t AVX512DQ : BitFieldWitdhEBX(17, 17);
	uint32_t RDSEED : BitFieldWitdhEBX(18, 18);
	uint32_t ADX : BitFieldWitdhEBX(19, 19);
	uint32_t SMAP : BitFieldWitdhEBX(20, 20);
	uint32_t AVX512_IFMA : BitFieldWitdhEBX(21, 21);
uint32_t: BitFieldWitdhEBX(22, 22);
	uint32_t CLFLUSHOPT : BitFieldWitdhEBX(23, 23);
	uint32_t CLWB : BitFieldWitdhEBX(24, 24);
	uint32_t INTEL_PROC_TRACE : BitFieldWitdhEBX(25, 25);
	uint32_t AVX512PF : BitFieldWitdhEBX(26, 26);
	uint32_t AVX512ER : BitFieldWitdhEBX(27, 27);
	uint32_t AVX512CD : BitFieldWitdhEBX(28, 28);
	uint32_t SHA : BitFieldWitdhEBX(29, 29);
	uint32_t AVX512BW : BitFieldWitdhEBX(30, 30);
	uint32_t AVX512VL : BitFieldWitdhEBX(31, 31);
	//ECX
	uint32_t PREFETCHWT1 : BitFieldWitdhECX(0, 0);
	uint32_t AVX512_VBMI : BitFieldWitdhECX(1, 1);
	uint32_t UMIP : BitFieldWitdhECX(2, 2);
	uint32_t PKU : BitFieldWitdhECX(3, 3);
	uint32_t OSPKE : BitFieldWitdhECX(4, 4);
	uint32_t WAITPKG : BitFieldWitdhECX(5, 5);
	uint32_t AVX512_VBMI2 : BitFieldWitdhECX(6, 6);
	uint32_t CET_SS : BitFieldWitdhECX(7, 7);
	uint32_t GFNI : BitFieldWitdhECX(8, 8);
	uint32_t VAES : BitFieldWitdhECX(9, 9);
	uint32_t VPCLMULQDQ : BitFieldWitdhECX(10, 10);
	uint32_t AVX512_VNNI : BitFieldWitdhECX(11, 11);
	uint32_t AVX512_BITALG : BitFieldWitdhECX(12, 12);
	uint32_t TME_EN : BitFieldWitdhECX(13, 13);
	uint32_t AVX512_VPOPCNTDQ : BitFieldWitdhECX(14, 14);
uint32_t: BitFieldWitdhECX(15, 15);
	uint32_t LA57 : BitFieldWitdhECX(16, 16);
	uint32_t MPX_MAWAU : BitFieldWitdhECX(21, 17);
	uint32_t RDPID : BitFieldWitdhECX(22, 22);
	uint32_t KEY_LOCKER : BitFieldWitdhECX(23, 23);
	uint32_t BUS_LOCK_DETECT : BitFieldWitdhECX(24, 24);
	uint32_t CLDEMOTE : BitFieldWitdhECX(25, 25);
uint32_t: BitFieldWitdhECX(26, 26);
	uint32_t MOVDIRI : BitFieldWitdhECX(27, 27);
	uint32_t MOVDIR64B : BitFieldWitdhECX(28, 28);
	uint32_t ENQCMD : BitFieldWitdhECX(29, 29);
	uint32_t SGX_LC : BitFieldWitdhECX(30, 30);
	uint32_t PKS : BitFieldWitdhECX(31, 31);
	//EDX
uint32_t: BitFieldWitdhEDX(0, 0);
	uint32_t SGX_KEYS : BitFieldWitdhEDX(1, 1);
	uint32_t AVX512_4VNNIW : BitFieldWitdhEDX(2, 2);
	uint32_t AVX512_4FMAPS : BitFieldWitdhEDX(3, 3);
	uint32_t FAST_SHORT_REP_MOVSB : BitFieldWitdhEDX(4, 4);
	uint32_t UINTR : BitFieldWitdhEDX(5, 5);
uint32_t: BitFieldWitdhEDX(7, 6);
	uint32_t AVX512_VP2INTERSECT : BitFieldWitdhEDX(8, 8);
	uint32_t MCU_OPT_CTRL : BitFieldWitdhEDX(9, 9);
	uint32_t MD_CLEAR : BitFieldWitdhEDX(10, 10);
	uint32_t RTM_ALWAYS_ABORT : BitFieldWitdhEDX(11, 11);
uint32_t: BitFieldWitdhEDX(12, 12);
	uint32_t RTM_FORCE_ABORT : BitFieldWitdhEDX(13, 13);
	uint32_t SERIALIZE : BitFieldWitdhEDX(14, 14);
	uint32_t HYBRID : BitFieldWitdhEDX(15, 15);
	uint32_t TSXLDTRK : BitFieldWitdhEDX(16, 16);
uint32_t: BitFieldWitdhEDX(17, 17);
	uint32_t PCONFIG : BitFieldWitdhEDX(18, 18);
	uint32_t ARCH_LBRS : BitFieldWitdhEDX(19, 19);
	uint32_t CET_IBT : BitFieldWitdhEDX(20, 20);
uint32_t: BitFieldWitdhEDX(21, 21);
	uint32_t AMX_BF16 : BitFieldWitdhEDX(22, 22);
	uint32_t AVX512_FP16 : BitFieldWitdhEDX(23, 23);
	uint32_t AMX_TILE : BitFieldWitdhEDX(24, 24);
	uint32_t AMX_INT8 : BitFieldWitdhEDX(25, 25);
	uint32_t IBRS_IBPB : BitFieldWitdhEDX(26, 26);
	uint32_t SPEC_CTRL_ST_PREDICTORS : BitFieldWitdhEDX(27, 27);
	uint32_t L1D_FLUSH_INTERFACE : BitFieldWitdhEDX(28, 28);
	uint32_t ARCH_CAPABILITIES : BitFieldWitdhEDX(29, 29);
	uint32_t CORE_CAPABILITIES : BitFieldWitdhEDX(30, 30);
	uint32_t SPEC_CTRL_SSBD : BitFieldWitdhEDX(31, 31);

}CPUIDLeaf0x07SubLeaf0;
typedef struct {
	//EAX
	uint32_t SHA512 : BitFieldWitdhEAX(0, 0);
	uint32_t SM3 : BitFieldWitdhEAX(1, 1);
	uint32_t SM4 : BitFieldWitdhEAX(2, 2);
uint32_t: BitFieldWitdhEAX(3, 3);
	uint32_t AVX_VNNI : BitFieldWitdhEAX(4, 4);
	uint32_t AVX5112_BF16 : BitFieldWitdhEAX(5, 5);
	uint32_t LASS : BitFieldWitdhEAX(6, 6);
	uint32_t CMPCCXADD : BitFieldWitdhEAX(7, 7);
	uint32_t ARCH_PERFMON_EXT : BitFieldWitdhEAX(8, 8);
uint32_t:BitFieldWitdhEAX(9, 9);
	uint32_t FAST_REP_MOVSB : BitFieldWitdhEAX(10, 10);
	uint32_t FAST_REP_STOSB : BitFieldWitdhEAX(11, 11);
	uint32_t FAST_REP_CMPSB_SCASB : BitFieldWitdhEAX(12, 12);
uint32_t: BitFieldWitdhEAX(18, 13);
	uint32_t WRMSRNS : BitFieldWitdhEAX(19, 19);
uint32_t:BitFieldWitdhEAX(20, 20);
	uint32_t AMX_FP16 : BitFieldWitdhEAX(21, 21);
	uint32_t HRESET : BitFieldWitdhEAX(22, 22);
	uint32_t AVX_IFMA : BitFieldWitdhEAX(23, 23);
uint32_t:BitFieldWitdhEAX(25, 24);
	uint32_t LAM : BitFieldWitdhEAX(26, 26);
	uint32_t MSRLIST : BitFieldWitdhEAX(27, 27);
uint32_t:BitFieldWitdhEAX(29, 28);
	uint32_t INVD_DISABLE_POST_BIOS_DONE : BitFieldWitdhEAX(30, 30);
uint32_t: BitFieldWitdhEAX(31, 31);
	//EBX
	uint32_t PPIN : BitFieldWitdhEBX(0, 0);
uint32_t: BitFieldWitdhEBX(2, 1);
	uint32_t CPUIDMAXVAL_LIM_RMV : BitFieldWitdhEBX(3, 3);
uint32_t:BitFieldWitdhEBX(31, 4);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t:BitFieldWitdhEDX(3, 0);
	uint32_t AVX_VNNI_INT8 : BitFieldWitdhEDX(4, 4);
	uint32_t AVX_NE_CONVERT : BitFieldWitdhEDX(5, 5);
uint32_t: BitFieldWitdhEDX(9, 6);
	uint32_t AVX_VNNI_INT16 : BitFieldWitdhEDX(10, 10);
uint32_t: BitFieldWitdhEDX(13, 11);
	uint32_t PREFETCHI : BitFieldWitdhEDX(14, 14);
uint32_t: BitFieldWitdhEDX(16, 15);
	uint32_t UIRET_UIF : BitFieldWitdhEDX(17, 17);
	uint32_t CET_SSS : BitFieldWitdhEDX(18, 18);
	uint32_t AVX10 : BitFieldWitdhEDX(19, 19);
uint32_t :BitFieldWitdhEDX(23, 20);
	uint32_t SLSM : BitFieldWitdhEDX(24, 24);
uint32_t: BitFieldWitdhEDX(31, 25);

} CPUIDLeaf0x07SubLeaf1;
typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
	uint32_t PSFD : BitFieldWitdhEDX(0, 0);
	uint32_t IPRED_CTRL : BitFieldWitdhEDX(1, 1);
	uint32_t RRSBA_CTRL : BitFieldWitdhEDX(2, 2);
	uint32_t DDPD_U : BitFieldWitdhEDX(3, 3);
	uint32_t BHI_CTRL : BitFieldWitdhEDX(4, 4);
	uint32_t MCDT_NO : BitFieldWitdhEDX(5, 5);
	uint32_t UC_LOCK_DISABLE : BitFieldWitdhEDX(6, 6);
	uint32_t MONITOR_MITG_NO : BitFieldWitdhEDX(7, 7);
uint32_t:BitFieldWitdhEDX(31, 8);
} CPUIDLeaf0x07SubLeaf2;
typedef struct {
	uint32_t storeage[4];
} CPUIDLeaf0x07SubLeafGENEREIC;
typedef struct {
	CPUIDLeaf0x07SubLeaf0 subleaf0;
	CPUIDLeaf0x07SubLeaf1 subleaf1;
	CPUIDLeaf0x07SubLeaf2 subleaf2;
	CPUIDLeaf0x07SubLeafGENEREIC GenereicSubLeafs[5];
} CPUIDLeaf0x07;

typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);
	
} CPUIDLeaf0x08;

typedef struct {
	//EAX
	uint32_t PLATFORM_DCA_CAP : BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);

} CPUIDLeaf0x09;

typedef struct {
	//EAX
	uint32_t VERSION : BitFieldWitdhEAX(7, 0);
	uint32_t NUM_GP_CTRS : BitFieldWitdhEAX(15, 8);
	uint32_t GP_CTR_WIDTH : BitFieldWitdhEAX(23, 16);
	uint32_t EVENT_ENUM_LENGTH : BitFieldWitdhEAX(31, 24);
	//EBX
	uint32_t CORE_CYC_NA : BitFieldWitdhEBX(0, 0);	
	uint32_t INTR_RET_NA : BitFieldWitdhEBX(1, 1);
	uint32_t REF_CYC_NA : BitFieldWitdhEBX(2, 2);
	uint32_t LLC_CYC_NA : BitFieldWitdhEBX(3, 3);
	uint32_t LLC_MISSES_NA : BitFieldWitdhEBX(4, 4);
	uint32_t BR_INSTR_RET_NA : BitFieldWitdhEBX(5, 5);
	uint32_t BR_MISPRED_RET_NA : BitFieldWitdhEBX(6, 6);
	uint32_t SLOTS_NA : BitFieldWitdhEBX(7, 7);
uint32_t: BitFieldWitdhEBX(9, 8);
	uint32_t FRONTEND_NA : BitFieldWitdhEBX(10, 10);
	uint32_t RETIRING_NA : BitFieldWitdhEBX(11, 11);
	uint32_t LBR_INSERTS_NA : BitFieldWitdhEBX(12, 12);
uint32_t: BitFieldWitdhEBX(31, 13);
//ECX
	uint32_t FIXED_CTR_MASK : BitFieldWitdhECX(31, 0);
	//EDX
	uint32_t NUM_FIXED_CTRS : BitFieldWitdhEDX(4, 0);
	uint32_t FIXED_CTR_WIDTH : BitFieldWitdhEDX(12, 5);
uint32_t:BitFieldWitdhEDX(14, 13);
	uint32_t ANYTHREAD_DEPRECATION : BitFieldWitdhEDX(15, 15);
	uint32_t SLOTS_PER_CYC : BitFieldWitdhEDX(19, 16);
uint32_t: BitFieldWitdhEDX(31, 20);

} CPUIDLeaf0x0A;
typedef struct {
	//EAX
	uint32_t SHIFT_COUNT : BitFieldWitdhEAX(4, 0);
uint32_t:BitFieldWitdhEAX(31, 5);
	//EBX
	uint32_t LOCAL_NEXT_LEVEL_NUM_LP : BitFieldWitdhEBX(15, 0);
uint32_t: BitFieldWitdhEBX(31, 16);
	//ECX
	uint32_t LEVEL_NUM : BitFieldWitdhECX(7, 0);
	uint32_t DOMAIN_TYPE : BitFieldWitdhECX(15, 8);
uint32_t: BitFieldWitdhECX(31, 16);
	//EDX
	uint32_t LOCAL_X2APIC_ID : BitFieldWitdhEDX(31, 0);

}CPUIDLeaf0x0BSubLeaf;
typedef struct {
	CPUIDLeaf0x0BSubLeaf* subleafs;
uint64_t:BitfieldWidth(63, 0);
} CPUIDLeaf0x0B;
typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x0C;

typedef struct {
	//EAX
	uint32_t X87 : BitFieldWitdhEAX(0, 0);
	uint32_t SSE : BitFieldWitdhEAX(1, 1);
	uint32_t AVX : BitFieldWitdhEAX(2, 2);
	uint32_t MPX_BNDREGS : BitFieldWitdhEAX(3, 3);
	uint32_t MPX_BNDCSR : BitFieldWitdhEAX(4, 4);
	uint32_t AVX512_OPMASK : BitFieldWitdhEAX(5, 5);
	uint32_t AVX512_ZMM_HI256 : BitFieldWitdhEAX(6, 6);
	uint32_t AVX512_HI16_ZMM : BitFieldWitdhEAX(7, 7);
	uint32_t PT : BitFieldWitdhEAX(8, 8);
	uint32_t PKRU : BitFieldWitdhEAX(9, 9);
	uint32_t PASID : BitFieldWitdhEAX(10, 10);
	uint32_t CET_U : BitFieldWitdhEAX(11, 11);
	uint32_t CET_S : BitFieldWitdhEAX(12, 12);
	uint32_t HDC : BitFieldWitdhEAX(13, 13);
	uint32_t UINTR : BitFieldWitdhEAX(14, 14);
	uint32_t LBR : BitFieldWitdhEAX(15, 15);
	uint32_t HWP : BitFieldWitdhEAX(16, 16);
	uint32_t AMX_TILECFG : BitFieldWitdhEAX(17, 17);
	uint32_t AMX_TILEDATA : BitFieldWitdhEAX(18, 18);
uint32_t: BitFieldWitdhEAX(31, 19);
	//EBX
	uint32_t XSAVE_BYTES_ENABLED_FEATURES : BitFieldWitdhEBX(31, 0);
	//ECX
	uint32_t XSAVE_BYTES_SUPPORTED_FEATURES : BitFieldWitdhECX(31, 0);
	//EDX
	uint32_t VALID_XCR0_UPPER_32 : BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x0DSubldeaf0;

typedef struct {
	//EAX
	uint32_t XSAVEOPT : BitFieldWitdhEAX(0, 0);
	uint32_t XSAVEC : BitFieldWitdhEAX(1, 1);
	uint32_t XGETBV1 : BitFieldWitdhEAX(2, 2);
	uint32_t XSAVES : BitFieldWitdhEAX(3, 3);
	uint32_t XFD : BitFieldWitdhEAX(4, 4);
uint32_t: BitFieldWitdhEAX(31, 5);
	//EBX
	uint32_t XSAVEOPT_BYTES_ENABLED_FEATURES : BitFieldWitdhEBX(31, 0);
	//ECX
	uint32_t X87 : BitFieldWitdhECX(0, 0);
	uint32_t SSE : BitFieldWitdhECX(1, 1);
	uint32_t AVX : BitFieldWitdhECX(2, 2);
	uint32_t MPX_BNDREGS : BitFieldWitdhECX(3, 3);
	uint32_t MPX_BNDCSR : BitFieldWitdhECX(4, 4);
	uint32_t AVX512_OPMASK : BitFieldWitdhECX(5, 5);
	uint32_t AVX512_ZMM_HI256 : BitFieldWitdhECX(6, 6);
	uint32_t AVX512_HI16_ZMM : BitFieldWitdhECX(7, 7);
	uint32_t PT : BitFieldWitdhECX(8, 8);
	uint32_t PKRU : BitFieldWitdhECX(9, 9);
	uint32_t PASID : BitFieldWitdhECX(10, 10);
	uint32_t CET_U : BitFieldWitdhECX(11, 11);
	uint32_t CET_S : BitFieldWitdhECX(12, 12);
	uint32_t HDC : BitFieldWitdhECX(13, 13);
	uint32_t UINTR : BitFieldWitdhECX(14, 14);
	uint32_t LBR : BitFieldWitdhECX(15, 15);
	uint32_t HWP : BitFieldWitdhECX(16, 16);
	uint32_t TILECFG : BitFieldWitdhECX(17, 17);
	uint32_t TILEDATA : BitFieldWitdhECX(18, 18);
uint32_t: BitFieldWitdhECX(31, 19);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);


}CPUIDLeaf0x0DSubLeaf1;

typedef struct {
	//EAX
	uint32_t COMP_SIZE : BitFieldWitdhEAX(31, 0);
	//EBX
	uint32_t COMP_OFFSET : BitFieldWitdhEBX(31, 0);
	//ECX
	uint32_t COMP_SUP : BitFieldWitdhECX(0, 0);
	uint32_t COMP_64B_ALIGNED : BitFieldWitdhECX(1, 1);
	uint32_t COMP_XFD : BitFieldWitdhECX(2, 2);
uint32_t: BitFieldWitdhECX(31, 3);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);

} CPUIDLeaf0x0DSubLeafGeneric;



typedef struct {
	CPUIDLeaf0x0DSubldeaf0 subleaf0;
	CPUIDLeaf0x0DSubLeaf1 subleaf1;
	CPUIDLeaf0x0DSubLeafGeneric* otherLeafs;
uint64_t: BitfieldWidth(63, 0);
} CPUIDLeaf0x0D;

typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x0E;

typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
	uint32_t MAX_RMID : BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(0, 0);
	uint32_t CMT_L3 : BitFieldWitdhEDX(1, 1);
uint32_t: BitFieldWitdhEDX(31, 2);
}CPUIDLeaf0x0FSubLeaf0;

typedef struct {
	//EAX
	uint32_t CTR_WIDTH : BitFieldWitdhEAX(7, 0);
	uint32_t RDT_M_OVE : BitFieldWitdhEAX(8, 8);
	uint32_t IO_QOS_CMT : BitFieldWitdhEAX(9, 9);
	uint32_t IO_QOS_MBM : BitFieldWitdhEAX(10, 10);
uint32_t:BitFieldWitdhEAX(31, 11);
	//EBX
	uint32_t CONV_FACTOR : BitFieldWitdhEBX(31, 0);
	//ECX
	uint32_t MAX_RMID_L3 : BitFieldWitdhECX(31, 0);
	//EDX
	uint32_t CMT_L3_OCCUP : BitFieldWitdhEDX(0, 0);
	uint32_t CMT_L3_TOTAL : BitFieldWitdhEDX(1, 1);
	uint32_t CMT_L3_LOCAL : BitFieldWitdhEDX(2, 2);
uint32_t: BitFieldWitdhEDX(31, 3);
}CPUIDLeaf0x0FSubLeaf1;

typedef struct {
	uint32_t storage[4];
}CPUIDLeaf0x0FSubLeafGeneric;

typedef struct {
	CPUIDLeaf0x0FSubLeaf0 subleaf0;
	CPUIDLeaf0x0FSubLeaf1 subleaf1;
	CPUIDLeaf0x0FSubLeafGeneric GenereicSubLeafs[6];
} CPUIDLeaf0x0F;

typedef struct {
	//EAX
uint32_t:BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t:BitFieldWitdhEBX(0, 0);
	uint32_t CAT_L3 : BitFieldWitdhEBX(1, 1);
	uint32_t CAT_L2 : BitFieldWitdhEBX(2, 2);
	uint32_t MBA : BitFieldWitdhEBX(3, 3);
uint32_t:BitFieldWitdhEBX(31, 4);
	//ECX
uint32_t:BitFieldWitdhECX(31, 0);
	//EDX
uint32_t:BitFieldWitdhEDX(31, 0);
}CPUIDLeaf0x10SubLeaf0;

typedef struct {
	//EAX
	uint32_t CAT_L3_BITMASK_LENGTH : BitFieldWitdhEAX(4, 0);
uint32_t: BitFieldWitdhEAX(31, 5);
	//EBX
	uint32_t CAT_L3_MAP : BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(0, 0);
	uint32_t CAT_L3_NONCPU : BitFieldWitdhECX(1, 1);
	uint32_t CAT_L3_CDP : BitFieldWitdhECX(2, 2);
	uint32_t CAT_L3_NONCONTIG : BitFieldWitdhECX(3, 3);
uint32_t:BitFieldWitdhECX(31, 4);
	//EDX
	uint32_t CAT_L3_MAX_CLOS : BitFieldWitdhEDX(15, 0);
uint32_t: BitFieldWitdhEDX(31, 16);
}CPUIDLeaf0x10SubLeaf1;

typedef struct {
	//EAX
	uint32_t CAT_L2_BITMASK_LENGTH : BitFieldWitdhEAX(4, 0);
uint32_t: BitFieldWitdhEAX(31, 5);
	//EBX
	uint32_t CAT_L2_MAP : BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(1, 0);
	uint32_t CAT_L2_CDP : BitFieldWitdhECX(2, 2);
	uint32_t CAT_L2_NONCONTIG : BitFieldWitdhECX(3, 3);
uint32_t: BitFieldWitdhECX(31, 4);
	//EDX
	uint32_t CAT_L2_MAX_CLOS : BitFieldWitdhEDX(15, 0);
uint32_t:BitFieldWitdhEDX(31, 16);
} CPUIDLeaf0x10SubLeaf2;

typedef struct {
	//EAX
	uint32_t MBA_MAX : BitFieldWitdhEAX(11, 0);
uint32_t: BitFieldWitdhEAX(31, 12);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
	uint32_t PER_THREAD_MBA : BitFieldWitdhECX(0, 0);
uint32_t:BitFieldWitdhECX(1, 1);
	uint32_t MBA_LINEAR : BitFieldWitdhECX(2, 2);
uint32_t:BitFieldWitdhECX(31, 3);
	//EDX
	uint32_t MBA_MAX_CLOS : BitFieldWitdhEDX(15, 0);
uint32_t: BitFieldWitdhEDX(31, 16);
}CPUIDLeaf0x10SubLeaf3;

typedef struct {
	uint32_t storage[4];
}CPUIDLeaf0x10SubLeafGeneric;
typedef struct {
	CPUIDLeaf0x10SubLeaf0 subLeaf0;
	CPUIDLeaf0x10SubLeaf1 subLeaf1;
	CPUIDLeaf0x10SubLeaf2 subLeaf2;
	CPUIDLeaf0x10SubLeaf3 subLeaf3;
	CPUIDLeaf0x10SubLeafGeneric genericLeafs[4];
} CPUIDLeaf0x10;

typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x11;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x12;

typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x13;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x14;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x15;

typedef struct {
	//EAX
	uint32_t LOCAL_PROCESSOR_BASE_FREQUENCY : BitFieldWitdhEAX(15, 0);
uint32_t: BitFieldWitdhEAX(31, 16);
	//EBX
	uint32_t LOCAL_MAXIMUM_FREQUENCY : BitFieldWitdhEBX(15, 0);
uint32_t: BitFieldWitdhEBX(31, 16);
	//ECX
	uint32_t LOCAL_BUS_FREQUENCY : BitFieldWitdhECX(15, 0);
uint32_t: BitFieldWitdhECX(31, 16);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x16;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x17;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x18;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x19;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x1A;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x1B;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x1C;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x1D;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x1E;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x1F;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x20;

typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x21;

typedef struct {
	//EAX
uint32_t: BitFieldWitdhEAX(31, 0);
	//EBX
uint32_t: BitFieldWitdhEBX(31, 0);
	//ECX
uint32_t: BitFieldWitdhECX(31, 0);
	//EDX
uint32_t: BitFieldWitdhEDX(31, 0);
} CPUIDLeaf0x22;

typedef struct {
	uint32_t tmp;
} CPUIDLeaf0x23;

typedef struct {
	//EAX
	uint32_t MAX_SUBLEAF : BitFieldWitdhEAX(31, 0);
	//EBX
	uint32_t VECTOR_ISA_VERSION : BitFieldWitdhEBX(7, 0);
uint32_t:BitFieldWitdhEBX(31, 8);
	//ECX
uint32_t:BitFieldWitdhECX(31, 0);
	//EDX
uint32_t:BitFieldWitdhEDX(31, 0);

}CPUIDLeaf0x24Subleaf;
typedef struct {
	CPUIDLeaf0x24Subleaf* Subleafs;
uint64_t: BitfieldWidth(63, 0);
} CPUIDLeaf0x24;


typedef struct {
	CPUIDLeaf0x00 leaf0;
	CPUIDLeaf0x01 leaf1;
	CPUIDLeaf0x02 leaf2;
	CPUIDLeaf0x03 leaf3;
	CPUIDLeaf0x04 leaf4;
	CPUIDLeaf0x05 leaf5;
	CPUIDLeaf0x06 leaf6;
	CPUIDLeaf0x07 leaf7;
	CPUIDLeaf0x08 leaf8;
	CPUIDLeaf0x09 leaf9;
	CPUIDLeaf0x0A leaf0A;
	CPUIDLeaf0x0B leaf0B;
	CPUIDLeaf0x0C leaf0C;
	CPUIDLeaf0x0D leaf0D;
	CPUIDLeaf0x0E leaf0E;
	CPUIDLeaf0x0F leaf0F;
	CPUIDLeaf0x10 leaf10;
	CPUIDLeaf0x11 leaf11;
	CPUIDLeaf0x12 leaf12;
	CPUIDLeaf0x13 leaf13;
	CPUIDLeaf0x14 leaf14;
	CPUIDLeaf0x15 leaf15;
	CPUIDLeaf0x16 leaf16;
	CPUIDLeaf0x17 leaf17;
	CPUIDLeaf0x18 leaf18;
	CPUIDLeaf0x19 leaf19;
	CPUIDLeaf0x1A leaf1A;
	CPUIDLeaf0x1B leaf1B;
	CPUIDLeaf0x1C leaf1C;
	CPUIDLeaf0x1D leaf1D;
	CPUIDLeaf0x1E leaf1E;
	CPUIDLeaf0x1F leaf1F;
	CPUIDLeaf0x20 leaf20;
	CPUIDLeaf0x21 leaf21;
	CPUIDLeaf0x22 leaf22;
	CPUIDLeaf0x23 leaf23;
	CPUIDLeaf0x24 leaf24;
}CPUIDLeafs;








