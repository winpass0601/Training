#include <./Component/Transform.h>
#include <windows.h>

using namespace rinfw;
using namespace component;
using namespace core::math;

void PrintFloat3(Float3& _data) {
	printf("[X:%f , Y:%f , Z:%f]\n", _data.x, _data.y, _data.z);
}
void Around(Float3& _data) {
	for (float &v : _data.p) {
		if (v > 360)
			v -= 360;
		else if (v < 0)
			v += 360;
	}
}

Transform::Transform() {
	//メンバーは各コンストラクタで初期化されるが
	//スケール値は初期値０ではなく１
	this->scale = Float3(1, 1, 1);
}
Transform::~Transform() {

}

void Transform::Create() {
}

void Transform::Release() {
}

void Transform::Delete() {
	delete this;
}

Transform &Transform::translate(Float3 _trans)
{
	this->position += _trans;
	for (int i = 0; i < onChangedCallBack.size(); ++i) {
		if (onChangedCallBack[i] != nullptr)
			onChangedCallBack[i]();
	}
	return *this;
}

Transform &Transform::translate(float _x, float _y, float _z)
{
	this->position.x += _x;
	this->position.y += _y;
	this->position.z += _z;
	for (int i = 0; i < onChangedCallBack.size(); ++i) {
		if (onChangedCallBack[i] != nullptr)
			onChangedCallBack[i]();
	}
	return *this;
}

Transform &Transform::rotate(Float3 _rot) {

	this->rotation += _rot;

	//３６０度で管理
	Around(this->rotation);

	return *this;
}

Transform &Transform::rotate(float _x, float _y, float _z)
{
	this->rotation.x += _x;
	this->rotation.y += _y;
	this->rotation.z += _z;

	//３６０度で管理
	Around(this->rotation);

	return *this;
}

Matrix44 Transform::getMatrix()
{
	core::math::Matrix44 res;
	core::math::Matrix44 rotX, rotY, rotZ, rot;
	core::math::Matrix44 scal;
	core::math::Matrix44 trans;
	core::math::matrix44RotationX(rotX, rotation.x);
	core::math::matrix44RotationY(rotY, rotation.y);
	core::math::matrix44RotationZ(rotZ, rotation.z);

	rot = rotX * rotZ * rotY;

	scal = core::math::matrix44Scaling(scale.x, scale.y, scale.z);

	core::math::matrix44Translation(trans, position);

	res = rot * scal  * trans;
	if (parent != nullptr)
		res *= parent->getMatrix();
	return res;
}

//Transform &Transform::scaling(Float3 _scale) {
//	printf("scale変更前->");
//	PrintFloat3(this->scale);
//
//	this->scale += _scale;
//
//	printf("scale変更後->");
//	PrintFloat3(this->scale);
//	return *this;
//}
//
//Transform &Transform::scaling(float _x, float _y, float _z)
//{
//	printf("scale変更前->");
//	PrintFloat3(this->scale);
//
//	this->scale.x += _x;
//	this->scale.y += _y;
//	this->scale.z += _z;
//
//	printf("scale変更後->");
//	PrintFloat3(this->scale);
//
//	return *this;
//}
