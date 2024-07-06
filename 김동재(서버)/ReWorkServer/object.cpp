#include "object.h"

static std::random_device rd;
static std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(0, 39);
std::uniform_int_distribution<int> dist2(0, 24);

atomic_bool spawn_able[40]{false, };
atomic_bool room_spawn_able[25]{ false, };

OBJECT::OBJECT(int id, int type)
{
	obj_id = id;
	obj_type = type;

	owner_id = -1;

	spawn_num = -1;

}

void OBJECT::show_approx_pos()
{
	std::cout << "오브젝트" << obj_id << " 위치 : " << spawn_num << std::endl;
}

void OBJECT::select_pos()
{
	while (spawn_num == -1) {
		spawn_num = dist(gen);
		if (spawn_able[spawn_num]) { //이미 있는경우
			spawn_num = -1;
		}

		else { //위치 선정 성공
			spawn_able[spawn_num] = true;
		}
	}
}

void OBJECT::select_room_pos()
{
	while (spawn_num == -1) {
		spawn_num = dist2(gen);
		if (room_spawn_able[spawn_num]) { //이미 있는경우
			spawn_num = -1;
		}

		else { //위치 선정 성공
			room_spawn_able[spawn_num] = true;
		}
	}
}