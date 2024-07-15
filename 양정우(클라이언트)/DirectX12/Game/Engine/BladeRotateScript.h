#pragma once

#include "MonoBehaviour.h"


class BladeRotateScript : public MonoBehaviour
{
public:
	BladeRotateScript();
	virtual ~BladeRotateScript();

	virtual void LateUpdate() override;
};

