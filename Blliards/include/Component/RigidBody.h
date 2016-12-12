#pragma once
#include <./Core/Manager.h>
#include <./Component/Transform.h>
#include <./Core/Math/Math.h>
#include <vector>
#include <functional>

namespace rinfw {
namespace component {
class RigidBody :public rinfw::core::Component {
public:
	RigidBody() {}
	~RigidBody() {}

	void Delete()override;
	void Release()override;
	void Create()override;

	void Update(Transform& _transform);

	void AddForce(core::math::Float3 _force);

	void setAngle(core::math::Float3 _normVec) {
		if (isnan(_normVec.x) || isnan(_normVec.y) || isnan(_normVec.z))
			this->angleVector = core::math::Float3(0, 0, 0);
		else
			this->angleVector = _normVec;
		this->velocity = this->angleVector * this->power;
	}
	core::math::Float3 getAngle() {
		return this->angleVector;
	}
	core::math::Float3 getVelocity() {
		return this->velocity;
	}
	core::math::Float3 getPrevVelocity() {
		return this->prevVelocity;
	}
	void setPower(float _pow) {
		this->power = _pow;
		this->velocity = this->angleVector * this->power;
	}
	float getPower() {
		return this->power;
	}
	void setVelocity(core::math::Float3 _velocity) {
		if (core::math::isZero(_velocity))
		{
			this->setAngle(core::math::Float3(0, 0, 0));
			this->setPower(0.0f);
		}
		else
		{
			this->setAngle(core::math::float3_Normalize(_velocity));
			this->setPower(core::math::float3_Length(_velocity));
		}
	}
	//‚PƒtƒŒ[ƒ€‚ÉˆÚ“®‚·‚éˆÚ“®—Ê‚ğŠ„‡‚Å•Ô‚·
	core::math::Float3 getMoveLength(float _per);

	void registUpdateEndCallBack(std::function<void()> _func) {
		this->onUpdateEnd.emplace_back(_func);
	}

private:
	core::math::Float3 velocity;
	core::math::Float3 prevVelocity;
	core::math::Float3 angleVector;
	float power;
	std::vector< std::function<void()> > onUpdateEnd;

};
}
}