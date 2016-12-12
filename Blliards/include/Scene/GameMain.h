#pragma once
#include <./Macro.h>
#include <./Core/Manager.h>
#include <./GPL/GPL.h>
#include <./Component/Model/Model.h>
#include <./GameObject/PlayerBall.h>
#include <./GameObject/Stage.h>

namespace rinfw {
namespace scene {
class GameMain : public core::Scene{
public:
	GameMain();
	~GameMain() override;
	bool Init() override;
	bool Release() override;
	bool Update() override;
	bool Draw() override;

protected:
private:

	SharedPtr<Stage> stage;
	SharedListPtr<Ball> ball;
};
}
}