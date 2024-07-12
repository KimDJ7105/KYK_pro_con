#include "game.h"
#include "session.h"

static std::random_device rd;
static std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(0, 39);
std::uniform_int_distribution<int> dist2(0, 24);

GAME::GAME(int id)
{
	game_id = id;
	team_num = 0;
	std::cout << game_id << "번 게임 " << "카드키, 터미널 생성 완료\n";

	std::fill(std::begin(spawn_able), std::end(spawn_able), false);
	std::fill(std::begin(room_spawn_able), std::end(room_spawn_able), false);

	for (int i = 0; i < 3; i++) {
		int o_id = GetNewObjectID(); //랜덤 위치 키카드 생성
		ingame_object[o_id] = std::make_shared<OBJECT>(o_id, OT_KEYCARD);
		ingame_object[o_id]->set_pos(select_pos());
		ingame_object[o_id]->show_approx_pos();

		int t_id = GetNewObjectID(); //랜덤 위치 터미널 생성
		ingame_object[t_id] = std::make_shared<OBJECT>(t_id, OT_TERMINAL);
		ingame_object[t_id]->set_pos(select_pos());
		ingame_object[t_id]->show_approx_pos();
	}

	//부활패드 생성
	CreateObjectApprox_nr(OT_RESURRECTION_PAD, 2);
	CreateObjectApprox_nr(OT_RESURRECTION_PAD, 10);
	CreateObjectApprox_nr(OT_RESURRECTION_PAD, 14);
	CreateObjectApprox_nr(OT_RESURRECTION_PAD, 22);

	//분쇄기 생성
	CreateObject(OT_GRINDER, 0, 0, 0, 0, 0, 0);
	CreateObject(OT_GRINDER, 0, 0, 0, 0, 0, 0);
	CreateObject(OT_GRINDER, 0, 0, 0, 0, 0, 0);
	CreateObject(OT_GRINDER, 0, 0, 0, 0, 0, 0);
}

int GAME::GetNewObjectID()
{
	return g_object_ID++;
}

bool GAME::IsTerminalOn()
{
	for (const auto& pair : ingame_object) {
		if (pair.second->obj_type == OT_TERMINAL && pair.second->owner_id == -1) {
			return false;
		}
	}

	return true;
}

bool GAME::operator==(const GAME& other) const {
	return game_id == other.game_id;
}

std::shared_ptr<OBJECT>& GAME::CreateObjectApprox(int obj_type)
{
	int o_id = GetNewObjectID();
	ingame_object[o_id] = std::make_shared<OBJECT>(o_id, obj_type);
	ingame_object[o_id]->set_pos(select_room_pos());
	ingame_object[o_id]->show_approx_pos();

	return ingame_object[o_id];
}

std::shared_ptr<OBJECT>& GAME::CreateObjectApprox_nr(int obj_type, int approx_pos)
{
	int o_id = GetNewObjectID();
	ingame_object[o_id] = std::make_shared<OBJECT>(o_id, obj_type);
	ingame_object[o_id]->set_pos(approx_pos);
	ingame_object[o_id]->show_approx_pos();

	return ingame_object[o_id];
}

std::shared_ptr<OBJECT>& GAME::CreateObject(int obj_type, float x, float y, float z, float dir_x, float dir_y, float dir_z)
{
	int o_id = GetNewObjectID();
	ingame_object[o_id] = std::make_shared<OBJECT>(o_id, obj_type);
	
	ingame_object[o_id]->pos[0] = x;
	ingame_object[o_id]->pos[1] = y;
	ingame_object[o_id]->pos[2] = z;

	ingame_object[o_id]->rot[0] = dir_x;
	ingame_object[o_id]->rot[1] = dir_y;
	ingame_object[o_id]->rot[2] = dir_z;

	return ingame_object[o_id];
}

int GAME::get_team_num()
{
	int curr_num = team_num;
	team_num = (team_num + 1) % 4;
	
	return curr_num;
}

void GAME::set_free_space(int obj_type, int spawn_num)
{
	if (obj_type == OT_RABBITFOOT) room_spawn_able[spawn_num] = false;
	else spawn_able[spawn_num] = false;
}

int GAME::select_pos()
{
	int spawn_num = -1;
	while (spawn_num == -1) {
		spawn_num = dist(gen);
		if (spawn_able[spawn_num]) { //이미 있는경우
			spawn_num = -1;
		}

		else { //위치 선정 성공
			spawn_able[spawn_num] = true;
		}
	}

	return spawn_num;
}

int GAME::select_room_pos()
{
	int spawn_num = -1;

	while (spawn_num == -1) {
		spawn_num = dist2(gen);
		if (room_spawn_able[spawn_num]) { //이미 있는경우
			spawn_num = -1;
		}

		else { //위치 선정 성공
			room_spawn_able[spawn_num] = true;
		}
	}

	return spawn_num;
}