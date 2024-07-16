//-------------------------------------------
//현재 진행중인 게임의 상태를 저장하는 클래스
//차후에 게임을 여러 인스턴스로 만들때 사용
//-------------------------------------------

#pragma once
#include "stdafx.h"

class SESSION;
class OBJECT;

class GAME
{
private:
	//int game_state;						//현재 게임의 진행 상태 (대기, 진행, 종료 등)
	atomic_int g_object_ID = MAX_USER + 1;  //인게임 오브젝트 id

	int team_num;

	int game_id;

	bool spawn_able[40];
	bool room_spawn_able[25];

	int rabbitfoot_owner_id;

private :
	int GetNewObjectID();

public:
	GAME(int id);
	bool IsTerminalOn();
	bool operator==(const GAME&other) const; //같은 인스턴스에 존재하는 플레이어인지 확인하기 위해 사용될 수 있음
	std::shared_ptr<OBJECT>& CreateObjectApprox(int obj_type);
	std::shared_ptr<OBJECT>& CreateObjectApprox_nr(int obj_type, int approx_pos);
	std::shared_ptr<OBJECT>& CreateObject(int obj_type, float x, float y, float z, float dir_x, float dir_y, float dir_z);

	int get_team_num();

	void set_free_space(int obj_type, int spawn_num);
	int select_pos();
	int select_room_pos();

	int get_game_id();

	void set_rabbitfoot_owner(int id);
	int get_rabbitfoot_owner();
public:

	unordered_map<int, std::shared_ptr<SESSION>> ingame_player;    //게임에 포함된 플레이어를 저장하는 컨테이너
	unordered_map<int, std::shared_ptr<OBJECT>> ingame_object;
};

