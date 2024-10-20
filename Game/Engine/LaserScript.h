#pragma once

#include "MonoBehaviour.h"

class LaserScript : public MonoBehaviour
{
public:
	LaserScript();
	virtual ~LaserScript();

	virtual void LateUpdate() override;

private:
	int my_NUM = -1;
	bool isSet = false;
};

