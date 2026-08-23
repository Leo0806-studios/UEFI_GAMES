#include "ctype.h"
#include "stdlib.h"
double atof(const char* str) {
	double a = 0.0, sign = 1.0;
	int e = 0;
	while (isspace((unsigned char)*str)) str++;
	if (*str == '-') { sign = -1.0; str++; }
	else if (*str == '+') str++;
	while (isdigit((unsigned char)*str)) { a = a * 10.0 + (*str - '0'); str++; }
	if (*str == '.') {
		str++;
		while (isdigit((unsigned char)*str)) { a = a * 10.0 + (*str - '0'); e--; str++; }
	}
	if (*str == 'e' || *str == 'E') {
		str++; int esign = 1, exp = 0;
		if (*str == '-') { esign = -1; str++; }
		else if (*str == '+') str++;
		while (isdigit((unsigned char)*str)) { exp = exp * 10 + (*str - '0'); str++; }
		e += (exp * esign);
	}
	while (e > 0) { a *= 10.0; e--; }
	while (e < 0) { a *= 0.1; e++; }
	return a * sign;
}
int atoi(const char* nptr) {
	int ret = 0;
	int sign = 1;
	while (isspace((unsigned char)*nptr)) nptr++;
	if (*nptr == '-') { sign = -1; nptr++; }
	else if (*nptr == '+') nptr++;
	while (isdigit((unsigned char)*nptr)) { ret = ret * 10 + (*nptr - '0'); nptr++; }
	return ret * sign;
}

long int atol(const char* nptr)
{
	long int ret = 0;
	int sign = 1;
	while (isspace((unsigned char)*nptr)) nptr++;
	if (*nptr == '-') { sign = -1; nptr++; }
	else if (*nptr == '+') nptr++;
	while (isdigit((unsigned char)*nptr)) { ret = ret * 10 + (*nptr - '0'); nptr++; }
	return ret * sign;

}

long long int atoll(const char* nptr) {
	long long int ret = 0;
	int sign = 1;
	while (isspace((unsigned char)*nptr)) nptr++;
	if (*nptr == '-') { sign = -1; nptr++; }
	else if (*nptr == '+') nptr++;
	while (isdigit((unsigned char)*nptr)) { ret = ret * 10 + (*nptr - '0'); nptr++; }
	return ret * sign;
}




/// TODO : implement 


double strtod(const char* restrict nptr, char** restrict endptr)
{
	const char* str = nptr;
	double a = 0.0, sign = 1.0;
	int e = 0;
	while (isspace((unsigned char)*str)) str++;
	if (*str == '-') { sign = -1.0; str++; }
	else if (*str == '+') str++;
	while (isdigit((unsigned char)*str)) { a = a * 10.0 + (*str - '0'); str++; }
	if (*str == '.') {
		str++;
		while (isdigit((unsigned char)*str)) { a = a * 10.0 + (*str - '0'); e--; str++; }
	}
	if (*str == 'e' || *str == 'E') {
		str++; int esign = 1, exp = 0;
		if (*str == '-') { esign = -1; str++; }
		else if (*str == '+') str++;
		while (isdigit((unsigned char)*str)) { exp = exp * 10 + (*str - '0'); str++; }
		e += (exp * esign);
	}
	while (e > 0) { a *= 10.0; e--; }
	while (e < 0) { a *= 0.1; e++; }
	*endptr = (char*)str;
	return a * sign;
}

float strtof(const char* restrict nptr, char** restrict endptr)
{
	const char* str = nptr;
	double a = 0.0, sign = 1.0;
	int e = 0;
	while (isspace((unsigned char)*str)) str++;
	if (*str == '-') { sign = -1.0; str++; }
	else if (*str == '+') str++;
	while (isdigit((unsigned char)*str)) { a = a * 10.0 + (*str - '0'); str++; }
	if (*str == '.') {
		str++;
		while (isdigit((unsigned char)*str)) { a = a * 10.0 + (*str - '0'); e--; str++; }
	}
	if (*str == 'e' || *str == 'E') {
		str++; int esign = 1, exp = 0;
		if (*str == '-') { esign = -1; str++; }
		else if (*str == '+') str++;
		while (isdigit((unsigned char)*str)) { exp = exp * 10 + (*str - '0'); str++; }
		e += (exp * esign);
	}
	while (e > 0) { a *= 10.0; e--; }
	while (e < 0) { a *= 0.1; e++; }
	*endptr = (char*)str;
	return a * sign;;
}

