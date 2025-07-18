#pragma once
#include "../TYPE_TRAITS/TYPE_TRAITS_F.h"
namespace STD {
	template<typename _Ty>
	constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept {
		return static_cast<remove_reference_t<_Ty>&&>(_Arg);
	}
}