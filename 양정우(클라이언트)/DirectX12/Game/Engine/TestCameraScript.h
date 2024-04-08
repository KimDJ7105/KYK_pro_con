#pragma once
#include "MonoBehaviour.h"

class TestCameraScript : public MonoBehaviour
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

	void KeyboardUpdate();

	void MouseUpdate();

private:
	float		_speed = 300.f;

	std::shared_ptr<GameObject> _player;
	int type = 0;
	int id = 0;

	//POINT _tempPos = { 0, 0 };
};

