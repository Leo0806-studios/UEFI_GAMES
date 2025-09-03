/*
* reimplementation of msvc++ eh data structures and related code
*
*/

//#pragma once
//#pragma warning(push)
//#pragma warning (disable:4820)
//#if defined(_M_CEE_PURE) || defined(BUILDING_C1XX_FORCEINCLUDE)
//#define _EH_RELATIVE_TYPEINFO 0
//#define _EH_RELATIVE_FUNCINFO 0
//#define _RTTI_RELATIVE_TYPEINFO 0
//#elif defined(_CHPE_X86_ARM64_EH_)
//#define _EH_RELATIVE_TYPEINFO 0
//#define _EH_RELATIVE_FUNCINFO 1
//#define _RTTI_RELATIVE_TYPEINFO 0
//#elif defined(_M_ARM)
//#define _EH_RELATIVE_TYPEINFO 1
//#define _EH_RELATIVE_FUNCINFO 1
//#define _RTTI_RELATIVE_TYPEINFO 0
//#elif defined(_M_X64) || defined(_M_ARM64)
//#define _EH_RELATIVE_TYPEINFO 1
//#define _EH_RELATIVE_FUNCINFO 1
//#define _RTTI_RELATIVE_TYPEINFO 1
//#else
//#define _EH_RELATIVE_TYPEINFO 0
//#define _EH_RELATIVE_FUNCINFO 0
//#define _RTTI_RELATIVE_TYPEINFO 0
//#endif
//#include <stdint.h>
//#include "STRING.h"
////
//// PMD - Pointer to Member Data: generalized pointer-to-member descriptor
////
//typedef struct PMD //NOSONAR
//{
//	int	mdisp;	// Offset of intended data within base
//	int	pdisp;	// Displacement to virtual base pointer
//	int	vdisp;	// Index within vbTable to offset of base
//} PMD;
//
//
////
//// PMFN - Pointer to Member Function
////
//#if _EH_RELATIVE_TYPEINFO
//typedef	int	PMFN;					// Image relative offset of Member Function //NOSONAR
//#else
//typedef void(__cdecl* PMFN)(void*);
//#endif
//
//typedef void* (__stdcall* PGETWINRT_OOM_EXCEPTION)();
//
//
//
//
////
//// TypeDescriptor - per-type record which uniquely identifies the type.
////
//// Each type has a decorated name which uniquely identifies it, and a hash
//// value which is computed by the compiler.  The hash function used is not
//// important; the only thing which is essential is that it be the same for
//// all time.
////
//// The special type '...' (ellipsis) is represented by a null name.
////
//#pragma warning(push)
//#pragma warning(disable:4200)	// nonstandard extension used: array of runtime bound
//
//#if defined(_M_X64) || defined(_M_ARM64) || defined(BUILDING_C1XX_FORCEINCLUDE)
//#pragma pack(push, TypeDescriptor, 8)
//#endif
//
//typedef struct TypeDescriptor //NOSONAR
//{
//#if defined(_WIN64) || defined(_RTTI) || defined(BUILDING_C1XX_FORCEINCLUDE)
//	const void* pVFTable;	// Field overloaded by RTTI
//#else
//	unsigned long	hash;			// Hash value computed from type's decorated name
//#endif
//	void* spare;			// reserved, possible for RTTI
//	char			name[];			// The decorated name of the type; 0 terminated. //NOSONAR
//} TypeDescriptor;
//
//#if defined(_M_X64) || defined(_M_ARM64) || defined(BUILDING_C1XX_FORCEINCLUDE)
//#pragma pack(pop, TypeDescriptor)
//#endif
//#pragma warning(pop)
//
//
///////////////////////////////////////////////////////////////////////////////
////
//// Description of the thrown object.
////
//// This information is broken down into three levels, to allow for maximum
//// comdat folding (at the cost of some extra pointers).
////
//// ThrowInfo is the head of the description, and contains information about
//// 				the particular variant thrown.
//// CatchableTypeArray is an array of pointers to type descriptors.  It will
////				be shared between objects thrown by reference but with varying
////				qualifiers.
//// CatchableType is the description of an individual type, and how to effect
////				the conversion from a given type.
////
////---------------------------------------------------------------------------
//
//
////
//// CatchableType - description of a type that can be caught.
////
//// Note:  although isSimpleType can be part of ThrowInfo, it is more
////		  convenient for the run-time to have it here.
////
//typedef const struct _s_CatchableType {//NOSONAR
//	unsigned int		properties;			// Catchable Type properties (Bit field)
//#if _EH_RELATIVE_TYPEINFO
//	int					pType;				// Image relative offset of TypeDescriptor
//#else
//	TypeDescriptor* pType;				// Pointer to the type descriptor for this type
//#endif
//	PMD 				thisDisplacement;	// Pointer to instance of catch type within thrown object.
//	int					sizeOrOffset;		// Size of simple-type object or offset into
//	//  buffer of 'this' pointer for catch object
//	PMFN				copyFunction;		// Copy constructor or CC-closure
//} CatchableType;
//
//
//
////
//// CatchableTypeArray - array of pointers to catchable types, with length
////
//#pragma warning (push)
//#pragma warning (disable:4200)	// nonstandard extension used: array of runtime bound
//typedef const struct _s_CatchableTypeArray {//NOSONAR
//	int	nCatchableTypes;
//#if _EH_RELATIVE_TYPEINFO
//	int				arrayOfCatchableTypes[];	// Image relative offset of Catchable Types //NOSONAR//NOLINT
//#else
//	CatchableType* arrayOfCatchableTypes[];
//#endif
//} CatchableTypeArray;
//#pragma warning (pop)
//
//
//
//
//
////
//// ThrowInfo - information describing the thrown object, statically built
//// at the throw site.
////
//// pExceptionObject (the dynamic part of the throw; see below) is always a
//// reference, whether or not it is logically one.  If 'isSimpleType' is true,
//// it is a reference to the simple type, which is 'size' bytes long.  If
//// 'isReference' and 'isSimpleType' are both false, then it's a UDT or
//// a pointer to any type (i.e. pExceptionObject points to a pointer).  If it's
//// a pointer, copyFunction is NULL, otherwise it is a pointer to a copy
//// constructor or copy constructor closure.
////
//// The pForwardCompat function pointer is intended to be filled in by future
//// versions, so that if say a DLL built with a newer version (say C10) throws,
//// and a C9 frame attempts a catch, the frame handler attempting the catch (C9)
//// can let the version that knows all the latest stuff do the work.
////
//typedef const struct _s_ThrowInfo {//NOSONAR
//	unsigned int	attributes;							// Throw Info attributes (Bit field)
//	PMFN			pmfnUnwind;							// Destructor to call when exception has been handled or aborted
//#if _EH_RELATIVE_TYPEINFO && !defined(BUILDING_C1XX_FORCEINCLUDE)
//	int				pForwardCompat;						// Image relative offset of Forward compatibility frame handler
//	int				pCatchableTypeArray;				// Image relative offset of CatchableTypeArray
//#else
//	int(__cdecl* pForwardCompat)(...);				// Forward compatibility frame handler
//	CatchableTypeArray* pCatchableTypeArray;			// Pointer to list of pointers to types
//#endif
//} ThrowInfo;

