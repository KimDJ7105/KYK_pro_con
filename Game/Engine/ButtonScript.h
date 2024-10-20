#pragma once

#include "MonoBehaviour.h"


class ButtonScript : public MonoBehaviour
{
public:
	ButtonScript();
	virtual ~ButtonScript();

	virtual void LateUpdate() override;

	Vec2 WorldToScreen(Vec3 worldPos);
};

