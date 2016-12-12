#pragma once
#include <./GameObject/Ball.h>

class PlayerBall : public Ball
{
private:
	bool tempFall;
	float respawnTimer;
public:
	bool Update()override;
	bool Draw()override;
};