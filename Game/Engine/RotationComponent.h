#pragma once

#include "MonoBehaviour.h"

class RotationComponent : public MonoBehaviour
{
public:
	RotationComponent();
	virtual ~RotationComponent();

	virtual void LateUpdate() override;
};

