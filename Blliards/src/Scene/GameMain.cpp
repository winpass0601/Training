#include <./Scene/GameMain.h>
#include <./Core/FPSManager.h>
#include <./GUI.h>
#include <./Device.h>
#include <./Core/FileSystem/IO.h>
#include <./GPL/Line/GPLLine.h>

using namespace rinfw::core;
using namespace rinfw::scene;
using namespace rinfw::graphics;

#define NINEBALL (9)
#define HITDIVIDE (20.0f)

math::Float3 initPos[10] =
{
	{math::Float3(0,0,-2.0f)},//����

	{math::Float3(0,0,1.0f)},//�P�ԁi�ȉ��~��

	{math::Float3(-0.09f,0,1.18f)},
	{math::Float3(0.09f,0,1.18f)},

	{math::Float3(-0.18f,0,1.36f)},
	{math::Float3(0.18f,0,1.36f)},
	{math::Float3(-0.09f,0,1.52f)},

	{math::Float3(0.09f,0,1.52f)},
	{math::Float3(0,0,1.68f)},

	{math::Float3(0.0f,0,1.36f)},
};

void HitCheckBall(SharedListPtr<Ball> ball);
void HitCheckHall(SharedListPtr<Ball> ball, SharedPtr<Stage> stage);
void HitCheckWall(SharedListPtr<Ball> ball, SharedPtr<Stage> stage);

GameMain::GameMain() {

}
GameMain::~GameMain() {

}
bool GameMain::Init() {
	DX11Device &device = DX11Device::getInstance();
	GPLModel &gplm = GPLModel::getInstance();
	GPLLine &gplL = GPLLine::getInstance();

	SharedPtr<Stage> stg(new Stage());
	stage = std::move(stg);
	stage->Create(L"Resource/Model/�r�����[�h��/stage.pmx");

	for (int i = 0; i < 1 + NINEBALL; ++i) {
		SharedPtr<Ball> tmpBall;
		if (i == 0)
		{
			SharedPtr<PlayerBall> t(new PlayerBall());
			tmpBall = std::move(t);
		}
		else
		{
			SharedPtr<Ball> t(new Ball());
			tmpBall = std::move(t);
		}
		std::wstring filepath = L"Resource/Model/�r�����[�h��/" + std::to_wstring(i);
		tmpBall->Create(filepath + L".pmx");
		tmpBall->getTransform().setPosition(initPos[i]);
		tmpBall->getSphere().center = tmpBall->getTransform().getPosition();
		tmpBall->getSphere().r = 0.08f;
		tmpBall->setID(i);
		this->ball.push_back(std::move(tmpBall));
	}

	if (!gplm.createPipeLine(device.getDevice())) {
		assert("error");
	}
	if (!gplL.createPipeLine(device.getDevice())) {
		assert("error");
	}
	return true;
}
bool GameMain::Release() {

	return true;
}
bool GameMain::Update() {
	for (auto it = ball.begin(); it != ball.end(); ++it) {
		(*it)->Update();
	}

	//�����蔻��
	HitCheckBall(ball);
	HitCheckHall(ball, stage);
	HitCheckWall(ball, stage);
	return true;
}
bool GameMain::Draw() {

	stage->Draw();

	auto it = ball.begin();
	for (int i = 0; i < NINEBALL + 1; ++i) {
		(*it)->Draw();
		++it;
	}

	math::Float3 s[2];
	s[0] = math::Float3(0, 0, 0);
	s[1] = math::Float3(0, 0, 1);

	return true;
}



//���Ƌ��̓����蔻��
bool SphereCast(SharedPtr<Ball> v1, SharedPtr<Ball> v2, math::Float3& _pos1, math::Float3& _pos2);
void PushBack(SharedPtr<Ball> v1, SharedPtr<Ball> v2, math::Float3 p1, math::Float3 p2);
void CalculateVector(SharedPtr<Ball> v1, SharedPtr<Ball> v2, math::Float3 p1, math::Float3 p2);

