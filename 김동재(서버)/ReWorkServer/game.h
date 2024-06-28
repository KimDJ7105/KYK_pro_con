//-------------------------------------------
//���� �������� ������ ���¸� �����ϴ� Ŭ����
//���Ŀ� ������ ���� �ν��Ͻ��� ���鶧 ���
//-------------------------------------------

#pragma once
#include "stdafx.h"
#include "session.h"

class GAME
{
private:
	int game_state;						//���� ������ ���� ���� (���, ����, ���� ��)
	int game_id;					    //���� �ν��Ͻ��� id
	unordered_map<int, std::unique_ptr<SESSION>> ingame_player;    //���ӿ� ���Ե� �÷��̾ �����ϴ� �����̳�
	unordered_map<int, std::unique_ptr<OBJECT>> ingame_object;

public:
	bool operator==(const GAME&other) const; //���� �ν��Ͻ��� �����ϴ� �÷��̾����� Ȯ���ϱ� ���� ���� �� ����
};

