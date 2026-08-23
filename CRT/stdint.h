#pragma once
#ifndef __STDINT__
#define  __STDINT__
#define __STDC_VERSION_STDINT_H__ 202311L


typedef signed __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;


typedef signed __int8 int_least8_t;
typedef unsigned __int8 uint_least8_t;
typedef __int16 int_least16_t;
typedef unsigned __int16 uint_least16_t;
typedef __int32 int_least32_t;
typedef unsigned __int32 uint_least32_t;
typedef  __int64 int_least64_t;
typedef unsigned __int64 uint_least64_t;


typedef signed __int8 int_fast8_t;
typedef unsigned __int8 uint_fast8_t;
typedef __int32 int_fast16_t;
typedef unsigned __int32 uint_fast16_t;
typedef __int32 int_fast32_t;
typedef unsigned __int32 uint_fast32_t;
typedef  __int64 int_fast64_t;
typedef unsigned __int64 uint_fast64_t;


typedef __int64 intptr_t;
typedef unsigned __int64 uintptr_t;


typedef __int64 intmax_t;
typedef unsigned __int64 uintmax_t;


#define INT8_WIDTH 8
#define UINT8_WIDTH 8
#define  INT16_WIDTH 16
#define UINT16_WIDTH 16
#define INT32_WIDTH 32
#define  UINT32_WIDTH 32
#define  INT64_WIDTH 64
#define  UINT64_WIDTH 64

#define INT_LEAST8_WIDTH 8
#define UINT_LEAST8_WIDTH 8
#define INT_LEAST16_WIDTH 16
#define UINT_LEAST16_WIDTH 16
#define INT_LEAST32_WIDTH 32
#define UINT_LEAST32_WIDTH 32
#define INT_LEAST64_WIDTH 64
#define UINT_LEAST64_WIDTH 64

#define INT_FAST8_WIDTH 8
#define UINT_FAST8_WIDTH 8
#define INT_FAST16_WIDTH 32
#define UINT_FAST16_WIDTH 32
#define INT_FAST32_WIDTH 32
#define UINT_FAST32_WIDTH 32
#define INT_FAST64_WIDTH 64
#define UINT_FAST64_WIDTH 64





#define INT8_MIN         (-127i8 - 1)
#define INT16_MIN        (-32767i16 - 1)
#define INT32_MIN        (-2147483647i32 - 1)
#define INT64_MIN        (-9223372036854775807i64 - 1)
#define INT8_MAX         127i8
#define INT16_MAX        32767i16
#define INT32_MAX        2147483647i32
#define INT64_MAX        9223372036854775807i64
#define UINT8_MAX        0xffui8
#define UINT16_MAX       0xffffui16
#define UINT32_MAX       0xffffffffui32
#define UINT64_MAX       0xffffffffffffffffui64

#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN    INT8_MIN
#define INT_FAST16_MIN   INT32_MIN
#define INT_FAST32_MIN   INT32_MIN
#define INT_FAST64_MIN   INT64_MIN
#define INT_FAST8_MAX    INT8_MAX
#define INT_FAST16_MAX   INT32_MAX
#define INT_FAST32_MAX   INT32_MAX
#define INT_FAST64_MAX   INT64_MAX
#define UINT_FAST8_MAX   UINT8_MAX
#define UINT_FAST16_MAX  UINT32_MAX
#define UINT_FAST32_MAX  UINT32_MAX
#define UINT_FAST64_MAX  UINT64_MAX


#endif // !__STDINT__
