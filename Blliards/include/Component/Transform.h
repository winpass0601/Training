#pragma once
#include <./Core/Manager.h>
#include <./Core/Math/Math.h>
#include <vector>
#include <functional>

namespace rinfw {

namespace component {
class Transform : public rinfw::core::Component {
public:
	Transform();
	~Transform();

	void Delete()override;
	void Release()override;
	void Create()override;

	

	//position
	Transform &translate(core::math::Float3 _trans);
	Transform &translate(float _x, float _y, float _z);
	Transform &setPosition(core::math::Float3 _pos) {
		this->position = _pos;
		for(int i = 0; i < onChangedCallBack.size();++i){
			if (onChangedCallBack[i] != nullptr)
				onChangedCallBack[i]();
		}
			
		return *this;
	};
	Transform &setPosition(float _x, float _y, float _z) {
		this->position.x = _x;
		this->position.y = _y;
		this->position.z = _z;
		for (int i = 0; i < onChangedCallBack.size(); ++i) {
			if (onChangedCallBack[i] != nullptr)
				onChangedCallBack[i]();
		}
		return *this;
	};
	core::math::Float3  &getPosition() {
		return this->position;
	};
	core::math::Float3 getPrevPosition() {
		return this->prevPosition;
	}


	//rotation
	Transform &rotate(core::math::Float3 _rot);
	Transform &rotate(float _x, float _y, float _z);
	Transform &setRotation(core::math::Float3 _rot) {
		this->rotation = _rot;
		return *this;
	};
	Transform &setRotation(float _x, float _y, float _z) {
		this->rotation.x = _x;
		this->rotation.y = _y;
		this->rotation.z = _z;

		return *this;
	};
	Transform &addRotation(core::math::Float3 _rot) {
		this->rotation += _rot;
		return *this;
	}
	Transform &addRotation(float _x, float _y, float _z) {
		this->rotation.x += _x;
		this->rotation.y += _y;
		this->rotation.z += _z;

		return *this;
	}
	core::math::Float3  &getRotation() {
		return this->rotation;
	};

	//scale
	//Transform &scaling(core::math::Float3 _scale);
	//Transform &scaling(float _x, float _y, float _z);
	Transform &setScale(core::math::Float3 _scale) {
		this->scale = _scale;
		return *this;
	};
	Transform &setScale(float _x, float _y, float _z) {
		this->scale.x = _x;
		this->scale.y = _y;
		this->scale.z = _z;

		return *this;
	};
	core::math::Float3  &getScale() {
		return this->scale;
	};

	void setParent(Transform* _parent) {
		parent = _parent;
	}
	core::math::Matrix44 getMatrix();
	void registChangedCallBack(std::function<void()> _func)  {
		this->onChangedCallBack.emplace_back(_func);
	}
	void UpdatePosition() {
		this->prevPosition = this->position;
	}

private:
	core::math::Float3 position;
	core::math::Float3 prevPosition;
	core::math::Float3 rotation;
	core::math::Float3 scale;

	Transform* parent;
	std::vector< std::function<void()> > onChangedCallBack;
};
}
}