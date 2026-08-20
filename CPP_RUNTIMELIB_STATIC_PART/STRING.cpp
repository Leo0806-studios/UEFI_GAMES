// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "STRING.h"
#include <intrin.h>
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//void* __cdecl memcpy(void* _Dst, const void* _Src, size_t _Size)
//{
//	size_t index = 0;
//#ifdef __AVX2__
//	for(; index + 32 <= _Size; index += 32) {
//		const __m256i data = _mm256_loadu_si256(
//			reinterpret_cast<__m256i const*>(static_cast<const unsigned char*>(_Src) + index));
//		_mm256_storeu_si256(
//			reinterpret_cast<__m256i*>(static_cast<unsigned char*>(_Dst) + index),
//			data);
//	}
//#endif
//	for (; index + 16 <= _Size; index += 16) {
//		const __m128i data = _mm_loadu_si128(
//			reinterpret_cast<__m128i const*>(static_cast<const unsigned char*>(_Src) + index));
//		_mm_storeu_si128(
//			reinterpret_cast<__m128i*>(static_cast<unsigned char*>(_Dst) + index),
//			data);
//	}
//	for (; index < _Size; ++index) {
//		static_cast<unsigned char*>(_Dst)[index] = static_cast<const unsigned char*>(_Src)[index];
//	}
//	return _Dst;
//}
