#pragma once

#include "MonoBehaviour.h"

class FloatingComponent : public MonoBehaviour
{
public:
	FloatingComponent();
	virtual ~FloatingComponent();

	virtual void LateUpdate() override;


private:
	double ElapsedTime = 0;
};

