#pragma once
#include "stdafx.h"

enum{WAY_UP, WAY_DOWN, WAY_LEFT, WAY_RIGHT};
class OBJECT
{
public :
	int obj_id;
	int obj_type;
	int owner_id;

	int spawn_num;

	int way;

	std::array<float, 3> pos;
	std::array<float, 3> rot;

public:
	void set_pos(int approx_pos);
	OBJECT(int id, int type);

	void show_approx_pos();

private:
};