#pragma message(__FILE__ " never include this with <utility> direct or indriect as it will triger odr violations.")//NOLINT
#ifdef _UTILITY_
#error "cant be included wiht <utility> as this contains a smal stub and would trigger odr violations"
#endif // _UTILITY_
#define _UTILITY_
#ifdef __clang__
struct _ThrowInfo;//dummy forward declaration to make clangtidy happy
#endif // __clang__

#include "STRING.h"
#include <ehdata_forceinclude.h>//NOLINT(llvmlibc-restrict-system-libc-headers)
#include <intrin.h>//NOLINT(llvmlibc-restrict-system-libc-headers)
#include <stdint.h>//NOLINT(llvmlibc-restrict-system-libc-headers)

namespace std {
	//minimal type stub so that the using in the next header is satisfied
	 template <class _Ty1, class _Ty2>
	 struct pair {};// store a pair of values

}// namespace std

enum unwind_info_flags : unsigned char { //NOSONAR
	UNW_FLAG_NHANDLER = 0b000,
	UNW_FLAG_EHANDLER = 0b001,
	UNW_FLAG_UHANDLER = 0b010,
	UNW_FLAG_CHAININFO = 0b100
};

using RVA = uint32_t;
using Register = size_t;
 struct _RUNTIME_FUNCTION {//NOSONAR
	RVA BeginAddress;		// The address of the first instruction in the function
	RVA EndAddress;		// The address of the first instruction after the function
	RVA UnwindData;		// The address of the unwind data for this function
} ;


 struct UNWIND_CODE {//NOSONAR
	//NOSONAR
	unsigned char code_offset; // offset in prolog where the operation is located
	unsigned char unwind_op : 4; // the operation to be performed
	unsigned char op_info : 4; // additional information about the operation
} ;
using UBYTE = unsigned char;
namespace FH4 {
	struct FuncInfo4;
}//namespace FH4
#pragma warning(push)
#pragma warning (disable:4820)
/// <summary>
/// this is not an packed representation but an in memory representation as i cant model the VLA in c++
/// </summary>
 struct _UNWIND_INFO { //NOSONAR
	UBYTE version : 3;
	UBYTE flags : 5;
	UBYTE size_of_prolog;
	UBYTE count_of_codes;
	UBYTE frame_register : 4;
	UBYTE frame_offset : 4;
	UNWIND_CODE* unwind_code_array;//inline vla
	union flagDependant {
		RVA exceptionHandler; // RVA to exception handler 
		RVA chainedFunction; // RVA to next function unwind info 
	}flag_dependant;//NOSONAR
	FH4::FuncInfo4* optionalData; // optional data//variable elngth
};


