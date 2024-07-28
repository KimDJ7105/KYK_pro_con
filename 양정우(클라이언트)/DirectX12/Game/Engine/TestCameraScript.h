#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

	void SetObjects();

	void RotationUpdate();

private:
	float		_speed = 300.f;
	float		_objspeed = 5.f;

	int type = 0;
	int id = 0;

	bool isMouseMod = true;

	wchar_t previousTitle[256] = { 0 };

	bool isOverlap = false;

	// 중력 상수 정의
	const float GRAVITY = 9.8f; // 예시로 지구 중력을 사용합니다.

	// 플레이어의 수직 속도
	float verticalVelocity = 0.0f;

	Vec3 previousPosition;

	shared_ptr<GameObject> playerObject;
	shared_ptr<GameObject> playerSubGunObject;
	shared_ptr<GameObject> playerMainGunObject;
	shared_ptr<GameObject> playerHeadCoreObject;

	shared_ptr<GameObject> nowGunObject;

	double fireTimeElapse = 0.0;
	double clickCooldown = 0.333;

	double weaponTimeElapse = 0.0;

	float weaponChangetime = -1;
	bool weaponChanging = false;

	int haveKeycard = 0;

	int nowGun = 0;


	bool wKeyState = false;
	bool aKeyState = false;
	bool sKeyState = false;
	bool dKeyState = false;


	bool isMoving = false;

	bool isCrushed = false;

	float moveSpeed = 200.f;

	bool isDash = false;

private:
	shared_ptr<GameObject> cursor;

private:
	shared_ptr<GameObject> flameParticle;
	double flameDuration = 0.080;
	double flameTimeElapse = 1;

	shared_ptr<GameObject> flameLight;

public:
	bool is_moveable(const Vec3& moveDirection, const shared_ptr<GameObject>& overlap);

};