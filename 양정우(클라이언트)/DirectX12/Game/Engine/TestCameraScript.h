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

	Vec3 beforePos;
};

