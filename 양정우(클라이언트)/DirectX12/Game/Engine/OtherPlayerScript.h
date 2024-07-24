#pragma once

#include "MonoBehaviour.h"

class OtherPlayerScript : public MonoBehaviour
{
public:
	OtherPlayerScript();
	virtual ~OtherPlayerScript();

	virtual void LateUpdate() override;
};

