#pragma once
#include "stdafx.h"

class OBJECT
{
public :
	int obj_id;
	int obj_type;
	int owner_id;

	int spawn_num;

	void set_pos(int approx_pos);
	void select_pos();
	void select_room_pos();
	OBJECT(int id, int type);

	void show_approx_pos();

private:

	std::array<float, 3> pos;
	std::array<float, 3> rot;
};

