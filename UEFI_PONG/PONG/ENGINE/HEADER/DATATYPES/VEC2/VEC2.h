#pragma once
#pragma warning(push,0)
#include <math.h>
#include <intrin.h>
#pragma warning(pop)
typedef struct  {
	float x; 
	float y; 
}Vector2;
inline Vector2 MultiplyVector2(Vector2 a, Vector2 b) {
	Vector2 result={a.x*b.x,a.y*b.y};
	return result;
}
inline Vector2 AddVector2(Vector2 a, Vector2 b) {
	Vector2 result={a.x+b.x,a.y+b.y};
	return result;
}
inline Vector2 SubtractVector2(Vector2 a, Vector2 b) {
	Vector2 result={a.x-b.x,a.y-b.y};
	return result;
}
inline Vector2 DivideVector2(Vector2 a, Vector2 b) {
	Vector2 result = { a.x / b.x,a.y / b.y };
	return result;
}
inline Vector2 ScaleVector2(Vector2 a, float scale) {
	Vector2 result={a.x*scale,a.y*scale};
	return result;
}
inline float LengthVector2(Vector2 a) {
	__m128 vec = _mm_set_ps(0, a.y, a.x, 0);
	__m128 squared = _mm_mul_ps(vec, vec);
	__m128 sum = _mm_hadd_ps(squared, squared);
	__m128 result = _mm_sqrt_ps(sum);
	float length;
	_mm_store_ss(&length, result);

	return length;
}
inline Vector2 NormalizeVector2(Vector2 a) {
 float length = LengthVector2(a);
	if (length == 0) {
		return (Vector2){0, 0}; // Return zero vector if length is zero to avoid division by zero
	}
	Vector2 result = {a.x / length, a.y / length};
	return result;
}
inline float DotProductVector2(Vector2 a, Vector2 b) {
	return a.x * b.x + a.y * b.y ;
}
inline bool EqualsVector2(Vector2 a, Vector2 b) {
	return a.x == b.x && a.y == b.y;
}
inline Vector2 InvertVector2(Vector2 a) {
	return ScaleVector2(a, -1.0f);
}
static Vector2 X_axis = { 1,0 };
static Vector2 Y_axis = { 0,1 };

