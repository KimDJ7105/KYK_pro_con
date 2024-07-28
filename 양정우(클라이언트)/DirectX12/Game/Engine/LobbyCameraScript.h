#pragma once

#include "MonoBehaviour.h"

class LobbyCameraScript : public MonoBehaviour
{
public:
	LobbyCameraScript();
	virtual ~LobbyCameraScript();

	virtual void LateUpdate() override;

private:
	shared_ptr<GameObject> cursor;

	bool isStart = false;

public:
	void GetWeaponSelectUI();


};

