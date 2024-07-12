#pragma once
#include "stdafx.h"

class OBJECT
{
public :
	int obj_id;
	int obj_type;
	int owner_id;

	int spawn_num;

	std::array<float, 3> pos;
	std::array<float, 3> rot;

public:
	void set_pos(int approx_pos);
	OBJECT(int id, int type);

	void show_approx_pos();

private:
};

