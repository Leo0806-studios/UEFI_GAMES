#pragma once
#ifndef __LOCALE__
#define __LOCALE__

#include "CRT_CORE.h"
#include "CRT_MACROS.h"
__CRT_START
#pragma warning(disable: __DISABLE_CRT_WARNINGS)
typedef struct IMPL_lconv {
	char* decimal_point; // "."
	char* thousands_sep; // ""
	char* grouping; // ""
	char* mon_decimal_point; // ""
	char* mon_thousands_sep; // ""
	char* mon_grouping; // ""
	char* positive_sign; // ""
	char* negative_sign; // ""
	char* currency_symbol; // ""
	char frac_digits; // CHAR_MAX
	char p_cs_precedes; // CHAR_MAX
	char n_cs_precedes; // CHAR_MAX
	char p_sep_by_space; // CHAR_MAX
	char n_sep_by_space; // CHAR_MAX
	char p_sign_posn; // CHAR_MAX
	char n_sign_posn; // CHAR_MAX
	char* int_curr_symbol; // ""
	char int_frac_digits; // CHAR_MAX
	char int_p_cs_precedes; // CHAR_MAX
	char int_n_cs_precedes; // CHAR_MAX
	char int_p_sep_by_space; // CHAR_MAX
	char int_n_sep_by_space; // CHAR_MAX
	char int_p_sign_posn; // CHAR_MAX
	char int_n_sign_posn; // CHAR_MAX
} lconv;

char* setlocale(int category, const char* locale);
lconv* localeconv(void);
__CRT_END
#endif // !__LOCALE__
