#pragma once
#include <./Core/Manager.h>
#include <./Core/Math/Math.h>
#include <./Component/Model/Model.h>
#include <./Component/Transform.h>
#include <./Core/Manager.h>
#include <./Primitive/Sphere.h>

class Stage : rinfw::core::GameObject
{
public:
	void Create(std::wstring _filepath);
	bool Init()override;
	bool Update()override;
	bool Draw()override;
	bool Release()override;

	//���̍��W
	Sphere hole[6];

	//�X�e�[�W�̍��W
	rinfw::core::math::Float3 stageRightTop;
	rinfw::core::math::Float3 stageLeftTop;
	rinfw::core::math::Float3 stageRightBottom;
	rinfw::core::math::Float3 stageLeftBottom;
protected:
private:
	rinfw::component::Transform transform;
	SharedPtr<rinfw::core::Model> model;
	
};