void HitCheckBall(SharedListPtr<Ball> ball)
{
	for (auto it = ball.begin(); it != ball.end(); ++it) {
		for (auto itE = ball.begin(); itE != ball.end(); ++itE)
		{
			//�Ώۂ������I�u�W�F�N�g�Ȃ画������Ȃ�
			if (it == itE || (*it)->IsFall() || (*itE)->IsFall())
				continue;

			//�C�e���[�^�[���g���₷���悤�ɃL���b�V��
			SharedPtr<Ball> v1 = (*it);
			SharedPtr<Ball> v2 = (*itE);

			//�X�t�B�A�L���X�g�Ŋђʑ΍�i���S�ł͂Ȃ��j
			math::Float3 p1, p2;
			if (!SphereCast(v1, v2, p1, p2))
				continue;

			//�ꎞ�I��HIT�����^�C�~���O�̃|�W�V�����Ɉړ��i���̌�Translate���邽�߁j
			v1->getTransform().setPosition(p1);
			v2->getTransform().setPosition(p2);

			PushBack(v1, v2, p1, p2);

			//�����Ԃ������W���擾���Ȃ���
			p1 = v1->getTransform().getPosition();
			p2 = v2->getTransform().getPosition();

			CalculateVector(v1, v2, p1, p2);
		}
	}
}

bool SphereCast(SharedPtr<Ball> v1, SharedPtr<Ball> v2, math::Float3& _pos1, math::Float3& _pos2)
{
	math::Float3 checkPos1;
	math::Float3 checkPos2;
	bool isHit = false;
	//�O���[�v�̍��W����
	//�ړ��ʂ̊����� 1.0f/HITDIVIDE ���i�߂čs���ē������Ă邩�`�F�b�N(HITDIVIDE�񔻒�����j
	for (float perFrame = 0.0f; perFrame < 1.0f; perFrame += 1.0f / HITDIVIDE)
	{
		checkPos1 = v1->getTransform().getPrevPosition() + v1->getRigidBody().getMoveLength(perFrame);
		checkPos2 = v2->getTransform().getPrevPosition() + v2->getRigidBody().getMoveLength(perFrame);
		math::Float3 checkLength = checkPos2 - checkPos1;
		if (powf(checkLength.x, 2) + powf(checkLength.y, 2) + powf(checkLength.z, 2) < powf(v1->getSphere().r + v2->getSphere().r, 2)) {
			isHit = true;
			break;
		}
	}
	_pos1 = checkPos1;
	_pos2 = checkPos2;
	return isHit;
}

void PushBack(SharedPtr<Ball> v1, SharedPtr<Ball> v2, math::Float3 p1, math::Float3 p2)
{
	//�e�����̍�
	float vx, vy;
	vx = p1.x - p2.x;
	vy = p1.z - p2.z;

	//�������擾
	float Length = math::float3_Length(p1 - p2);

	//�߂荞��ł��鋗�����擾
	float distance;
	distance = (v1->getSphere().r + v2->getSphere().r) - Length;
	if (Length > 0)Length = 1.0f / Length;
	vx *= (Length + 0.001f);
	vy *= (Length + 0.001f);

	//���ۂɉ����Ԃ�
	distance /= 2.0f;
	v1->getTransform().translate(math::Float3(vx*distance, 0, vy*distance));
	v2->getTransform().translate(math::Float3(-(vx*distance), 0, -(vy * distance)));
}

