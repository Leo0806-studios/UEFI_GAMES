#pragma once
#include <intrin.h>
#include "HEADER/TYPE_TRAITS/TYPE_TRAITS_F.h"
#include "HEADER/SAL/SAL_F.h"
namespace STD {
	/**
	* uses the correct exchange function based on the size of the type.
	* Type : the type to exchange.
	* target : the target pointer to exchange.
	* value : the value to exchange with.
	* ret : the return value of the exchange operation. this is the initial value pointer to by target.
	*/
#define SELECT_CORECT_EXCHANGE(Type,target,value,ret)\
if constexpr(sizeof(Type)==1){\
		ret = _InterlockedExchange8(reinterpret_cast<char*>(target), value);\
	} else if constexpr(sizeof(Type)==2){\
		ret = _InterlockedExchange16(reinterpret_cast<short*>(target), value);\
	} else if constexpr(sizeof(Type)==4){\
		ret = _InterlockedExchange(reinterpret_cast<long*>(target), value);\
	} else if constexpr(sizeof(Type)==8){\
		ret = _InterlockedExchange64(reinterpret_cast<long long*>(target), value);\
	} 


	/**
	* uses the correct load function based on the size of the type.
	*/
#define SELECT_CORECT_LOAD(Type,target,ret)\
if constexpr(sizeof(Type)==1){\
ret = _InterlockedOr8(reinterpret_cast<char*>(target), 0);\
	} else if constexpr(sizeof(Type)==2){\
ret = _InterlockedOr16(reinterpret_cast<short*>(target), 0);\
	} else if constexpr(sizeof(Type)==4){\
ret = _InterlockedOr(reinterpret_cast<long*>(target), 0);\
	} else if constexpr(sizeof(Type)==8){\
ret = _InterlockedOr64(reinterpret_cast<long long*>(target), 0);\
	}

#define SELECT_CORECT_ADD(Type,target,value,ret)\
if constexpr(sizeof(Type)==1){\
		ret = _InterlockedExchangeAdd8(reinterpret_cast<char*>(target), value);\
	} else if constexpr(sizeof(Type)==2){\
		ret = _InterlockedExchangeAdd16(reinterpret_cast<short*>(target), value);\
	} else if constexpr(sizeof(Type)==4){\
		ret = _InterlockedExchangeAdd(reinterpret_cast<long*>(target), value);\
	} else if constexpr(sizeof(Type)==8){\
		ret = _InterlockedExchangeAdd64(reinterpret_cast<long long*>(target), value);\
	}

	/**
	* uses the correct compare exchange function based on the size of the type.
	* Type : the type to compare and exchange.
	* target : the target pointer to compare and exchange.
	* expected : the expected value to compare with.
	* desired : the value to exchange with if the expected value matches the target.
	* ret : the return value of the compare exchange operation. this is the initial value pointed to by target.
	* target,expected and value ignore the sign https://learn.microsoft.com/en-us/cpp/intrinsics/interlockedcompareexchange-intrinsic-functions?view=msvc-170
	*/
#define SELECT_CORECT_COMPARE_EXCHANGE(Type,target,expected,desired,ret)\
if constexpr(sizeof(Type)==1){\
		ret = _InterlockedCompareExchange8(reinterpret_cast<char*>(target), desired, expected);\
	} else if constexpr(sizeof(Type)==2){\
		ret = _InterlockedCompareExchange16(reinterpret_cast<short*>(target), desired, expected);\
	} else if constexpr(sizeof(Type)==4){\
		ret = _InterlockedCompareExchange(reinterpret_cast<long*>(target), desired, expected);\
	} else if constexpr(sizeof(Type)==8){\
		ret = _InterlockedCompareExchange64(reinterpret_cast<long long*>(target), desired, expected);\
	}



