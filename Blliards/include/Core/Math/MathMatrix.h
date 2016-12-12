#pragma once

#include <./Core/Math/MathStruct.h>

#include <string.h>	//memset

#include <math.h>

namespace rinfw {
namespace core {
namespace math {


inline Matrix44 matrix44Transpose(Matrix44& _data) {

	Matrix44 ret;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			ret.m[col][row] = _data.m[row][col];
		}
	}

	return ret;
}

inline void matrix44Identity(Matrix44& _data) {
	memset(&_data, 0, sizeof(Matrix44));
	_data._11 = _data._22 = _data._33 = _data._44 = 1;
}

inline Matrix44& matrix44Translation(Matrix44& _out, float _x, float _y, float _z) {

	matrix44Identity(_out);
	_out._41 = _x;
	_out._42 = _y;
	_out._43 = _z;

	return _out;
}

inline Matrix44& matrix44Translation(Matrix44& _out,Float3 _vec) {
	matrix44Identity(_out);
	_out._41 = _vec.x;
	_out._42 = _vec.y;
	_out._43 = _vec.z;
	return _out;
}
 
inline Matrix44& matrix44RotationX(Matrix44& _out, float _degree) {

	float sin = sinf(_degree * PI / 180.0f);
	float cos = cosf(_degree * PI / 180.0f);
	
	matrix44Identity(_out);

	_out._22 = cos;
	_out._23 = -sin;
	_out._32 = sin;
	_out._33 = cos;

	return _out;
}

inline Matrix44& matrix44RotationY(Matrix44& _out, float _degree) {

	float sin = sinf(_degree * PI / 180.0f);
	float cos = cosf(_degree * PI / 180.0f);
	
	matrix44Identity(_out);

	_out._11 = cos;
	_out._13 = sin;
	_out._31 = -sin;
	_out._33 = cos;

	return _out;
}

inline Matrix44& matrix44RotationZ(Matrix44& _out, float _degree) {

	float sin = sinf(_degree * PI / 180.0f);
	float cos = cosf(_degree * PI / 180.0f);
	
	matrix44Identity(_out);

	_out._11 = cos;
	_out._12 = -sin;
	_out._21 = sin;
	_out._22 = cos;

	return _out;
}

inline Matrix44 matrix44Scaling(float _x,float _y, float _z) {
	Matrix44 out;
	matrix44Identity(out);
	out._11 = _x;
	out._22 = _y;
	out._33 = _z;
	
	return out;
}

inline Matrix44 matrix44LookAtLH(Float3 _camPos, Float3 _lookAt, Float3 _camUp) {
	Float3 zAxis = float3_Normalize(_lookAt - _camPos);
	Float3 xAxis = float3_Normalize(float3_Cross(_camUp, zAxis));
	Float3 yAxis = float3_Cross(zAxis, xAxis);

	Matrix44 ret;
	ret._11 = xAxis.x; ret._12 = yAxis.x; ret._13 = zAxis.x; ret._14 = 0;
	ret._21 = xAxis.y; ret._22 = yAxis.y; ret._23 = zAxis.y; ret._24 = 0;
	ret._31 = xAxis.z; ret._32 = yAxis.z; ret._33 = zAxis.z; ret._34 = 0;
	ret._41 = -float3_Dot(xAxis, _camPos); ret._42 = -float3_Dot(yAxis, _camPos); ret._43 = -float3_Dot(zAxis, _camPos); ret._44 = 1;

	return ret;
}

inline Matrix44 matrix44PerspectiveFovLH(float _FovAngleY, float _AspectRatio, float _nearZ, float _farZ) {
	float cosFov = cosf(_FovAngleY*0.5f);
	float sinFov =  sinf(_FovAngleY*0.5f);
	
	float fRange = _farZ / (_farZ - _nearZ);
	float height = cosFov / sinFov;
	


	Matrix44 ret;
	matrix44Identity(ret);
	ret._11 = height / _AspectRatio; ret._12 = 0; ret._13 = 0; ret._14 = 0;
	ret._21 = 0; ret._22 = height; ret._23 = 0; ret._24 = 0;
	ret._31 = 0; ret._32 = 0; ret._33 = fRange; ret._34 = 1;
	ret._41 = 0; ret._42 = 0; ret._43 = -fRange; ret._44 = 0;

	return ret;
}

inline Matrix44 matrix44OrthoGraphicLH(float _width, float _height, float _nearZ, float _farZ) {
	Matrix44 result;
	float range;

	range = 1.0f / (_farZ - _nearZ);

	result._11 = 2.0f / _width;
	result._12 = result._13 = result._14 = 0.0f;
	result._21 = 0.0f;
	result._22 = 2.0f / _height;
	result._23 = result._24 = 0.0f;
	result._31 = result._32 = 0.0f;
	result._33 = range;
	result._34 = 0.0f;
	result._41 = result._42 = 0.0f;
	result._43 = -range * _nearZ;
	result._44 = 1.0f;

	return result;
} 

}
}
}