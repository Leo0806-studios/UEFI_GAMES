#pragma once
#include "HEADER/SMART_POINTERS/CONTROL_BLOCKS/CONTROL_BASE/CONTROL_BASE_F.h"

namespace STD {
	template <typename Type>
	class GenericUniqueBlock : public ControlBase {
		template <typename T> friend class UniquePointer;
		Type* ptr = nullptr;
		using DeleterObj = bool(*)(Type* self);
		using Deleter = bool(*)(GenericUniqueBlock<Type>* self);
		Deleter deleter = nullptr;
		DeleterObj deleterObj = nullptr;
	public:
		void Destroy() override {
			if (ptr) {
				deleterObj(ptr);
				ptr = nullptr;
			}
		}
		void Delete() override {
			deleter(this);
		}
		void* Get() override {
			return static_cast<void*>(ptr);
		}
	};
}