#include <./Component/RigidBody.h>
#include <./Core/FPSManager.h>

using namespace rinfw;
using namespace component;
using namespace core::math;

void RigidBody::Update(Transform& _transform)
{      
	core::FPSManager fpsManager = core::FPSManager::getInstance();
	//ˆÚ“®
	_transform.translate(this->velocity * fpsManager.getDeltaTime());
	//—Í‚ðXV
	this->power -= 0.8f * fpsManager.getDeltaTime();
	if (this->power < 0.0f)
		this->power = 0.0f;
	this->prevVelocity = this->velocity;
	this->velocity = this->angleVector *this->power;

	for (int i = 0; i < this->onUpdateEnd.size(); ++i) {
		if (this->onUpdateEnd[i] != nullptr)
			this->onUpdateEnd[i]();
	}
}
void RigidBody::AddForce(Float3 _force)
{
	if (float3_Length(_force) == 0)
		return;
	this->setPower(float3_Length(_force));
	this->setAngle(float3_Normalize(_force));
}
core::math::Float3 RigidBody::getMoveLength(float _per) {
	return (this->prevVelocity * _per) * core::FPSManager::getInstance().getDeltaTime();
}

void RigidBody::Create() {
}

void RigidBody::Release() {
}

void RigidBody::Delete() {
	delete this;
}