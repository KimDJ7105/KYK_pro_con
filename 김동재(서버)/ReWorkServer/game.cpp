#include "game.h"
#include "session.h"

GAME::GAME(int id)
{
	game_id = id;

	std::cout << game_id << "�� ���� " << "ī��Ű, �͹̳� ���� �Ϸ�\n";

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
