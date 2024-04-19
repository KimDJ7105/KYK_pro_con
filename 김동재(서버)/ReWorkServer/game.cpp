#include "game.h"

bool GAME::operator==(const GAME& other) const {
	return game_id == other.game_id;
}