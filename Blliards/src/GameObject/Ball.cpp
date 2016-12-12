#include <./GameObject/Ball.h>
#include <./Core/FileSystem/IO.h>
#include <./GPL/GPL.h>
#include <./GUI.h>
using namespace rinfw::core;
using namespace rinfw::graphics;

void Ball::Create(std::wstring _filepath)
{
	filesystem::IO<Model>().Import(_filepath, model);
	transform.setScale(0.8f, 0.8f, 0.8f);
	transform.translate(0, 1.0f, 0);
	sphere.center = transform.getPosition();
	transform.registChangedCallBack([=]() {this->getSphere().center = this->getTransform().getPosition(); });
	rigidbody.registUpdateEndCallBack(std::bind(std::mem_fn(&rinfw::component::Transform::UpdatePosition),&transform));
	isFall = false;
}
void Ball::AddForce(math::Float3 _vec)
{
	rigidbody.AddForce(_vec);
}

bool Ball::Init()
{
	return true;
}

bool Ball::Update()
{
	if (isFall)
	{
		this->rigidbody.setVelocity(math::Float3(0, -1.0f, 0));
	}
	this->rigidbody.Update(this->transform);
	return true;
}

bool Ball::Draw()
{
	GPLModel& gplm = GPLModel::getInstance();

	gplm.Draw(*((rinfw::core::PmxModel*)model.get()),transform.getMatrix());

	return true;
}

bool Ball::Release() {
	return true;
}