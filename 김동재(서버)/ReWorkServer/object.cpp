#include "object.h"
atomic_int g_object_ID = MAX_USER + 1;
OBJECT::OBJECT(int type)
{
	obj_id = g_object_ID;
	obj_type = type;

	approx_pos[0] = -1; approx_pos[1] = -1;

	g_object_ID++;
}
