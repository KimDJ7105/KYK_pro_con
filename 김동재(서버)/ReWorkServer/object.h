#pragma once
#include "stdafx.h"

class OBJECT
{
public :
	int obj_id;
	int obj_type;

	OBJECT(int type);

private:
	int approx_pos[2]{ -1,-1 };

	std::array<float, 3> pos;
	std::array<float, 3> rot;
};

