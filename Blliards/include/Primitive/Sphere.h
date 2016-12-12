#pragma once
#include <./Core/Math/Math.h>

struct Sphere
{
	rinfw::core::math::Point center;	//���S�_
	float r;	//���a

	Sphere() {}
	Sphere(rinfw::core::math::Point p,float _r):center(p),r(_r){}
	~Sphere(){}
};
