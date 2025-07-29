#pragma once
namespace STD {
	template <class _Ty, _Ty _Val>
	struct integral_constant {
		static constexpr _Ty value = _Val;

		using value_type = _Ty;
		using type = integral_constant;

		constexpr operator value_type() const noexcept {
			return value;
		}

		 constexpr value_type operator()() const noexcept {
			return value;
		}
	};
	 template <class... _Types>
		using void_t = void;
	template <class _Ty, class = void>
	struct _Add_reference { // add reference (non-referenceable type)
		using _Lvalue = _Ty;
		using _Rvalue = _Ty;
	};

	template <class _Ty>
	struct _Add_reference<_Ty, void_t<_Ty&>> { // (referenceable type)
		using _Lvalue = _Ty&;
		using _Rvalue = _Ty&&;
	};

	 template <class _Ty>
		struct add_lvalue_reference {
		using type = typename _Add_reference<_Ty>::_Lvalue;
	};

	 template <class _Ty>
		using add_lvalue_reference_t = typename _Add_reference<_Ty>::_Lvalue;

	 template <class _Ty>
		struct add_rvalue_reference {
		using type = typename _Add_reference<_Ty>::_Rvalue;
	};

	 template <class _Ty>
		using add_rvalue_reference_t = typename _Add_reference<_Ty>::_Rvalue;

	template <bool _Val>
	using bool_constant = integral_constant<bool, _Val>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

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
	using remove_reference_t = typename remove_reference<T>::type;

	 template <class _Ty>
		struct remove_cv { // remove top-level const and volatile qualifiers
		using type = _Ty;

		template <template <class> class _Fn>
		using _Apply = _Fn<_Ty>; // apply cv-qualifiers from the class template argument to _Fn<_Ty>
	};

	template <class _Ty>
	struct remove_cv<const _Ty> {
		using type = _Ty;

		template <template <class> class _Fn>
		using _Apply = const _Fn<_Ty>;
	};

	template <class _Ty>
	struct remove_cv<volatile _Ty> {
		using type = _Ty;

		template <template <class> class _Fn>
		using _Apply = volatile _Fn<_Ty>;
	};

	template <class _Ty>
	struct remove_cv<const volatile _Ty> {
		using type = _Ty;

		template <template <class> class _Fn>
		using _Apply = const volatile _Fn<_Ty>;
	};

	 template <class _Ty>
		using remove_cv_t = typename remove_cv<_Ty>::type;

	template<typename, typename>
	constexpr inline bool is_same_v = false;
	template<typename T>
	constexpr inline bool is_same_v<T, T> = true;

	template<typename T >
	constexpr inline bool is_pod_v = __is_pod(T);

	template <bool _Test, typename _Ty = void>
	struct enable_if {}; // no member "type" when !_Test

	template <typename _Ty>
	struct enable_if<true, _Ty> { // type is _Ty for _Test
		using type = _Ty;
	};

	template <bool _Test, typename _Ty = void>
	using enable_if_t = typename enable_if<_Test, _Ty>::type;

	template <class _Ty>
	struct is_trivially_copyable : bool_constant<__is_trivially_copyable(_Ty)> {
		// determine whether _Ty is a trivially copyable type
	};

	template <class _Ty>
	constexpr bool is_trivially_copyable_v = __is_trivially_copyable(_Ty);


	template <class _Ty>
	constexpr bool is_copy_constructible_v = __is_constructible(_Ty, add_lvalue_reference_t<const _Ty>);

	template <class _Ty>
	constexpr bool is_move_constructible_v = __is_constructible(_Ty, _Ty);
	template <class _Ty>
	constexpr bool is_copy_assignable_v = __is_assignable(add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<const _Ty>);
	template <class _Ty>
	constexpr bool is_move_assignable_v = __is_assignable(add_lvalue_reference_t<_Ty>, _Ty);

	template <class>
	constexpr bool is_const_v = false; // determine whether type argument is const qualified

	template <class _Ty>
	constexpr bool is_const_v<const _Ty> = true;
	template <class>
	constexpr bool is_volatile_v = false; // determine whether type argument is volatile qualified

	template <class _Ty>
	constexpr bool is_volatile_v<volatile _Ty> = true;


	template <class _Ty, class... _Types>
	constexpr bool _Is_any_of_v = // true if and only if _Ty is in _Types
	(STD::is_same_v<_Ty, _Types> || ...);

	 template <class _Ty>
		constexpr bool is_integral_v = _Is_any_of_v<remove_cv_t<_Ty>, bool, char, signed char, unsigned char, wchar_t,
#ifdef __cpp_char8_t
		char8_t,
#endif // defined(__cpp_char8_t)
		char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;
		template <class _Ty>
		constexpr bool is_floating_point_v = _Is_any_of_v<remove_cv_t<_Ty>, float, double, long double>;

		template <class _From, class _To>
		constexpr bool is_convertible_v = __is_convertible_to(_From, _To);
}