#pragma once

#include "MonoBehaviour.h"

class EndingCameraScript : public MonoBehaviour
{
public:
	EndingCameraScript();
	virtual ~EndingCameraScript();

	virtual void LateUpdate() override;
};

