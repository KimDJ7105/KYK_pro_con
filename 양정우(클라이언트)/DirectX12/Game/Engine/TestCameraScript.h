#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

	void MoveUpdate();

	void RotationUpdate();

	void RotatingPickedObject();

private:
	float		_speed = 300.f;
	float		_objspeed = 5.f;

	int type = 0;
	int id = 0;

	bool isMouseMod = true;
	bool isWindowCapture = true;

	wchar_t previousTitle[256] = { 0 };

	shared_ptr<GameObject> pickedMovingObject;

	bool isOverlap = false;

	// 중력 상수 정의
	const float GRAVITY = 9.8f; // 예시로 지구 중력을 사용합니다.

	// 플레이어의 수직 속도
	float verticalVelocity = 0.0f;

	Vec3 previousPosition;

	shared_ptr<GameObject> playerObject;
	shared_ptr<GameObject> playerGunObject;

	double timeElapse = 0.0;
	const double clickCooldown = 0.5;


	int haveKeycard = 0;
};

