#pragma once
#ifndef __MAIN__LOCALE__
#define __MAIN__LOCALE__
#include "locale.h"


typedef struct IMPL_clconv {
	const char* decimal_point; // "."
	const char* thousands_sep; // ""
	const char* grouping; // ""
	const char* mon_decimal_point; // ""
	const char* mon_thousands_sep; // ""
	const char* mon_grouping; // ""
	const char* positive_sign; // ""
	const char* negative_sign; // ""
	const char* currency_symbol; // ""
	char frac_digits; // CHAR_MAX
	char p_cs_precedes; // CHAR_MAX
	char n_cs_precedes; // CHAR_MAX
	char p_sep_by_space; // CHAR_MAX
	char n_sep_by_space; // CHAR_MAX
	char p_sign_posn; // CHAR_MAX
	char n_sign_posn; // CHAR_MAX
	const char* int_curr_symbol; // ""
	char int_frac_digits; // CHAR_MAX
	char int_p_cs_precedes; // CHAR_MAX
	char int_n_cs_precedes; // CHAR_MAX
	char int_p_sep_by_space; // CHAR_MAX
	char int_n_sep_by_space; // CHAR_MAX
	char int_p_sign_posn; // CHAR_MAX
	char int_n_sign_posn; // CHAR_MAX
} clconv;
typedef struct IMPL_locle {
	const char* upperCase; // "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	const char* lowerCase; // "abcdefghijklmnopqrstuvwxyz"
	const char* punctuation; // "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
	const char* whitespace; // " \t\n\v\f\r"
	const char* digits; // "0123456789"
	const char* hexDigits; // "0123456789abcdefABCDEF"
	const char* blank; // " \t"
	clconv lconv; //
} locale;
#endif // !__MAIN__LOCALE__
