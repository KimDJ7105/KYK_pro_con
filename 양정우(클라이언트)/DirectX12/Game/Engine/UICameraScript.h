#pragma once

#include "MonoBehaviour.h"

class UICameraScript : public MonoBehaviour
{

private:
	bool cameraMod = false;

public:
	UICameraScript();
	virtual ~UICameraScript();

	virtual void LateUpdate() override;
};

