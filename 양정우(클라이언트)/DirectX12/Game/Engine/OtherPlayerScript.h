#pragma once

#include "MonoBehaviour.h"

class OtherPlayerScript : public MonoBehaviour
{
public:
	OtherPlayerScript();
	virtual ~OtherPlayerScript();

	virtual void LateUpdate() override;

	void GetPlayerGuns();

private:
	shared_ptr<GameObject> playerSubGunObject;
	shared_ptr<GameObject> playerMainGunObject;

	shared_ptr<GameObject> nowGunObject;

	bool isSet = false;

	double deadSequence_Start = 0;

	bool isRevive = false;

	int myGunType = -1;
};

