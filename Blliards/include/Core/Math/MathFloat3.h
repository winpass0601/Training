#pragma once

#include <./Core/Math/MathStruct.h>

#include <math.h>


#define RINFWISNAN(x)  ((*(unsigned int*)&(x) & 0x7F800000) == 0x7F800000 && (*(unsigned int*)&(x) & 0x7FFFFF) != 0)
#define RINFWISINF(x)  ((*(unsigned int*)&(x) & 0x7FFFFFFF) == 0x7F800000)
namespace rinfw {
namespace core {
namespace math {

inline Float3 float3_Set(float _x, float _y, float _z) {
	Float3 ret(_x, _y, _z);
	return ret;
}
inline Float3 float3_Abs(Float3 _v) {
	Float3 result;
	result.x = fabsf(_v.x);
	result.y = fabsf(_v.y);
	result.z = fabsf(_v.z);
	return result;
}

inline Float3 float3_Min(Float3 _v1, Float3 _v2) {
	Float3 result;
	result.x = (_v1.x < _v2.x) ? _v1.x : _v2.x;
	result.y = (_v1.y < _v2.y) ? _v1.y : _v2.y;
	result.z = (_v1.z < _v2.z) ? _v1.z : _v2.z;
	return result;
}

inline Float3 float3_Max(Float3 _v1, Float3 _v2) {
	Float3 result;
	result.x = (_v1.x > _v2.x) ? _v1.x : _v2.x;
	result.y = (_v1.y > _v2.y) ? _v1.y : _v2.y;
	result.z = (_v1.z > _v2.z) ? _v1.z : _v2.z;
	return result;
}

inline Float3 float3_MultiplyAdd(Float3 _a, Float3 _b, Float3 _c) {
	Float3 result;

	result.x = (_a.x * _b.x) + _c.x;
	result.y = (_a.y * _b.y) + _c.y;
	result.z = (_a.z * _b.z) + _c.z;
	return result;
}

inline Float3 float3_NegativeMultiplySubtract(Float3 _v1, Float3 _v2, Float3 _v3) {
	Float3 result;

	result.x = _v3.x - (_v1.x * _v2.x);
	result.y = _v3.y - (_v1.y * _v2.y);
	result.z = _v3.z - (_v1.z * _v2.z);
	return result;
}

inline float float3_Length(Float3 _data) {
	return sqrtf(_data.x*_data.x + _data.y*_data.y + _data.z * _data.z);
}

inline Float3 float3_Normalize(Float3 _data) {
	float len = float3_Length(_data);
	
	_data.x /= len;
	_data.y /= len;
	_data.z /= len;
	return _data;
}
inline void float3_Normalize(Float3 _data, Float3& _out) {
	float len = float3_Length(_data);

	_out.x = _data.x / len;
	_out.y = _data.y / len;
	_out.z = _data.z / len;
}


inline float float3_Dot(Float3 _a, Float3 _b) {
	return _a.x*_b.x + _a.y*_b.y + _a.z*_b.z;
}
inline Float3 float3_Cross(Float3 _a, Float3 _b) {
	Float3 out;
	out.x = _a.y*_b.z - _a.z*_b.y;
	out.y = _a.z*_b.x - _a.x*_b.z;
	out.z = _a.x*_b.y - _a.y*_b.x;
	return out;
}

inline float float3_Angle(Float3 _a, Float3 _b) {
	float cos = float3_Dot(_a, _b) / (float3_Length(_a) * float3_Length(_b));

	float ans = acos(cos) * 180.0f / PI;

	return ans;
}


inline bool float3_Equal(Float3 _a, Float3 _b) {
	if (_a.x == _b.x && _a.y == _b.y && _a.z == _b.z)
		return true;
	return false;
}
inline Float3 float3_Clamp(Float3 _v1, Float3 _min, Float3 _max) {
	Float3 result;

	result = float3_Max(_min, _v1);
	result = float3_Min(_max, result);
	return result;
}
inline Float3 float3_ReciprocalSqrt(Float3 _v) {
	Float3 result;
	unsigned int i;

	result.x = 0.0f;

	for (i = 0; i < 3; ++i)
	{
		if (RINFWISNAN(_v.p[i]))
		{
			result.p[i] = 0x7FC00000;
		}
		else if (_v.p[i] == 0.0f || _v.p[i] == -0.0f)
		{
			result.p[i] = (float)(0x7F800000 | ((unsigned int) result.p[i] & 0x80000000));
		}
		else
		{
			result.p[i] = 1.0f / _v.p[i];
		}
	}
	return result;
}
inline Float3 float3_ACos(Float3 _v) {
	Float3 v2, v3, absV;
	Float3 C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11;
	Float3 R0, R1, R2, R3, R4;
	Float3 OneMinusAbsV;
	Float3 Rsq;
	Float3 Result;

	static Float3 OnePlusEpsilon = { 1.00000011921f, 1.00000011921f, 1.00000011921f };

	absV = float3_Abs(_v);

	v2 = _v * _v;
	v3 = v2 * absV;

	R4 = float3_NegativeMultiplySubtract(absV, _v, _v);

	OneMinusAbsV = OnePlusEpsilon - absV;
	Rsq = float3_ReciprocalSqrt(OneMinusAbsV);

	C0 = float3_Set(-0.05806367563904f, -0.05806367563904f, -0.05806367563904f);
	C1 = float3_Set(-0.41861972469416f, -0.41861972469416f, -0.41861972469416f);
	C2 = float3_Set(0.22480114791621f, 0.22480114791621f, 0.22480114791621f);
	C3 = float3_Set(2.17337241360606f, 2.17337241360606f, 2.17337241360606f);

	C4 = float3_Set(0.61657275907170f, 0.61657275907170f, 0.61657275907170f);
	C5 = float3_Set(4.29696498283455f, 4.29696498283455f, 4.29696498283455f);
	C6 = float3_Set(-1.18942822255452f, -1.18942822255452f, -1.18942822255452f);
	C7 = float3_Set(-6.53784832094831f, -6.53784832094831f, -6.53784832094831f);

	C8 = float3_Set(-1.36926553863413f,-1.36926553863413f,-1.36926553863413f);
	C9 = float3_Set(-4.48179294237210f, -4.48179294237210f, -4.48179294237210f);
	C10 = float3_Set(1.41810672941833f, 1.41810672941833f, 1.41810672941833f);
	C11 = float3_Set(5.48179257935713f, 5.48179257935713f, 5.48179257935713f);

	R0 = float3_MultiplyAdd(C3, absV, C7);
	R1 = float3_MultiplyAdd(C1, absV, C5);
	R2 = float3_MultiplyAdd(C2, absV, C6);
	R3 = float3_MultiplyAdd(C0, absV, C4);

	R0 = float3_MultiplyAdd(R0, absV, C11);
	R1 = float3_MultiplyAdd(R1, absV, C9);
	R2 = float3_MultiplyAdd(R2, absV, C10);
	R3 = float3_MultiplyAdd(R3, absV, C8);

	R0 = float3_MultiplyAdd(R2, v3, R0);
	R1 = float3_MultiplyAdd(R3, v3, R1);

	R0 = _v * R0;
	R1 = R4 * R1;

	Result = float3_MultiplyAdd(R1,Rsq,R0);
	Result = Float3(1.570796327f, 1.570796327f, 1.570796327f) - Result;

	return Result;
}

inline Float3 float3_AngleBetweenNormal(Float3 _v1, Float3 _v2)
{
	Float3 result;
	Float3 negativeOne;
	Float3 One;

	float dot = float3_Dot(_v1, _v2);
	result = float3_Set(dot, dot, dot);
	negativeOne = float3_Set(-1, -1, -1);
	One = float3_Set(1, 1, 1);

	result = float3_Clamp(result, negativeOne, One);
	result = float3_ACos(result);
	return result;
}
inline bool isZero(Float3 _v)
{
	if (abs(_v.x) <= FLT_EPSILON && abs(_v.y) <= FLT_EPSILON && abs(_v.z) <= FLT_EPSILON)
		return true;
	return false;
}

}
}
}