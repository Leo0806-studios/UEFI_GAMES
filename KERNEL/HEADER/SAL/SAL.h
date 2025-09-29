#pragma once
#ifndef OS_SAL_H
#define OS_SAL_H
#pragma once
#define NODISCARD [[nodiscard]]
#define NODISCARD_MSG(msg) [[nodiscard(msg)]]
#define FALLTHROUGH [[fallthrough]]
#define MAYBE_UNUSED [[maybe_unused]]
#define LIKELY [[likely]]
#define UNLIKELY [[unlikely]]
#define NO_RETURN [[noreturn]]
#define DEPRECATED [[deprecated]]
#define DEPRECATED_MSG(msg) [[deprecated(msg)]]

#include  <sal.h>
#endif // !_SAL_H
