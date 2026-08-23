#pragma once
#ifndef __INTTYPES__
#define __INTTYPES__
#include "stdint.h"
typedef struct
{
	intmax_t quot;
	intmax_t rem;
} _Lldiv_t;

typedef _Lldiv_t imaxdiv_t;

#endif // !__INTTYPES__