void CalculateVector(SharedPtr<Ball> v1, SharedPtr<Ball> v2, math::Float3 p1, math::Float3 p2)
{
	//�O�t���[���̉^���x�N�g�����擾
	math::Float3 v1Velo = v1->getRigidBody().getPrevVelocity();
	math::Float3 v2Velo = v2->getRigidBody().getPrevVelocity();

	//���ˌ�x�N�g���v�Z�P
	math::Float3 AB = p2 - p1;
	//�Փ˕����x�N�g���P
	math::Float3 ABNorm = math::float3_Normalize(AB);
	//�{�[���P�̉�]�x�N�g��
	math::Float3 rot1 = ABNorm * math::float3_Dot(v1Velo, ABNorm);
	//�{�[���P�̉^���x�N�g��
	math::Float3 trans1 = v1Velo - rot1;

	//���ˌ�x�N�g���v�Z�Q
	math::Float3 BA = p1 - p2;
	//�Փ˕����x�N�g���Q
	math::Float3 BANorm = math::float3_Normalize(BA);
	//�{�[���Q�̉�]�x�N�g��
	math::Float3 rot2 = BANorm * math::float3_Dot(v2Velo, BANorm);
	//�{�[���Q�̉^���x�N�g��
	math::Float3 trans2 = v2Velo - rot2;

	//��]�x�N�g���̌���
	math::Float3 tmp = rot1;
	rot1 = rot2;
	rot2 = tmp;

	//�ړ������Ɨ͂��Z�b�g�i�����ŉ^���x�N�g�����v�Z���Ă���j
	v1->getRigidBody().setAngle(math::float3_Normalize(rot1 + trans1));
	v1->getRigidBody().setPower(math::float3_Length(rot1 + trans1));
	v2->getRigidBody().setAngle(math::float3_Normalize(rot2 + trans2));
	v2->getRigidBody().setPower(math::float3_Length(rot2 + trans2));
}

//�ǂƌ��̓����蔻��
void HitCheckHall(SharedListPtr<Ball> ball, SharedPtr<Stage> stage)
{
	for (auto it = ball.begin(); it != ball.end(); ++it) {
		if ((*it)->IsFall())
		{
			continue;
		}
		for (int i = 0; i < 6; ++i)
		{
			math::Float3 p1 = (*it)->getTransform().getPosition();
			math::Float3 length = stage->hole[i].center - p1;
			//�{�[���̒��S�_�����̓����ɂ���Η����Ă锻��
			if (powf(length.x, 2) + powf(length.y, 2) + powf(length.z, 2) <= powf(stage->hole[i].r, 2)) {
				(*it)->getRigidBody().setVelocity(math::Float3(0, 0, 0));
				(*it)->getTransform().setPosition(stage->hole[i].center);
				(*it)->IsFall(true);
			}
		}
	}
}

//�ǂƋ��̓����蔻��
void PushWallX(SharedPtr<Ball> ball, float _perLength);
void PushWallZ(SharedPtr<Ball> ball, float _perLength);
void CheckRightWall(SharedPtr<Ball> ball, SharedPtr<Stage> stage);
void CheckLeftWall(SharedPtr<Ball> ball, SharedPtr<Stage> stage);
void CheckTopWall(SharedPtr<Ball> ball, SharedPtr<Stage> stage);
void CheckBottomWall(SharedPtr<Ball> ball, SharedPtr<Stage> stage);

void HitCheckWall(SharedListPtr<Ball> ball, SharedPtr<Stage> stage)
{
	//�@�e�ǂƏՓ˂��Ă邩�ǂ������`�F�b�N
	//�A�^���x�N�g�����Ǒ��Ɍ����Ă邩�ǂ������`�F�b�N
	//�B�{�[���̒��S���ǂ̓������O�������`�F�b�N
	//�C�����̌����_���W���Z�o
	//�D�{�[���̒��S���狅�̑傫�����A�^���x�N�g���Ƌt�����Ɉړ�
	//�E�^���x�N�g���𔽎˂����ďI��
	for (auto it = ball.begin(); it != ball.end(); ++it)
	{
		if ((*it)->IsFall())
			continue;
		//�E��
		CheckRightWall((*it), stage);

		//����
		CheckLeftWall((*it), stage);

		//���
		CheckTopWall((*it), stage);

		//����
		CheckBottomWall((*it), stage);
	}
}

