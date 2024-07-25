#pragma once
#include "stdafx.h"

enum EVENT_TYPE { EV_LASER_TRAP_ON, EV_MOVE_LASER_TRAP, EV_MOVE_GRINDER, EV_SPAWN_EXIT, EV_SPAWN_ITEM };
class TIMER_EVENT
{
public :
	int game_id;
	chrono::system_clock::time_point wakeup_time;
	EVENT_TYPE event_id;
	int target_id;

	float x;
	float y;
	float z;

public:
	constexpr bool operator < (const TIMER_EVENT& L) const
	{
		return (wakeup_time > L.wakeup_time);
	}
};