	/**
*
* the following tis the base template for non integral not floating non bool types that are larger than 128 bits.
* //TODO implement this for types smaler  than 128 bits.
*/
	template <typename Type,typename enable = void>
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
		NODISCARD constexpr void store(const Type& value) {
			_mm_mfence();
			while (_InterlockedCompareExchange8(reinterpret_cast<char*>(&Flag), 1, 0) != 0) {
				_mm_pause();
			}
			atomicStorage = value;
			_mm_lfence();
			_InterlockedExchange8(reinterpret_cast<char*>(&Flag), 0);
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

	template <typename Type>
	class atomic < Type, STD::enable_if_t<STD::is_integral_v<Type>> > {
		public:
		static_assert(!STD::is_const_v<Type>, "atomic<T> for integrals requires T to be non-const.");
		static_assert(!STD::is_volatile_v<Type>, "atomic<T> for integrals requires T to be non-volatile.");
	private:
		Type atomicStorage = 0;
	public:
		NODISCARD Type load() {
			Type ret = 0;
			SELECT_CORECT_LOAD(Type, &atomicStorage, ret);
			return ret;
		}
		NODISCARD void store(const Type value) {
			Type ret = 0;
			SELECT_CORECT_EXCHANGE(Type, &atomicStorage, value, ret);
			
		}
		NODISCARD Type exchange(Type newValue) {
			Type ret = 0;
			SELECT_CORECT_EXCHANGE(Type, &atomicStorage, newValue, ret);
			return ret;
		}
		NODISCARD atomic() = default;
		NODISCARD atomic(const Type& value) : atomicStorage(value) {}
		NODISCARD atomic(const atomic& other) = delete;
		NODISCARD atomic(atomic&& other) = delete;
		NODISCARD atomic& operator=(const atomic& other) = delete;
		NODISCARD atomic& operator=(atomic&& other) = delete;
		NODISCARD bool compare_exchange_strong(Type expected, Type desired) {
			Type ret = 0;
			SELECT_CORECT_COMPARE_EXCHANGE(Type, &atomicStorage, expected, desired, ret);
			return ret == expected;
		}
		/// <summary>
		/// compare exchange_weak is a weaker version of compare_exchange_strong.
		/// currently it does the same thing as compare_exchange_strong.
		/// it is just here for compatibility with the C++ standard.
		/// </summary>
		/// <param name="expected"></param>
		/// <param name="desired"></param>
		/// <returns></returns>
		NODISCARD bool compare_exchange_weak(Type expected, Type desired) {
			Type ret = 0;
			SELECT_CORECT_COMPARE_EXCHANGE(Type, &atomicStorage, expected, desired, ret);
			return ret == expected;
		}

		NODISCARD Type fetch_add(Type value) {
			Type ret = 0;
			SELECT_CORECT_EXCHANGE(Type, &atomicStorage,  value, ret);
			return ret;
		}
		NODISCARD Type fetch_sub(Type value) {
			Type ret = 0;
			SELECT_CORECT_ADD(Type, &atomicStorage,  value, ret);
			return ret;
		}
		NODISCARD Type increment() {
			return fetch_add(1);
		}
		NODISCARD Type decrement() {
			return fetch_sub(1);
		}
		NODISCARD Type operator++() {
			return increment();
		}
		NODISCARD Type operator++(int) {
			Type oldValue = load();
			increment();
			return oldValue;
		}
		NODISCARD Type operator--() {
			return decrement();
		}
		NODISCARD Type operator--(int) {
			Type oldValue = load();
			decrement();
			return oldValue;
		}
	};

	template <typename Type>
	class atomic < Type, STD::enable_if_t<STD::is_floating_point_v<Type>> > {
	public:
		static_assert(!STD::is_const_v<Type>, "atomic<T> for floating point requires T to be non-const.");
		static_assert(!STD::is_volatile_v<Type>, "atomic<T> for floating point requires T to be non-volatile.");
	private:
		Type atomicStorage = 0.0;
	public:
		NODISCARD Type load() {
			Type ret = 0.0;
			SELECT_CORECT_LOAD(Type, &atomicStorage, ret);
			return ret;
		}
		NODISCARD void store(const Type& value) {
			Type ret = 0.0;
			SELECT_CORECT_EXCHANGE(Type, &atomicStorage, value, ret);
			
		}
		NODISCARD Type exchange(Type newValue) {
			Type ret = 0.0;
			SELECT_CORECT_EXCHANGE(Type, &atomicStorage, newValue, ret);
			return ret;
		}
		NODISCARD atomic() = default;
		NODISCARD atomic(const Type& value) : atomicStorage(value) {}
		NODISCARD atomic(const atomic& other) = delete;
		NODISCARD atomic(atomic&& other) = delete;
		NODISCARD atomic& operator=(const atomic& other) = delete;
		NODISCARD atomic& operator=(atomic&& other) = delete;
		//no compare exchange for floating point as without the ability to use epsilon it is not possible to compare floating point values reliably


		NODISCARD Type fetch_add(Type value) {
			Type ret = 0.0;
			SELECT_CORECT_ADD(Type, &atomicStorage,  value, ret);
			return ret;
		}
		NODISCARD Type fetch_sub(Type value) {
			Type ret = 0.0;
			SELECT_CORECT_ADD(Type, &atomicStorage,  value, ret);
			return ret;
		}
	};
	template <>
	class atomic < bool, STD::enable_if_t<STD::is_same_v<bool, bool>> > {
		public:
		static_assert(!STD::is_const_v<bool>, "atomic<T> for bool requires T to be non-const.");
		static_assert(!STD::is_volatile_v<bool>, "atomic<T> for bool requires T to be non-volatile.");
	private:
		bool atomicStorage = false;
	public:
		NODISCARD bool load() {
		

			return static_cast<bool>(_InterlockedOr8(reinterpret_cast<char*>(&atomicStorage), 0));
		}
		NODISCARD bool store(const bool& value) {
			

			return static_cast<bool>(_InterlockedExchange8(reinterpret_cast<char*>(&atomicStorage), value));
		}
		NODISCARD bool exchange(bool newValue) {

			return static_cast<bool>(_InterlockedExchange8(reinterpret_cast<char*>(&atomicStorage), newValue));
		}
		NODISCARD atomic() = default;
		NODISCARD atomic(const bool& value) : atomicStorage(value) {}
		NODISCARD atomic(const atomic& other) = delete;
		NODISCARD atomic(atomic&& other) = delete;
		NODISCARD atomic& operator=(const atomic& other) = delete;
		NODISCARD atomic& operator=(atomic&& other) = delete;
		NODISCARD bool compare_exchange_strong(bool expected, bool desired) {


			return  static_cast<bool>(_InterlockedCompareExchange8(reinterpret_cast<char*>(&atomicStorage), desired, expected)) == expected;
		}
		NODISCARD bool compare_exchange_weak(bool expected, bool desired) {
			return  static_cast<bool>(_InterlockedCompareExchange8(reinterpret_cast<char*>(&atomicStorage), desired, expected)) == expected;
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