void PushWallX(SharedPtr<Ball> ball, float _perLength)
{
	math::Float3 pos;
	pos = ball->getTransform().getPosition() + (ball->getRigidBody().getVelocity() * _perLength);
	pos = pos + ball->getRigidBody().getAngle() * (-ball->getSphere().r);

	ball->getTransform().setPosition(pos);

	math::Float3 velocity = ball->getRigidBody().getVelocity();
	velocity.x = -velocity.x;
	ball->getRigidBody().setVelocity(velocity);
}

void PushWallZ(SharedPtr<Ball> ball, float _perLength)
{
	math::Float3 pos;
	pos = ball->getTransform().getPosition() + (ball->getRigidBody().getVelocity() * _perLength);
	pos = pos + ball->getRigidBody().getAngle() * (-ball->getSphere().r);

	ball->getTransform().setPosition(pos);

	math::Float3 velocity = ball->getRigidBody().getVelocity();
	velocity.z = -velocity.z;
	ball->getRigidBody().setVelocity(velocity);
}

void CheckRightWall(SharedPtr<Ball> ball, SharedPtr<Stage> stage)
{
	if (ball->getTransform().getPosition().z > stage->stageRightTop.z || ball->getTransform().getPosition().z < stage->stageRightBottom.z)
		return;
	if (ball->getTransform().getPosition().x + ball->getSphere().r >= stage->stageRightBottom.x)
	{
		//�x�N�g�����Ǖ����Ɍ����Ă��Ȃ���Δ���I��
		math::Float3 AB = stage->stageRightTop - stage->stageRightBottom;
		float c = math::float3_Cross(AB, ball->getRigidBody().getVelocity()).y;
		if (c < 0)
			return;

		float crs = 0.0f;
		//�����߂荞��ł邾�����A���S�ɉE�ɏo�Ă邩���`�F�b�N
		//�ǂ̐����ƕǂ̕Е����猩���{�[���ւ̃x�N�g���̊O�ς̂x�̒l��
		//���̐��Ȃ炽���߂荞��ł���A���̐��Ȃ犮�S�ɉE�ɏo�Ă���	
		math::Float3 V = ball->getTransform().getPosition() - stage->stageRightBottom;
		math::Float3 V1 = AB;
		math::Float3 V2 = ball->getRigidBody().getVelocity();

		//�قڕǂ���𔻒�
		float k = math::float3_Angle(V, V2);
		if (k < 20.0f || k > 160.0f)
		{
			math::Float3 pos = ball->getTransform().getPosition();
			pos.x = stage->stageRightTop.x - ball->getSphere().r - 0.0001f;
			ball->getTransform().setPosition(pos);

			math::Float3 velo = ball->getRigidBody().getVelocity();
			velo.x = -velo.x;
			ball->getRigidBody().setVelocity(velo);

			return;
		}

		math::Float3 cross = math::float3_Cross(AB, V);
		if (-1.0f < cross.y && cross.y < 1.0f)
		{
			//�{�[���������Ɏ����Ă���
			if (cross.y <= 0)
			{
				//�����߂荞��ł鎞
				float check = math::float3_Cross(V1, V2).y;
				if (check >= -FLT_EPSILON && check <= FLT_EPSILON)
					crs = 0;
				else
					crs = math::float3_Cross(V, V1).y / check;
			}
			else
			{
				//���S�ɊO�ɍs�����Ƃ�
				//�u�Q�𔽓]
				V2.x = -V2.x;
				V2.z = -V2.z;
				//�����߂荞��ł鎞
				float check = math::float3_Cross(V1, V2).y;
				if (check >= -FLT_EPSILON && check <= FLT_EPSILON)
					crs = 0;
				else
					crs = math::float3_Cross(V, V1).y / check;
			}
			PushWallX(ball, crs);
		}
	}
}