#pragma warning(pop)

 struct RUNTIME_FUNCTION {
	 RVA BeginAddress;
	 RVA EndAddress;
	 RVA UnwindData;  // RVA to .xdata entry
 };
 using PRUNTIME_FUNCTION = RUNTIME_FUNCTION*;
struct CONTEXT64 {
	// minimal fields you'll actually use; or use Windows CONTEXT if you can.
	Register Rip, Rsp, Rbp; //NOSONAR
	Register Rbx, Rsi, Rdi, R12, R13, R14, R15; // non-volatiles //NOSONAR
	// ... XMM non-volatiles if you care, flags, etc.
};

/*


//############################### LAnguage Specific EH data structures #####################################
#define assert_ehdata(a) if (!(a)) { abort(); }



namespace EHD4 {
	//using int8_t = signed __int8;
	//using uint8_t = unsigned __int8;
	//using uint32_t = unsigned __int32;
	//using int32_t = signed __int32;
	//using uintptr_t = unsigned __int64;
	//using ptrdiff_t = signed __int64;
	using __ehstate_t = int32_t;
	constexpr inline uint32_t MAX_CONT_ADDRESSES = 2;

	constexpr inline int8_t s_negLengthTab[16] =
	{
		-1,    // 0
		-2,    // 1
		-1,    // 2
		-3,    // 3
		-1,    // 4
		-2,    // 5
		-1,    // 6
		-4,    // 7
		-1,    // 8
		-2,    // 9
		-1,    // 10
		-3,    // 11
		-1,    // 12
		-2,    // 13
		-1,    // 14
		-5,    // 15
	};
	constexpr inline uint8_t s_shiftTab[16] =
	{
		32 - 7 * 1,    // 0
		32 - 7 * 2,    // 1
		32 - 7 * 1,    // 2
		32 - 7 * 3,    // 3
		32 - 7 * 1,    // 4
		32 - 7 * 2,    // 5
		32 - 7 * 1,    // 6
		32 - 7 * 4,    // 7
		32 - 7 * 1,    // 8
		32 - 7 * 2,    // 9
		32 - 7 * 1,    // 10
		32 - 7 * 3,    // 11
		32 - 7 * 1,    // 12
		32 - 7 * 2,    // 13
		32 - 7 * 1,    // 14
		0,             // 15
	};

	struct UnwindMapEntry4 {
		enum Type {
			NoUW = 0b00,
			DtorWithObj = 0b01,
			DtorWithPtrToObj = 0b10,
			RVA = 0b11
		};
		uint32_t        nextOffset;
		Type            type;
		int32_t         action;
		uint32_t        object;
	};


	struct HandlerTypeHeader {
		enum contType {
			NONE = 0b00,
			ONE = 0b01,
			TWO = 0b10,
			RESERVED = 0b11
		};
#pragma warning(push)
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
		union {
			struct {
				uint8_t adjectives : 1;
				uint8_t dispType : 1;
				uint8_t dispCatchObj : 1;
				uint8_t contIsRVA : 1;
				uint8_t contAddr : 2;
				uint8_t unused : 2;
			};
#pragma warning(pop)
			uint8_t value;
		};
		HandlerTypeHeader()
			: value(0) {
		}
	};

	static_assert(sizeof(HandlerTypeHeader) == sizeof(uint8_t), "Size of HandlerTypeHeader not 1 Byte");
	struct HandlerType4 {
	public:
		HandlerTypeHeader header{};
		uint32_t          adjectives=0;
		int32_t           dispType=0;
		uint32_t          dispCatchObj=0;
		int32_t           dispOfHandler=0;
		uintptr_t         continuationAddress[MAX_CONT_ADDRESSES];//NOSONAR
		void reset() {
			header.value = 0;
			adjectives = 0;
			dispType = 0;
			dispCatchObj = 0;
			dispOfHandler = 0;
			memset(continuationAddress, 0, sizeof(continuationAddress));
		}
		HandlerType4() {
			memset(continuationAddress, 0, sizeof(continuationAddress));
		}
	};

	struct TryBlockMapEntry4 {
		uint32_t tryLow;
		uint32_t tryHigh;
		uint32_t catchHigh;
		int32_t  dispHandlerArray;
	};
	struct IPtoStateMapEntry4 {
		uint32_t Ip;
		int32_t  State;
	};
	struct SetIPtoStateMapEntry4
	{
		int32_t addrStartRVA;
		int32_t dispOfIPMap;

	};



	struct FuncInfo4Header {
#pragma warning(push)
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union

		union {
			struct {
				uint8_t isCatch : 1;  // 1 if this represents a catch funclet, 0 otherwise
				uint8_t isSeparated : 1;  // 1 if this function has separated code segments, 0 otherwise
				uint8_t BBT : 1;  // Flags set by Basic Block Transformations
				uint8_t UnwindMap : 1;  // Existence of Unwind Map RVA
				uint8_t TryBlockMap : 1;  // Existence of Try Block Map RVA
				uint8_t EHs : 1;  // EHs flag set
				uint8_t NoExcept : 1;  // NoExcept flag set
				uint8_t reserved : 1;  // Have to be 0
			};
#pragma warning(pop)
			uint8_t value;
		};
		FuncInfo4Header()
			: value(0) {
		}
	};



	static_assert(sizeof(FuncInfo4Header) == sizeof(uint8_t), "Size of FuncInfo4Header not 1 Byte");
	struct FuncInfo4 {
	public:
		FuncInfo4Header   header;
		uint32_t            bbtFlags;
		int32_t             dispUnwindMap;       // rva to unwind map
		int32_t             dispTryBlockMap;     // rva to try block map
		int32_t             dispIPtoStateMap;    // rva to ip to state map
		uint32_t            dispFrame;           // displacement of address of function frame wrt establisher frame, only used for catch funclets
		FuncInfo4()
			: header(),
			bbtFlags(0),
			dispUnwindMap(0),
			dispTryBlockMap(0),
			dispIPtoStateMap(0),
			dispFrame(0) {
		}
	};



	inline int32_t ReadInt(uint8_t** buffer) {
		__assume(buffer != nullptr);
		const int value = *(reinterpret_cast<int*>(*buffer));
		*buffer += sizeof(int32_t);//NOLINT//buffer is always valid as its part of the curently running image
		return value;
	}
	inline uint32_t ReadUnsigned(uint8_t** pbEncoding) {
		__assume(pbEncoding != nullptr);
		const auto lengthBits = static_cast<uint32_t>(**pbEncoding & 0x0FU);
#pragma warning(push)
#pragma warning(disable:4365)
		const size_t negLength = s_negLengthTab[lengthBits];//NOLINT
#pragma warning(pop)
		const uint32_t shift = s_shiftTab[lengthBits];
		uint32_t result = *(reinterpret_cast<uint32_t*>(*pbEncoding - negLength - 4));
		result >>= shift;
		*pbEncoding -= negLength;
		return result;
	}
	inline uint8_t* imageRelToByteBuffer(uintptr_t imageBase, int32_t disp) {
		return reinterpret_cast<uint8_t*>(imageBase + disp);
	}
	inline ptrdiff_t DecompFuncInfo(uint8_t* buffer, FuncInfo4& FuncInfoDe, uintptr_t imageBase, int32_t functionStart, bool rawIP2StateRVA) {
		uint8_t* buffer_start = buffer;
		FuncInfoDe.header.value = buffer[0];
		++buffer;
		if (FuncInfoDe.header.BBT) {
			FuncInfoDe.bbtFlags = ReadUnsigned(&buffer);
		}
		if (FuncInfoDe.header.UnwindMap) {
			FuncInfoDe.dispUnwindMap = ReadInt(&buffer);
		}
		if (FuncInfoDe.header.TryBlockMap) {
			FuncInfoDe.dispTryBlockMap = ReadInt(&buffer);
		}
		if (rawIP2StateRVA) {
			FuncInfoDe.dispIPtoStateMap = ReadInt(&buffer);
		}
		else {
			if (FuncInfoDe.header.isSeparated) {
				FuncInfoDe.dispIPtoStateMap = 0;
				int dispToSegMap = ReadInt(&buffer);
				if (dispToSegMap != 0) {
					uint8_t* segBuffer = imageRelToByteBuffer(imageBase, dispToSegMap);
					uint32_t numSegEntries = ReadUnsigned(&segBuffer);
					for (uint32_t i = 0; i < numSegEntries; i++)
					{
						int32_t segRVA = ReadInt(&segBuffer);
						int dispSegTable = ReadInt(&segBuffer);
						if (segRVA == functionStart)
						{
							FuncInfoDe.dispIPtoStateMap = dispSegTable;
							break;
						}
					}
				}
				else {
					__fastfail(UINT_FAST32_MAX);
				}
			}
			else {
				FuncInfoDe.dispIPtoStateMap = ReadInt(&buffer);
			}
		}
		if (FuncInfoDe.header.isCatch) {
			FuncInfoDe.dispFrame = ReadUnsigned(&buffer);
		}
		return buffer - buffer_start;
	}
	inline ptrdiff_t DecompFuncInfo(uint8_t* buffer, FuncInfo4& FuncInfoDe, uintptr_t imageBase, int32_t functionStart) {
		return DecompFuncInfo(buffer, FuncInfoDe, imageBase, functionStart, false);
	}



	class UWMap4 {
		uint32_t    _numEntries = 0;
		uint8_t* _bufferStart = nullptr;
		UnwindMapEntry4 _UWEntry{};

	public:
		UWMap4(const FuncInfo4* funcInfo, uintptr_t imageBase) {
			if (funcInfo->dispUnwindMap != 0) {
				uint8_t* buffer = imageRelToByteBuffer(imageBase, funcInfo->dispUnwindMap);
				_numEntries = ReadUnsigned(&buffer);
				_bufferStart = buffer;
			}
			else {
				//this might not be beccesary as id efault initialize to 0 and nullptr
				_numEntries = 0;
				_bufferStart = nullptr;
			}
		}
		class iterator {//NOLINT
			UWMap4& _UWMap;//NOLINT
			uint8_t* _currEntry;
		public:
			iterator(UWMap4& UWMap, uint8_t* currEntry) :_UWMap(UWMap), _currEntry(currEntry) {
			}
			iterator(const iterator& other) :_UWMap(other._UWMap), _currEntry(other._currEntry) {
			}
			void WalkBack() {
				_UWMap.WalkBack(&_currEntry);
			}
			iterator& operator++() {
				_UWMap.WalkForward(&_currEntry);
				return *this;
			}
			UnwindMapEntry4 operator*() {
				uint8_t* origOffset = _currEntry;
				_UWMap.ReadEntry(&_currEntry);
				_currEntry = origOffset;
				return _UWMap._UWEntry;
			}
			iterator& operator=(const iterator& other) {
				if (this != &other) {
					_UWMap = other._UWMap;
					_currEntry = other._currEntry;
				}
				return *this;
			}
			bool operator!=(const iterator& other) const {
				return _currEntry != other._currEntry;
			}
			bool operator>(const iterator& other) const {
				return _currEntry > other._currEntry;
			}
			bool operator >= (const iterator& other) const {
				return _currEntry >= other._currEntry;
			};
			ptrdiff_t operator-(const iterator& other) const {
				return _currEntry - other._currEntry;
			}
		};
		iterator begin() {
			return iterator(*this, _bufferStart);
		}
		iterator end() {
			uint8_t* currOffset = _bufferStart;
			for(uint32_t i = 0; i < _numEntries; i++) {
				ReadEntry(&currOffset);
			}
			return iterator(*this, currOffset);
		}
		void getStartStop(__ehstate_t start, __ehstate_t stop, iterator& startIter, iterator& stopIter) {
			uint8_t* startOffset = _bufferStart - 1;
			uint8_t* stopOffset = _bufferStart - 1;
			uint8_t* currOffset = _bufferStart;
			bool foundStart = false;
			bool foundStop = false;
			for (uint32_t i = 0; i < _numEntries; i++)
			{
				if (i == static_cast<uint32_t>(start)) {
					startOffset = currOffset;
					foundStart = true;
				}

				if (i == static_cast<uint32_t>(stop)) {
					stopOffset = currOffset;
					foundStop = true;
				}

				if (foundStart && foundStop) {
					break;
				}

				ReadEntry(&currOffset);
			}

			startIter = iterator(*this, startOffset);
			stopIter = iterator(*this, stopOffset);
		}
		static __ehstate_t getStateFromIterators(const iterator& lowStateIter, const __ehstate_t lowState,
			const iterator& highStateIter, const __ehstate_t highState, const iterator& toStateIter)
		{
			__ehstate_t state = -1;


			if (toStateIter > highStateIter || lowStateIter > toStateIter)
			{
				return state;
			}

			if ((toStateIter - lowStateIter) < (highStateIter - toStateIter))
			{
				iterator lowStateIterCopy = lowStateIter;
				state = lowState;
				while (toStateIter > lowStateIterCopy)
				{
					++lowStateIterCopy;
					++state;
				}
			}
			else
			{
				iterator toStateIterCopy = toStateIter;
				state = highState;

				while (highStateIter > toStateIterCopy)
				{
					++toStateIterCopy;
					--state;
				}
			}

			return state;
		}
		uint32_t getNumEntries()const  {
			return _numEntries;
		}
		private:
			void ReadEntry(uint8_t** currOffset)
			{
				_UWEntry.nextOffset = ReadUnsigned(currOffset);
				_UWEntry.type = static_cast<UnwindMapEntry4::Type>(_UWEntry.nextOffset & 0b11);
				_UWEntry.nextOffset >>= 2;

				if (_UWEntry.type == UnwindMapEntry4::Type::DtorWithObj || _UWEntry.type == UnwindMapEntry4::Type::DtorWithPtrToObj)
				{
					_UWEntry.action = ReadInt(currOffset);
					_UWEntry.object = ReadUnsigned(currOffset);
				}
				else if (_UWEntry.type == UnwindMapEntry4::Type::RVA)
				{
					_UWEntry.action = ReadInt(currOffset);
				}
			}

			void WalkBack(uint8_t** currOffset)
			{
				uint8_t* origOffset = *currOffset;

				ReadEntry(currOffset);

				*currOffset = origOffset - _UWEntry.nextOffset;
			}

			void WalkForward(uint8_t** currOffset)
			{
				ReadEntry(currOffset);
			}
	};
	class TryBlockMap4 {
		uint32_t _numTryBlocks = 0;
		uint8_t* _bufferStart = nullptr;
		uint8_t* _buffer = nullptr;
		TryBlockMapEntry4 _tryBlock{};
	public:
		TryBlockMap4(const FuncInfo4* pFuncInfo, uintptr_t imageBase)
		{
			if (pFuncInfo->dispTryBlockMap != 0)
			{
				_buffer = imageRelToByteBuffer(imageBase, pFuncInfo->dispTryBlockMap);
				_numTryBlocks = ReadUnsigned(&_buffer);
				_bufferStart = _buffer;
				DecompTryBlock();
			}
			else
			{
				_numTryBlocks = 0;
			}
		}
		class iterator {
			TryBlockMap4& _tryBlockMap;
			uint32_t _currBlock;
		public:
			iterator(TryBlockMap4& tryBlockMap,uint32_t currBlock):_tryBlockMap(tryBlockMap),_currBlock(currBlock){
			}
			iterator& operator++() {
				_currBlock++;
				_tryBlockMap.DecompTryBlock();
				return *this;
			}
			TryBlockMapEntry4 operator*() {
				return _tryBlockMap._tryBlock;
			}
			bool operator!=(const iterator& other) const {
				return _currBlock != other._currBlock;
			}
			bool operator<(const iterator& other) const {
				return _currBlock < other._currBlock;
			}
			iterator& operator=(const iterator& other) {
				if (this != &other) {
					_tryBlockMap = other._tryBlockMap;
					_currBlock = other._currBlock;
				}
				return *this;
			}
			void incrementToSentinel()
			{
				_currBlock++;
			}
		};
		iterator begin() {
			return iterator(*this, 0);
		}
		iterator end() {
			return iterator(*this, _numTryBlocks);
		}
		uint32_t getNumTryBlocks()const {
			return _numTryBlocks;
		}
		void setBuffer(iterator& iter) {
			_buffer = _bufferStart;
			DecompTryBlock();
			for (iterator start = begin(); start != iter; ++start)
			{
				DecompTryBlock();
			}
		}
	private:
		void DecompTryBlock()
		{
			_tryBlock.tryLow = ReadUnsigned(&_buffer);
			_tryBlock.tryHigh = ReadUnsigned(&_buffer);
			_tryBlock.catchHigh = ReadUnsigned(&_buffer);
			_tryBlock.dispHandlerArray = ReadInt(&_buffer);
		}
	};
	class HandlerMap4 {
		uint32_t _numHandlers;
		uint8_t* _buffer{};
		uint8_t* _bufferStart{};
		HandlerType4 _handler;
		uintptr_t _imageBase;
		int32_t _functionStart;
	public:
		HandlerMap4(const TryBlockMapEntry4* tryMap, uintptr_t imageBase, int32_t functionStart) : _imageBase(imageBase), _functionStart(functionStart)
		{
			if (tryMap->dispHandlerArray != 0)
			{
				_buffer = imageRelToByteBuffer(_imageBase, tryMap->dispHandlerArray);
				_numHandlers = ReadUnsigned(&_buffer);
				_bufferStart = _buffer;
				DecompHandler();
			}
			else
			{
				_numHandlers = 0;
			}
		}

		class iterator
		{
		private:
			HandlerMap4& _handlerMap;
			uint32_t _currBlock;
		public:
			iterator(HandlerMap4& handlerMap, uint32_t currBlock)
				: _handlerMap(handlerMap), _currBlock(currBlock)
			{
			}

			iterator& operator++()
			{
				_handlerMap.DecompHandler();
				_currBlock++;
				return *this;
			}

			iterator operator++(int)
			{
				iterator tmp(*this);
				operator++();
				return tmp;
			}

			HandlerType4 operator*()
			{
				return _handlerMap._handler;
			}

			bool operator==(const iterator& other) const
			{
				return (_currBlock == other._currBlock);
			}

			bool operator!=(const iterator& other) const
			{
				return !(*this == other);
			}

		};
		iterator begin()
		{
			return iterator(*this, 0);
		}
		iterator end()
		{
			return iterator(*this, _numHandlers);
		}
		void setBuffer(uint32_t index) {
			_buffer = _bufferStart;
			DecompHandler();
			for (uint32_t i = 0; i < index; i++) {
				DecompHandler();
			}
		}
		void resetBuffer() {
			setBuffer(0);
		}
		uint32_t getNumHandlers()const {
			return _numHandlers;
		}
		//this s destructive
		HandlerType4* getLastEntry() {
			resetBuffer();
			setBuffer(_numHandlers - 1);
			return &_handler;
		}
	private:
		void DecompHandler()
		{
			_handler.reset();
			_handler.header.value = _buffer[0];
			++_buffer;

			if (_handler.header.adjectives) {
				_handler.adjectives = ReadUnsigned(&_buffer);
			}

			if (_handler.header.dispType) {
				_handler.dispType = ReadInt(&_buffer);
			}

			if (_handler.header.dispCatchObj) {
				_handler.dispCatchObj = ReadUnsigned(&_buffer);
			}

			_handler.dispOfHandler = ReadInt(&_buffer);

			if (_handler.header.contIsRVA)
			{
				if (_handler.header.contAddr == HandlerTypeHeader::contType::ONE) {
					_handler.continuationAddress[0] = ReadInt(&_buffer);
				}
				else if (_handler.header.contAddr == HandlerTypeHeader::contType::TWO) {
					_handler.continuationAddress[0] = ReadInt(&_buffer);
					_handler.continuationAddress[1] = ReadInt(&_buffer);
				}
				else {
					// no encoded cont addresses or unknown
				}
			}
			else
			{
				if (_handler.header.contAddr == HandlerTypeHeader::contType::ONE) {
					_handler.continuationAddress[0] = _functionStart + ReadUnsigned(&_buffer);
				}
				else if (_handler.header.contAddr == HandlerTypeHeader::contType::TWO) {
					_handler.continuationAddress[0] = _functionStart + ReadUnsigned(&_buffer);
					_handler.continuationAddress[1] = _functionStart + ReadUnsigned(&_buffer);
				}
				else {
					// no encoded cont addresses or unknown
				}
			}
		}

	};


}//namespace EHD4
*/
#//pragma warning(pop)
#define _INC_STDLIB
#define _INC_STRING
using BYTE = unsigned char;
#define FAST_FAIL_FATAL_APP_EXIT UINT32_MAX
#include <ehdata4_export.h>//NOLINT(llvmlibc-restrict-system-libc-headers)

#undef _INC_STDLIB 
#undef _INC_STRING