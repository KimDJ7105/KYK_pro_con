//-------------------------------------------
//현재 진행중인 게임의 상태를 저장하는 클래스
//차후에 게임을 여러 인스턴스로 만들때 사용
//-------------------------------------------

#pragma once
#include "stdafx.h"
#include "session.h"

class GAME
{
private:
	int game_state;						//현재 게임의 진행 상태 (대기, 진행, 종료 등)
	int game_id;					    //게임 인스턴스의 id
	unordered_map<int, std::unique_ptr<SESSION>> ingame_player;    //게임에 포함된 플레이어를 저장하는 컨테이너
	unordered_map<int, std::unique_ptr<OBJECT>> ingame_object;

public:
	bool operator==(const GAME&other) const; //같은 인스턴스에 존재하는 플레이어인지 확인하기 위해 사용될 수 있음
};

