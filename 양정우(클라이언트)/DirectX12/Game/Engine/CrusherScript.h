#pragma once

#include "MonoBehaviour.h"
#include "Quaternion.h"
struct Quaternion;

class CrusherScript : public MonoBehaviour
{
public:
	CrusherScript();
	virtual ~CrusherScript();

	virtual void LateUpdate() override;

private:
	bool isStart = false;


	shared_ptr<GameObject> blade_upper;
	shared_ptr<GameObject> blade_middle;
	shared_ptr<GameObject> blade_bottom;

	float crusherScale = 0.0f;

	// 블레이드 자체 회전 속도 (라디안/초)
	float bladeRotationSpeed = 1.f; // 필요에 따라 조정

	int my_NUM = -1;
};

