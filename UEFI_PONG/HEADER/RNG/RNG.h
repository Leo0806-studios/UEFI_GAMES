#pragma once
#include <stdint.h>
void InitializeRNG();
void ShutdownRNG();
size_t GetRandomUint();
size_t GetRandomUintRange(size_t min, size_t max);
__int64 GetRandomInt();
__int64 GetRandomIntRange(__int64 min, __int64 max);
float GetRandomFloat();
float GetRandomFloatRange(float min, float max);
double GetRandomDouble();
double GetRandomDoubleRange(double min, double max);
void GetRandomBytes(void* buffer, size_t size);
