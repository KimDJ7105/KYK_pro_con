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

bool GAME::operator==(const GAME& other) const {
	return game_id == other.game_id;
}