void CheckLeftWall(SharedPtr<Ball> ball, SharedPtr<Stage> stage)
{
	if (ball->getTransform().getPosition().z > stage->stageRightTop.z || ball->getTransform().getPosition().z < stage->stageRightBottom.z)
		return;
	if (ball->getTransform().getPosition().x - ball->getSphere().r <= stage->stageLeftBottom.x)
	{
		//�x�N�g�����Ǖ����Ɍ����Ă��Ȃ���Δ���I��
		math::Float3 AB = stage->stageLeftTop - stage->stageLeftBottom;
		float c = math::float3_Cross(AB, ball->getRigidBody().getVelocity()).y;
		if (c > 0)
			return;

		float crs = 0.0f;
		//�����߂荞��ł邾�����A���S�ɉE�ɏo�Ă邩���`�F�b�N
		//�ǂ̐����ƕǂ̕Е����猩���{�[���ւ̃x�N�g���̊O�ς̂x�̒l��
		//���̐��Ȃ炽���߂荞��ł���A���̐��Ȃ犮�S�ɉE�ɏo�Ă���
		math::Float3 V = ball->getTransform().getPosition() - stage->stageLeftBottom;
		math::Float3 V1 = AB;
		math::Float3 V2 = ball->getRigidBody().getVelocity();

		//�قڕǂ���𔻒�
		float k = math::float3_Angle(V, V2);
		if (k < 20.0f || k > 160.0f) {
			math::Float3 pos = ball->getTransform().getPosition();
			pos.x = stage->stageLeftTop.x + ball->getSphere().r + 0.0001f;
			ball->getTransform().setPosition(pos);

			math::Float3 velo = ball->getRigidBody().getVelocity();
			velo.x = -velo.x;
			ball->getRigidBody().setVelocity(velo);
			return;
		}

		math::Float3 cross = math::float3_Cross(AB, V);
		//�{�[���������Ɏ����Ă���
		if (cross.y >= 0)
		{
			//�����߂荞��ł鎞
			float check = math::float3_Cross(V1, V2).y;
			if (check >= -FLT_EPSILON && check <= FLT_EPSILON)
				crs = 0;
			else
				crs = math::float3_Cross(V, V1).y / check;
		}
		else
		{
			//���S�ɊO�ɍs�����Ƃ�
			//�u�Q�𔽓]
			V2.x = -V2.x;
			V2.z = -V2.z;
			float check = math::float3_Cross(V1, V2).y;
			if (check >= -FLT_EPSILON && check <= FLT_EPSILON)
				crs = 0;
			else
				crs = math::float3_Cross(V, V1).y / check;
		}
		PushWallX(ball, crs);
	}
}

void CheckTopWall(SharedPtr<Ball> ball, SharedPtr<Stage> stage)
{
	if (ball->getTransform().getPosition().x > stage->stageRightTop.x || ball->getTransform().getPosition().x < stage->stageLeftTop.x)
		return;
	if (ball->getTransform().getPosition().z + ball->getSphere().r >= stage->stageLeftTop.z)
	{
		//�x�N�g�����Ǖ����Ɍ����Ă��Ȃ���Δ���I��
		math::Float3 AB = stage->stageRightTop - stage->stageLeftTop;
		float c = math::float3_Cross(AB, ball->getRigidBody().getVelocity()).y;
		if (c > 0)
			return;

		float crs = 0.0f;
		//�����߂荞��ł邾�����A���S�ɉE�ɏo�Ă邩���`�F�b�N
		//�ǂ̐����ƕǂ̕Е����猩���{�[���ւ̃x�N�g���̊O�ς̂x�̒l��
		//���̐��Ȃ炽���߂荞��ł���A���̐��Ȃ犮�S�ɉE�ɏo�Ă���
		math::Float3 V = ball->getTransform().getPosition() - stage->stageLeftTop;
		math::Float3 V1 = AB;
		math::Float3 V2 = ball->getRigidBody().getVelocity();
		//�قڕǂ���𔻒�
		float k = math::float3_Angle(V, V2);
		if (k < 20.0f || k > 160.0f) {
			math::Float3 pos = ball->getTransform().getPosition();
			pos.z = stage->stageLeftTop.z - ball->getSphere().r - 0.0001f;
			ball->getTransform().setPosition(pos);

			math::Float3 velo = ball->getRigidBody().getVelocity();
			velo.z = -velo.z;
			ball->getRigidBody().setVelocity(velo);
			return;
		}

		math::Float3 cross = math::float3_Cross(AB, V);
		//�{�[���������Ɏ����Ă���
		if (cross.y >= 0)
		{
			//�����߂荞��ł鎞
			float check = math::float3_Cross(V1, V2).y;
			if (check >= -FLT_EPSILON && check <= FLT_EPSILON)
				crs = 0;
			else
				crs = math::float3_Cross(V, V1).y / check;
		}
		else
		{
			//���S�ɊO�ɍs�����Ƃ�
			//�u�Q�𔽓]
			V2.x = -V2.x;
			V2.z = -V2.z;
			//�����߂荞��ł鎞
			float check = math::float3_Cross(V1, V2).y;
			if (check >= -FLT_EPSILON && check <= FLT_EPSILON)
				crs = 0;
			else
				crs = math::float3_Cross(V, V1).y / check;
		}
		PushWallZ(ball, crs);
	}
}

