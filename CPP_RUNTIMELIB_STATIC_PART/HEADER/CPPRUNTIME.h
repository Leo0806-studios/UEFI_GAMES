//###########################################
//				CPPRUNTIME.h
//	Copyright (C) Leo0806 2025
//	Created -
//	Created by Leo0806
//	Last modified 03.09.2025
//	This file is part of the cpp runtime
//	this is the main include for the cpp runtime. contains many forward declarations (untill moved to their own headers later)
// and small helper functions
//############################################

#include <sal.h>//NOLINT //this is itelsf a system header. im writing a runtime
#include "HEADER/CRT_MACROS.h"
//#include<compare>
extern "C" {
	/// <summary>
	/// malloc.
	/// if _DEBUG is defined returns nullptr on more error conditions.
	/// without it it assumes that none of these error conditions will happen and might crash the program if they do.
	/// </summary>
	/// <param name="_Size"></param>
	/// <returns></returns>
	_Check_return_ _Ret_maybenull_ _Post_writable_byte_size_(_Size)_declspec(restrict) void* malloc(_In_ size_t _Size); //NOLINT(readability-redundant-declaration)
	void free(//NOLINT(readability-redundant-declaration)
		_Pre_maybenull_ _Post_invalid_ void* _Block
	);

	void _free_dbg(void* _Block);

}
template<typename T> [[nodiscard]] constexpr __forceinline bool static AS_BOOL(T x) { return static_cast<bool>(x); }//NOLINT //NOSONAR


namespace std {//NOSONAR
	/// <summary>
	/// terminates the current process and all its threds. if no os callback is supplied it will looop forever. in kernel mode it causes a panic
	/// </summary>
	_declspec(noreturn) void terminate();

	void setTerminateHandler(void(*handler)());
}//namespace std
CRT_START
__declspec(noreturn) void abort();
CRT_END
namespace std {
	template<typename SizeType,typename Tag>
	class StrongSize {
		SizeType value_;
		template<typename SizeType2,typename Tag2> friend class StrongSize;
	public:
		// Constructor
		explicit constexpr StrongSize(SizeType v) : value_(v) {}

		// Access the raw value explicitly
		[[nodiscard]]  constexpr  SizeType value() const { return value_; }

		// Increment / decrement
		StrongSize& operator++() { ++value_; return *this; }
		StrongSize operator++(int) { StrongSize temp = *this; ++value_; return temp; }
		StrongSize& operator--() { --value_; return *this; }

		// Arithmetic with same type
		constexpr StrongSize operator+(StrongSize other) const { return StrongSize(value_ + other.value_); }
		constexpr StrongSize operator-(StrongSize other) const { return StrongSize(value_ - other.value_); }

		// Comparison
		constexpr bool operator<=>(const StrongSize& other) const = default;
		constexpr bool operator<=>(const SizeType& other)const {
			return value_ <=> other;
		}
		template<typename Tag2>
		constexpr StrongSize<SizeType,Tag2> castTo() const { return StrongSize<SizeType,Tag2>(value_); }
		template<typename SizeType2,typename Tag2>
		constexpr void castFrom(StrongSize<SizeType2,Tag2> other) { value_ = other.value_; }
	};
	struct IndexTag {};
	struct SizeTag {};
	struct OffsetTag {};

	using Index = StrongSize<size_t,IndexTag>;
	using Size = StrongSize<size_t,SizeTag>;
	using Offset = StrongSize<size_t,OffsetTag>;

	using iIndex = StrongSize<int, IndexTag>;
	using iSize = StrongSize<int, SizeTag>;
	using iOffset = StrongSize<int, OffsetTag>;
	using cIndex = StrongSize<char, IndexTag>;
	using cSize = StrongSize<char, SizeTag>;
	using cOffset = StrongSize<char, OffsetTag>;

}//namespace std