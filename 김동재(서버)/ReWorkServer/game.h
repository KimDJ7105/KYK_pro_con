//-------------------------------------------
//���� �������� ������ ���¸� �����ϴ� Ŭ����
//���Ŀ� ������ ���� �ν��Ͻ��� ���鶧 ���
//-------------------------------------------

#pragma once
#include "stdafx.h"

class SESSION;
class OBJECT;

class GAME
{
private:
	//int game_state;						//���� ������ ���� ���� (���, ����, ���� ��)
	atomic_int g_object_ID = MAX_USER + 1;  //�ΰ��� ������Ʈ id

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
	bool operator==(const GAME&other) const; //���� �ν��Ͻ��� �����ϴ� �÷��̾����� Ȯ���ϱ� ���� ���� �� ����
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

	unordered_map<int, std::shared_ptr<SESSION>> ingame_player;    //���ӿ� ���Ե� �÷��̾ �����ϴ� �����̳�
	unordered_map<int, std::shared_ptr<OBJECT>> ingame_object;
};

