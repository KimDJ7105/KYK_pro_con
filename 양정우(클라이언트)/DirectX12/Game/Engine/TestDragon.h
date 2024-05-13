#pragma once
#include "MonoBehaviour.h"

class TestDragon : public MonoBehaviour
{
public:
	virtual void Update() override;

	void Set(int num);
};

