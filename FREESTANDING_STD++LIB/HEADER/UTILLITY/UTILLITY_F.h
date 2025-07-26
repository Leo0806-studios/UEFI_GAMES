#pragma once
#include "HEADER/TYPE_TRAITS/TYPE_TRAITS_F.h"
namespace STD {
#pragma warning (push)
#pragma warning(disable:26473)
	template<typename _Ty>
	constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept {
		return static_cast<remove_reference_t<_Ty>&&>(_Arg);
	}
#pragma warning(pop)


	struct _Ignore { // struct that ignores assignments
		template <class _Ty>
		constexpr const _Ignore& operator=(const _Ty&) const noexcept {
			// do nothing
			return *this;
		}
	};

	 inline constexpr _Ignore ignore{};
}