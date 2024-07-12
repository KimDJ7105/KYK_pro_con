#include "timer_event.h"

constexpr bool TIMER_EVENT::operator < (const TIMER_EVENT& L) const
{
	return (wakeup_time > L.wakeup_time);
}