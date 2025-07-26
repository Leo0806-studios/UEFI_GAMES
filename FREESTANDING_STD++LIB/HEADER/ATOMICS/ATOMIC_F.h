#pragma once
#include <intrin.h>
#include "HEADER/TYPE_TRAITS/TYPE_TRAITS_F.h"
#include "HEADER/SAL/SAL_F.h"
namespace STD {
	template <typename Type>
	class atomic {
	public:
		static_assert(STD::is_trivially_copyable_v<Type>, "atomic<T> requires T to be trivially copyable.");
		static_assert(STD::is_copy_constructible_v<Type>, "atomic<T> requires T to be copy constructible.");
		static_assert(STD::is_move_constructible_v<Type>, "atomic<T> requires T to be move constructible.");
		static_assert(STD::is_copy_assignable_v<Type>, "atomic<T> requires T to be copy assignable.");
		static_assert(STD::is_move_assignable_v<Type>, "atomic<T> requires T to be move assignable.");
		static_assert(!STD::is_const_v<Type>, "atomic<T> requires T to be non-const.");
		static_assert(!STD::is_volatile_v<Type>, "atomic<T> requires T to be non-volatile.");
	private:

		Type atomicStorage = {};
		mutable bool Flag = false; 

	public:
		NODISCARD constexpr Type load()const {
			Type value = {};
			_mm_mfence();
			while (_InterlockedCompareExchange8(reinterpret_cast<char*>(&Flag), 1, 0) != 0) {
				_mm_pause();
			}
			value = atomicStorage;
			_mm_lfence();
			_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
			return value;

		}
		NODISCARD constexpr Type exchange(Type newValue) {
			_mm_mfence();
			while (_InterlockedCompareExchange8(reinterpret_cast<char*>(&Flag), 1, 0) != 0) {
				_mm_pause();
			}
			Type oldValue = atomicStorage;
			atomicStorage = newValue;
			_mm_lfence();
			_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
			return oldValue;
		}
		NODISCARD constexpr atomic() = default;
		NODISCARD constexpr atomic(const Type& value) : atomicStorage(value) {}
		NODISCARD constexpr atomic(const atomic& other) = delete;
		NODISCARD constexpr atomic(atomic&& other) = delete;
		NODISCARD atomic& operator=(const atomic& other) = delete;
		NODISCARD atomic& operator=(atomic&& other) = delete;

		NODISCARD constexpr bool compare_exchange_strong(Type& expected, Type desired) {
			_mm_mfence();
			while (_InterlockedCompareExchange8(reinterpret_cast<char*>(&Flag), 1, 0) != 0) {
				_mm_pause();
			}
			if (atomicStorage == expected) {
				atomicStorage = desired;
				_mm_lfence();
				_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
				return true;
			} else {
				expected = atomicStorage;
				_mm_lfence();
				_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
				return false;
			}

		}
		NODISCARD constexpr bool compare_exchange_weak(Type& expected, Type desired){
			_mm_mfence();
			while (_InterlockedCompareExchange8(reinterpret_cast<char*>(&Flag), 1, 0) != 0) {
				_mm_pause();
			}
			if (atomicStorage == expected) {
				atomicStorage = desired;
				_mm_lfence();
				_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
				return true;
			} else {
				expected = atomicStorage;
				_mm_lfence();
				_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
				return false;
			}
		}
		template <typename = STD::enable_if<STD::is_integral_v<Type> && !STD::is_same_v<Type, bool>>>
		NODISCARD constexpr Type fetch_add(Type value) {
			_mm_mfence();
			while (_InterlockedCompareExchange8(reinterpret_cast<char*>(&Flag), 1, 0) != 0) {
				_mm_pause();
			}
			Type oldValue = atomicStorage;
			atomicStorage += value;
			_mm_lfence();
			_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
			return oldValue;
		}
		NODISCARD constexpr Type fetch_sub(Type value) {
			_mm_mfence();
			while (_InterlockedCompareExchange8(reinterpret_cast<char*>(&Flag), 1, 0) != 0) {
				_mm_pause();
			}
			Type oldValue = atomicStorage;
			atomicStorage -= value;
			_mm_lfence();
			_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
			return oldValue;
		}
	};
	using atomic_bool = atomic<bool>;
	using atomic_char = atomic<char>;
	using atomic_signed_char = atomic<signed char>;
	using atomic_unsigned_char = atomic<unsigned char>;
	using atomic_int = atomic<int>;
	using atomic_unsigned_int = atomic<unsigned int>;
	using atomic_short = atomic<short>;
	using atomic_unsigned_short = atomic<unsigned short>;
	using atomic_long = atomic<long>;
	using atomic_unsigned_long = atomic<unsigned long>;
	using atomic_long_long = atomic<long long>;
	using atomic_unsigned_long_long = atomic<unsigned long long>;

}