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
	bool isWindowCapture = true;

	wchar_t previousTitle[256] = { 0 };

	bool isOverlap = false;

	// �߷� ��� ����
	const float GRAVITY = 9.8f; // ���÷� ���� �߷��� ����մϴ�.

	// �÷��̾��� ���� �ӵ�
	float verticalVelocity = 0.0f;

	Vec3 previousPosition;

	shared_ptr<GameObject> playerObject;
	shared_ptr<GameObject> playerSubGunObject;
	shared_ptr<GameObject> playerMainGunObject;
	shared_ptr<GameObject> playerHeadCoreObject;

	shared_ptr<GameObject> nowGunObject;

	double timeElapse = 0.0;
	const double clickCooldown = 0.067;


	int haveKeycard = 0;


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
};