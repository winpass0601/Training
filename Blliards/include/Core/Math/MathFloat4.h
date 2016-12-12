#pragma once

#include <./Core/Math/MathStruct.h>
#include <math.h>
#include <Windows.h>

namespace rinfw {
namespace core {
namespace math {

inline Float4 float4_Set(float _x, float _y, float _z,float _w) {
	Float4 ret(_x, _y, _z,_w);
	return ret;
}
inline Float4 float4_Multiply(Float4 _a, Float4 _b) {
	Float4 result;
	result.x = _a.x * _b.x;
	result.y = _a.y * _b.y;
	result.z = _a.z * _b.z;
	result.w = _a.w * _b.w;
	return result; 
}
inline Float4 float4_MultiplyAdd(Float4 _a, Float4 _b, Float4 _c) {
	Float4 result;

	result.x = (_a.x * _b.x) + _c.x;
	result.y = (_a.y * _b.y) + _c.y;
	result.z = (_a.z * _b.z) + _c.z;
	result.w = (_a.w * _b.w) + _c.w;
	return result;
}
inline Float4 float4_NegativeMultiplySubtract(Float4 _v1, Float4 _v2, Float4 _v3) {
	Float4 result;

	result.x = _v3.x - (_v1.x * _v2.x);
	result.y = _v3.y - (_v1.y * _v2.y);
	result.z = _v3.z - (_v1.z * _v2.z);
	result.w = _v3.w - (_v1.w * _v2.w);
	return result;
}


}
}
}
