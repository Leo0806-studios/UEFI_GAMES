#pragma once
#include "HEADER/TYPE_TRAITS/TYPE_TRAITS_F.h"
#include "HEADER/UTILLITY/UTILLITY_F.h"
#include "HEADER/INTERNAL/MACROS.h"
namespace STD {
	template<typename T>
	class assignable_once {
		private:
		T value;
		bool assigned = false;
	public:
		assignable_once() = default;
		//first copy and such for the assignable_once object
		assignable_once(const assignable_once&other):value(other.value),assigned(other.assigned) {	}
		assignable_once& operator=(const assignable_once& other) {
			if (this != &other) {
				value = other.value;
				assigned = other.assigned;
			}
			return *this;
		}
		assignable_once(assignable_once&& other)  : value(STD::move(other.value)), assigned(STD::exchange(other.assigned,false)) {}

		assignable_once& operator=(assignable_once&& other)  {
			if (this != &other) {
				value = STD::move(other.value);
				assigned = STD::exchange(other.assigned, false);
			}
			return *this;
		}
		// now for the wrapped object
		assignable_once(const T& val) : value(val), assigned(true) {}
		assignable_once(T&& val)  : value(STD::move(val)), assigned(true) {}
		assignable_once& operator=(const T& val) {
			if (!assigned) {
				value = val;
				assigned = true;
			} else {
				STD_ASSERT(false, "assignable_once can only be assigned once.");
			}
			return *this;
		}
		assignable_once& operator=(T&& val)  {
			if (!assigned) {
				value = STD::move(val);
				assigned = true;
			} else {
				STD_ASSERT(false, "assignable_once can only be assigned once.");
			}
			return *this;
		}
		bool assigne(const T& val) {
			if (!assigned) {
				value = val;
				assigned = true;
				return true;
			}
			STD_ASSERT(false, "assignable_once can only be assigned once.");
			return false;
		}
		bool assigne(T&& val) {
			if (!assigned) {
				value = STD::move(val);
				assigned = true;
				return true;
			}
			STD_ASSERT(false, "assignable_once can only be assigned once.");
			return false;
		}
		bool isLocked() const {
			return assigned;
		}
	};
}