#pragma once
#include <./Core/Manager.h>
#include <./Component/Model/Model.h>
#include <./Component/Transform.h>
#include <./Component/RigidBody.h>
#include <./Core/Manager.h>
#include <./Primitive/Sphere.h>

class Ball : rinfw::core::GameObject
{
public:
	void Create(std::wstring _filepath);
	void setID(int _id) {
		this->id = _id;
	}
	void AddForce(rinfw::core::math::Float3 _vec);
	bool Init()override;
	bool Update()override;
	bool Draw()override;
	bool Release()override;

	rinfw::component::Transform& getTransform(){
		return transform;
	};
	rinfw::component::RigidBody& getRigidBody() {
		return rigidbody;
	};
	Sphere& getSphere() { return sphere; }
	void IsFall(bool _frag) {
		this->isFall = _frag;
	}
	bool IsFall() {
		return this->isFall;
	}
protected:
	rinfw::component::Transform transform;
	rinfw::component::RigidBody rigidbody;
	SharedPtr<rinfw::core::Model> model;
	Sphere sphere;
	int id;
	bool isFall;
private:
};