void CheckBottomWall(SharedPtr<Ball> ball, SharedPtr<Stage> stage)
{
	if (ball->getTransform().getPosition().x > stage->stageRightBottom.x || ball->getTransform().getPosition().x < stage->stageLeftBottom.x)
		return;
	if (ball->getTransform().getPosition().z - ball->getSphere().r <= stage->stageLeftBottom.z)
	{
		//�x�N�g�����Ǖ����Ɍ����Ă��Ȃ���Δ���I��
		math::Float3 AB = stage->stageRightBottom - stage->stageLeftBottom;
		float c = math::float3_Cross(AB, ball->getRigidBody().getVelocity()).y;
		if (c < 0)
			return;

		float crs = 0.0f;
		//�����߂荞��ł邾�����A���S�ɉE�ɏo�Ă邩���`�F�b�N
		//�ǂ̐����ƕǂ̕Е����猩���{�[���ւ̃x�N�g���̊O�ς̂x�̒l��
		//���̐��Ȃ炽���߂荞��ł���A���̐��Ȃ犮�S�ɉE�ɏo�Ă���
		math::Float3 V = ball->getTransform().getPosition() - stage->stageLeftBottom;
		math::Float3 V1 = AB;
		math::Float3 V2 = ball->getRigidBody().getVelocity();

		//�قڕǂ���𔻒�
		float k = math::float3_Angle(V, V2);
		if (k < 20.0f || k > 160.0f) {
			math::Float3 pos = ball->getTransform().getPosition();
			pos.z = stage->stageLeftBottom.z + ball->getSphere().r + 0.0001f;
			ball->getTransform().setPosition(pos);

			math::Float3 velo = ball->getRigidBody().getVelocity();
			velo.z = -velo.z;
			ball->getRigidBody().setVelocity(velo);
			return;
		}

		math::Float3 cross = math::float3_Cross(AB, V);
		//�{�[���������Ɏ����Ă���
		if (cross.y <= 0)
		{
			//�����߂荞��ł鎞
			float check = math::float3_Cross(V1, V2).y;
			if (check >= -FLT_EPSILON && check <= FLT_EPSILON)
				crs = 0;
			else
				crs = math::float3_Cross(V, V1).y / check;
		}
		else
		{
			//���S�ɊO�ɍs�����Ƃ�
			//�u�Q�𔽓]
			V2.x = -V2.x;
			V2.z = -V2.z;
			//�����߂荞��ł鎞
			float check = math::float3_Cross(V1, V2).y;
			if (check >= -FLT_EPSILON && check <= FLT_EPSILON)
				crs = 0;
			else
				crs = math::float3_Cross(V, V1).y / check;
		}
		PushWallZ(ball, crs);
	}
}