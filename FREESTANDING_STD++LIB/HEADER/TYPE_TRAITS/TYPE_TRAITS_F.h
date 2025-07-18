#pragma once
namespace STD {
    template <class _Ty>
    struct remove_reference {
        using type = _Ty;
        using _Const_thru_ref_type = const _Ty;
    };

    template <class _Ty>
    struct remove_reference<_Ty&> {
        using type = _Ty;
        using _Const_thru_ref_type = const _Ty&;
    };

    template <class _Ty>
    struct remove_reference<_Ty&&> {
        using type = _Ty;
        using _Const_thru_ref_type = const _Ty&&;
    };

	template <typename T>
	using remove_reference_t =typename remove_reference<T>::type;
}