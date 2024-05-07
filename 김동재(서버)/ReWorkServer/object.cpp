#include "object.h"

static std::random_device rd;
static std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(0, 24);
std::uniform_int_distribution<int> dist2(0, 3);

atomic_int g_object_ID = MAX_USER + 1;

OBJECT::OBJECT(int type)
{
	obj_id = g_object_ID;
	obj_type = type;

	approx_pos[0] = -1; approx_pos[1] = -1;

	g_object_ID++;
}

void OBJECT::show_approx_pos()
{
	std::cout << "오브젝트" << obj_id << " 위치 : " << approx_pos[0] << " " << approx_pos[1] << std::endl;
}

void OBJECT::select_pos()
{
	bool check[4]{ 0, };
	approx_pos[0] = dist(gen);
	approx_pos[1] = -1;
	if ((approx_pos[0] + 5) < 24) check[0] = true;
	if ((approx_pos[0] - 5) > 0) check[1] = true;
	if ((approx_pos[0] + 1) % 5 != 0) check[2] = true;
	if ((approx_pos[0] - 1) % 5 != 4) check[3] = true;

	while (approx_pos[1] == -1) {
		switch (dist2(gen)) {
		case 0:
			if (check[0]) approx_pos[1] = approx_pos[0] + 5;
			break;
		case 1:
			if (check[1]) approx_pos[1] = approx_pos[0] - 5;
			break;
		case 2:
			if (check[2]) approx_pos[1] = approx_pos[0] + 1;
			break;
		case 3:
			if (check[3]) approx_pos[1] = approx_pos[0] - 1;
			break;
		}
	}
}