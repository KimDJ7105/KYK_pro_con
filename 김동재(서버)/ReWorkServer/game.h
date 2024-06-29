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
	int game_id;					    //���� �ν��Ͻ��� id
	atomic_int g_object_ID = MAX_USER + 1;  //�ΰ��� ������Ʈ id

private :
	int GetNewObjectID();

public:
	GAME(int id);

	bool operator==(const GAME&other) const; //���� �ν��Ͻ��� �����ϴ� �÷��̾����� Ȯ���ϱ� ���� ���� �� ����
public:
	unordered_map<int, std::shared_ptr<SESSION>> ingame_player;    //���ӿ� ���Ե� �÷��̾ �����ϴ� �����̳�
	unordered_map<int, std::shared_ptr<OBJECT>> ingame_object;
};

