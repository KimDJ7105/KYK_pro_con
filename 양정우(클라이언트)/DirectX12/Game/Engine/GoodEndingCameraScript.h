#pragma once

#include "MonoBehaviour.h"

class GoodEndingCameraScript : public MonoBehaviour
{
public:
	GoodEndingCameraScript();
	virtual ~GoodEndingCameraScript();

	virtual void LateUpdate() override;
};

