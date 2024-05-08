#pragma once
#include "stdafx.h"

class OBJECT
{
public :
	int obj_id;
	int obj_type;
	int owner_id;

	int approx_pos[2]{ -1,-1 };

	void select_pos();
	OBJECT(int id, int type);

	void show_approx_pos();

private:

	std::array<float, 3> pos;
	std::array<float, 3> rot;
};

