#pragma once
#include "HEADER/TYPE_TRAITS/TYPE_TRAITS_F.h"
#include "HEADER/ATOMICS/ATOMIC_F.h"
#include "HEADER/UTILLITY/UTILLITY_F.h"
namespace STD {

	class ControlBase {


		friend class PointerBase;

		NODISCARD constexpr ControlBase(const ControlBase&) = delete;
		NODISCARD constexpr ControlBase(ControlBase&&) = delete;
		NODISCARD ControlBase& operator=(const ControlBase&) = delete;
		NODISCARD ControlBase& operator=(ControlBase&&) = delete;
	public:
		virtual void Destroy() = 0;
		virtual void Delete() = 0;
		virtual void* Get() = 0;
	protected:
		STD::atomic_unsigned_long_long refCount = 1;
		STD::atomic_unsigned_long_long weakCount = 1;
	public:


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
				DecrementWeakCount();
			}
		}
		NODISCARD constexpr ControlBase() = default;
		virtual ~ControlBase() = default;

	};
}

