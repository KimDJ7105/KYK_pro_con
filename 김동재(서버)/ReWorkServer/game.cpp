#include "game.h"
#include "session.h"

GAME::GAME(int id)
{
	game_id = id;
	team_num = 0;
	std::cout << game_id << "번 게임 " << "카드키, 터미널 생성 완료\n";

	for (int i = 0; i < 3; i++) {
		int o_id = GetNewObjectID();
		ingame_object[o_id] = std::make_shared<OBJECT>(o_id, OT_KEYCARD);
		ingame_object[o_id]->select_pos();
		ingame_object[o_id]->show_approx_pos();

		int t_id = GetNewObjectID();
		ingame_object[t_id] = std::make_shared<OBJECT>(t_id, OT_TERMINAL);
		ingame_object[t_id]->select_pos();
		ingame_object[t_id]->show_approx_pos();
	}

	//부활패드 생성
	CreateObjectApprox_nr(OT_RESURRECTION_PAD, 2);
	CreateObjectApprox_nr(OT_RESURRECTION_PAD, 10);
	CreateObjectApprox_nr(OT_RESURRECTION_PAD, 14);
	CreateObjectApprox_nr(OT_RESURRECTION_PAD, 22);
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
	ingame_object[o_id]->select_room_pos();
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

int GAME::get_team_num()
{
	int curr_num = team_num;
	team_num = (team_num + 1) % 4;
	
	return curr_num;
}
