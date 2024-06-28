#pragma once

#include "MonoBehaviour.h"

class LobbyCameraScript : public MonoBehaviour
{
public:
	LobbyCameraScript();
	virtual ~LobbyCameraScript();

	virtual void LateUpdate() override;
};

