#include <./GameObject/Stage.h>
#include <./Core/FileSystem/IO.h>
#include <./GPL/GPL.h>
using namespace rinfw::core;
using namespace rinfw::graphics;

void Stage::Create(std::wstring _filepath)
{
	filesystem::IO<Model>().Import(_filepath, model);
	transform.setScale(2,2,2);
	transform.rotate(0, 90, 0);
	transform.translate(0, -1.98f, 0);

	//åäç¿ïW
	hole[0].center = math::Float3(-1.61f, 0, 3.29f);
	hole[1].center = math::Float3(1.61f, 0, 3.29f);
	hole[2].center = math::Float3(-1.67f, 0, 0);
	hole[3].center = math::Float3(1.67f, 0, 0);
	hole[4].center = math::Float3(-1.61f, 0, -3.29f);
	hole[5].center = math::Float3(1.61f, 0, -3.29f);

	for (int i = 0; i < 6; ++i) {
		hole[i].r = 0.2f;
	}

	//ÉRÉäÉWÉáÉìÇÃê›íË
	this->stageRightTop = math::Float3(1.6f, 0, 3.2f);
	this->stageRightBottom = math::Float3(1.6f, 0, -3.2f);
	this->stageLeftTop = math::Float3(-1.6f, 0, 3.2f);
	this->stageLeftBottom = math::Float3(-1.6f, 0, -3.2f);
}
bool Stage::Init()
{
	return true;
}

bool Stage::Update()
{
	return true;
}

bool Stage::Draw()
{
	GPLModel& gplm = GPLModel::getInstance();
	

	gplm.Draw(*((rinfw::core::PmxModel*)model.get()),transform.getMatrix());

	return true;
}

bool Stage::Release() {
	return true;
}