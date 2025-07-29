#pragma once
#include "HEADER/SMART_POINTERS/CONTROL_BLOCKS/CONTROL_BASE/CONTROL_BASE_F.h"

namespace STD {
	template <typename Type>
	class GenericUniqueBlock : public ControlBase {
		template <typename T> friend class UniquePointer;
		Type* ptr = nullptr;
	public:
		void Destroy() override {
			if (ptr) {
				delete ptr;
				ptr = nullptr;
			}
		}
		void Delete() override {
			delete this;
		}
		void* Get() override {
			return static_cast<void*>(ptr);
		}
	};
}