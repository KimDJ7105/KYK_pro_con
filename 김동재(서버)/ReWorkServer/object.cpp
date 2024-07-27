#include "object.h"

OBJECT::OBJECT(int id, int type)
{
	obj_id = id;
	obj_type = type;

	owner_id = -1;
	way = -1;
	spawn_num = -1;

	triggered_laser = 0;

	for (int i = 0; i < 3; i++) {
		pos[i] = 0.f;
		rot[i] = 0.f;
	}
}

void OBJECT::show_approx_pos()
{
	std::cout << "������Ʈ" << obj_id << " ��ġ : " << spawn_num << std::endl;
}

void OBJECT::set_pos(int approx_pos)
{
	spawn_num = approx_pos;
}