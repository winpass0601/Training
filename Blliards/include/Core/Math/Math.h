#pragma once
#pragma warning(disable:4305)
#include <./Core/Math/MathFloat3.h>
#include <./Core/Math/MathFloat4.h>
#include <./Core/Math/MathMatrix.h>

namespace rinfw {
namespace core {
namespace math {
inline float RINFWDegree(float _radian) {
	return _radian * (180.0f / PI);
}
inline float RINFWRadian(float _degree) {
	return _degree * (PI / 180.0f);
}

inline Float3 float3Transform(Matrix44 _mat, Float3 _vec) {
	Float3 X(_vec.x, _vec.x, _vec.x);
	Float3 Y(_vec.y, _vec.y, _vec.y);
	Float3 Z(_vec.z, _vec.z, _vec.z);
	Float3 result;

	result = float3_MultiplyAdd(Z, Float3(_mat.m[2][0], _mat.m[2][1], _mat.m[2][2]), Float3(_mat.m[3][0], _mat.m[3][1], _mat.m[3][2]));
	result = float3_MultiplyAdd(Y, Float3(_mat.m[1][0], _mat.m[1][1], _mat.m[1][2]), result);
	result = float3_MultiplyAdd(X, Float3(_mat.m[0][0], _mat.m[0][1], _mat.m[0][2]), result);

	return result;
}
inline Matrix44& matrix44RotationAxis(Float3 _axis, float _deg) {
	if (float3_Equal(_axis, Float3(0, 0, 0))) {
		Matrix44 ret;
		matrix44Identity(ret);
		return ret;
	}
	
	_axis = float3_Normalize(_axis);
	

	float sinAngle = sinf(RINFWRadian(_deg));
	float cosAngle = cosf(RINFWRadian(_deg));
	float negativeCos = 1.0f - cosAngle;
	float XY = _axis.x * _axis.y;
	float ZX = _axis.z * _axis.x;
	float YZ = _axis.y * _axis.z;
	float XX = _axis.x * _axis.x;
	float YY = _axis.y * _axis.y;
	float ZZ = _axis.z * _axis.z;
	float XSin = _axis.x * sinAngle;
	float YSin = _axis.y * sinAngle;
	float ZSin = _axis.z * sinAngle;

	Matrix44 result;
	result._11 = XX * negativeCos + cosAngle;
	result._12 = XY * negativeCos + ZSin;
	result._13 = ZX * negativeCos - YSin;
	result._14 = 0.0f;

	result._21 = XY * negativeCos - ZSin;
	result._22 = YY * negativeCos + cosAngle;
	result._23 = YZ * negativeCos + XSin;
	result._24 = 0.0f;

	result._31 = ZX *negativeCos + YSin;
	result._32 = YZ * negativeCos - XSin;
	result._33 = ZZ * negativeCos + cosAngle;
	result._34 = 0;
	
	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
	
	return result;
} 



}
}
}