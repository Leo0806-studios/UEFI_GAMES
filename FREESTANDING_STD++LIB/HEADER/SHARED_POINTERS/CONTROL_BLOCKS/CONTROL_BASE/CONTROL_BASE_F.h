#pragma once
#include "HEADER/TYPE_TRAITS/TYPE_TRAITS_F.h"
#include "HEADER/ATOMICS/ATOMIC_F.h"
#include "HEADER/UTILLITY/UTILLITY_F.h"
namespace STD {

	class ControlBase {
		STD::atomic_unsigned_long_long refCount = 1;
		STD::atomic_unsigned_long_long weakCount = 1;
		template <typename T>friend class SharedPointer;
		template <typename T>friend class WeakPointer;
		template<typename T> friend class UniquePointer;
		friend class PointerBase;


		NODISCARD constexpr ControlBase() = default;
		NODISCARD constexpr ControlBase(const ControlBase&) = delete;
		NODISCARD constexpr ControlBase(ControlBase&&) = delete;
		NODISCARD ControlBase& operator=(const ControlBase&) = delete;
		NODISCARD ControlBase& operator=(ControlBase&&) = delete;

	protected:
		virtual void Destroy() = 0;
		virtual void Delete() = 0;
		virtual void IncrementWeakCount() {
			
			STD::ignore= weakCount.fetch_add(1);
		}
		virtual void DecrementWeakCount() {
			if (weakCount.fetch_sub(1) == 1) {
				Delete();

			}
		}
		virtual void IncrementRefCount()  {
			STD::ignore=refCount.fetch_add(1);
		}
		virtual void DecrementRefCount() {
			if (refCount.fetch_sub(1) == 1) {
				Destroy();
				DecrementRefCount();
			}
		}
	public:
		virtual ~ControlBase() = default;


	};
}