long double strtold(const char* restrict nptr, char** restrict endptr)
{
	const char* str = nptr;
	double a = 0.0, sign = 1.0;
	int e = 0;
	while (isspace((unsigned char)*str)) str++;
	if (*str == '-') { sign = -1.0; str++; }
	else if (*str == '+') str++;
	while (isdigit((unsigned char)*str)) { a = a * 10.0 + (*str - '0'); str++; }
	if (*str == '.') {
		str++;
		while (isdigit((unsigned char)*str)) { a = a * 10.0 + (*str - '0'); e--; str++; }
	}
	if (*str == 'e' || *str == 'E') {
		str++; int esign = 1, exp = 0;
		if (*str == '-') { esign = -1; str++; }
		else if (*str == '+') str++;
		while (isdigit((unsigned char)*str)) { exp = exp * 10 + (*str - '0'); str++; }
		e += (exp * esign);
	}
	while (e > 0) { a *= 10.0; e--; }
	while (e < 0) { a *= 0.1; e++; }
	*endptr = (char*)str;
	return a * sign;
}






static int digit_value(unsigned char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';

	if (c >= 'a' && c <= 'z')
		return c - 'a' + 10;

	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 10;

	return -1;
}

static const char* skip_whitespace(const char* str)
{
	while (isspace((unsigned char)*str))
		str++;

	return str;
}

static int get_sign(const char** str)
{
	int sign = 1;

	if (**str == '-')
	{
		sign = -1;
		(*str)++;
	}
	else if (**str == '+')
	{
		(*str)++;
	}

	return sign;
}

static int get_base(const char** str, int base)
{
	if (base != 0)
		return base;

	if ((*str)[0] == '0')
	{
		if ((*str)[1] == 'x' || (*str)[1] == 'X')
			return 16;

		return 8;
	}

	return 10;
}

static const char* skip_base_prefix(const char* str, int base)
{
	if (base == 16 && str[0] == '0' &&
		(str[1] == 'x' || str[1] == 'X'))
	{
		return str + 2;
	}

	return str;
}

static int get_digit(const char** str, int base)
{
	int digit = digit_value((unsigned char)**str);

	if (digit < 0 || digit >= base)
		return -1;

	(*str)++;
	return digit;
}

static long long parse_digits(const char** str, int base)
{
	long long value = 0;
	int digit;

	while ((digit = get_digit(str, base)) >= 0)
		value = value * base + digit;

	return value;
}
static unsigned long long parse_unsigned_digits(const char** str, int base)
{
	unsigned long long value = 0;
	int digit;
	while ((digit = get_digit(str, base)) >= 0)
		value = value * base + digit;
	return value;
}
long strtol(const char* restrict nptr, char** restrict endptr, int base)
{
	const char* str = skip_whitespace(nptr);

	int sign = get_sign(&str);

	base = get_base(&str, base);
	str = skip_base_prefix(str, base);

	const char* digits_start = str;
	long long value = parse_digits(&str, base);

	if (endptr)
	{
		if (str == digits_start)
			*endptr = (char*)nptr;
		else
			*endptr = (char*)str;
	}

	return (long)(value * sign);
}

long long strtoll(const char* restrict nptr, char** restrict endptr, int base)
{
	const char* str = skip_whitespace(nptr);

	int sign = get_sign(&str);

	base = get_base(&str, base);
	str = skip_base_prefix(str, base);

	const char* digits_start = str;
	long long value = parse_digits(&str, base);

	if (endptr)
	{
		if (str == digits_start)
			*endptr = (char*)nptr;
		else
			*endptr = (char*)str;
	}

	return (value * sign);
}

unsigned long strtoul(const char* restrict nptr, char** restrict endptr, int base)
{
	const char* str = skip_whitespace(nptr);
	base = get_base(&str, base);
	str = skip_base_prefix(str, base);
	const char* digits_start = str;
	unsigned long long value = parse_unsigned_digits(&str, base);
	if (endptr)
	{
		if (str == digits_start)
			*endptr = (char*)nptr;
		else
			*endptr = (char*)str;
	}
	return (unsigned long)value;
}

unsigned long long strtoull(const char* restrict nptr, char** restrict endptr, int base)
{
	const char* str = skip_whitespace(nptr);
	base = get_base(&str, base);
	str = skip_base_prefix(str, base);
	const char* digits_start = str;
	unsigned long long value = parse_unsigned_digits(&str, base);
	if (endptr)
	{
		if (str == digits_start)
			*endptr = (char*)nptr;
		else
			*endptr = (char*)str;
	}
	return value;
}

static unsigned int state = 1;//random by diecroll
int rand(void)
{
	//technicaly valid by the c standart
	//seed++;
	//return seed % (RAND_MAX + 1);

	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;

	return (int)(state % ((unsigned int)RAND_MAX + 1u));
}
