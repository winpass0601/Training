#include <./GameObject/PlayerBall.h>
#include <./Core/FileSystem/IO.h>
#include <./Core/FPSManager.h>
#include <./GPL/GPL.h>
#include <./GPL/Line/GPLLine.h>
#include <./GUI.h>
using namespace rinfw::core;
using namespace rinfw::graphics;

#define RESPAWN_TIME (1.0f)

bool PlayerBall::Update()
{
	static math::Float3 vec(0, 0, 1.0f);
	static float power = 1.0f;
	static bool isPush = false;
	ImGui::Begin("pushVector");
	ImGui::DragFloat3("Vector", vec.p, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("Power", &power, 0.1f, 0.0f, 10.0f);
	ImGui::End();
	vec.y = 0;

	if (this->isFall)
	{
		if (tempFall == false)
		{
			respawnTimer = 0.0f;
		}
		respawnTimer += FPSManager::getInstance().getDeltaTime();
		if (respawnTimer > RESPAWN_TIME)
		{
			this->getRigidBody().setVelocity(math::Float3(0, 0, 0));
			this->getTransform().setPosition(0, 0, -2.0f);
			this->isFall = false;
		}
		else
			this->rigidbody.setVelocity(math::Float3(0, -1.0f, 0));
	}
	else
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			if (!isPush)
			{
				isPush = true;
				if(!math::isZero(vec))
					this->rigidbody.AddForce(math::float3_Normalize(vec)*power);
			}
		}
		else
		{
			isPush = false;
		}
	}
	this->rigidbody.Update(this->transform);
	tempFall = isFall;
	return true;
}

bool PlayerBall::Draw()
{
	GPLModel& gplm = GPLModel::getInstance();

	gplm.Draw(*((rinfw::core::PmxModel*)model.get()), transform.getMatrix());

	return true;
}