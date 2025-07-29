#pragma once
#include "HEADER/SMART_POINTERS/PTR_BASE/PTR_BASE.h"
#include "HEADER/SMART_POINTERS/CONTROL_BLOCKS/CONTROL_BASE/CONTROL_BASE_F.h"
#include "HEADER/SMART_POINTERS/CONTROL_BLOCKS/UNIQUE/UNIQUE_BLOCK.h"
#include "HEADER/TYPE_TRAITS/TYPE_TRAITS_F.h"
#include "HEADER/UTILLITY/UTILLITY_F.h"
#include "HEADER/SAL/SAL_F.h"
#include "HEADER/INTERNAL/MACROS.h"
namespace STD {
		template <typename Type>
	class UniquePointer : public PointerBase {
	public:
		UniquePointer() = default;
		UniquePointer(const UniquePointer&) = delete;
		UniquePointer(UniquePointer&& other) {
			controlBlock = STD::exchange(other.controlBlock,nullptr);
		}
		UniquePointer& operator=(const UniquePointer&) = delete;
		UniquePointer& operator=(UniquePointer&& other)  {
			if (this != &other) {
				controlBlock = STD::exchange(other.controlBlock, nullptr);
			}
			return *this;
		}
		~UniquePointer() {
#ifdef _DEBUG
			STD_ASSERT(controlBlock != nullptr, "UniquePointer is being destroyed without a control block.");
			if (controlBlock) {
				controlBlock->DecrementRefCount();

			}
			


#else
			if (controlBlock) {
				controlBlock->DecrementRefCount();

			}
#endif
		}
		Type* Get() const {
			return static_cast<Type*>(controlBlock->Get());
		}

		Type* operator->() const {
#ifdef _DEBUG
STD_ASSERT(controlBlock != nullptr, "UniquePointer is being dereferenced without a control block.");
if (!controlBlock) {
	return nullptr;
}
return static_cast<Type*>(controlBlock->Get());
#else
			return static_cast<Type*>(controlBlock->Get());

#endif // _DEBUG

		}
		static constexpr UniquePointer MakeUnique( CLAIMS(Type, ptr), CLAIMS(GenericUniqueBlock<Type>, controllBlock)) {
			UniquePointer uniquePtr;
			uniquePtr.controlBlock = controllBlock;
			static_cast<GenericUniqueBlock<Type>*>(uniquePtr.controlBlock)->ptr = ptr;
			return uniquePtr;
			
		}
	};
}
