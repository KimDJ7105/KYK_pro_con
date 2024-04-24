#pragma once
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255

#define BOARD_WIDTH   52
#define BOARD_HEIGHT  30

#define MAX_USER 10000

#define MY_SERVER_IP "127.0.0.1"
#define MY_SERVER_PORT  "4000"
#define MY_PORT 4000

#define CS_POS_INFO 1
#define CS_BOX_CREATE 2
#define CS_MOUSE_INFO 3
#define CS_PICKING_INFO 4

#define SC_POS           1 //오브젝트 이동 & 회전
#define SC_PUT_PLAYER    2 //오브젝트 생성
#define SC_REMOVE_PLAYER 3 //오브젝트 제거
#define SC_LOGIN_INFO	 4 //로그인 정보
#define SC_SET_HP        5 //플레이어의 HP를 설정

#define SC_CREATE_BOX 10 //test용


#pragma pack (push, 1)

//---------Client to Server-----------------

struct cs_packet_pos_info {
	BYTE size;
	BYTE type;
	float x;
	float y;
	float z;
};

struct cs_packet_box_create {
	BYTE size;
	BYTE type;
	float x;
	float y;
	float z;
};


struct cs_packet_logout {
	BYTE size;
	BYTE type;
};

struct cs_packet_mouse_info {
	BYTE size;
	BYTE type;
	float x;
	float y;
	float z;
};

struct cs_packet_picking_info {
	BYTE size;
	BYTE type;
	int shooter_id;
	int target_id;
};

//---------Server To Client-----------------

struct sc_packet_pos {
	BYTE size;
	BYTE type;
	WORD id;
	float x;
	float y;
	float z;
	float dirx;
	float diry;
	float dirz;
};

struct sc_packet_put {
	BYTE size;
	BYTE type;
	WORD id;
	float x;
	float y;
	float z;
	float dirx;
	float diry;
	float dirz;
};

struct sc_packet_login_info {
	BYTE size;
	BYTE type;
	WORD id;
	float x;
	float y;
	float z;
	float dirx;
	float diry;
	float dirz;
};

struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_create_box {
	BYTE size;
	BYTE type;
	WORD id;
	float x;
	float y;
	float z;
};

struct sc_packet_set_hp {
	BYTE size;
	BYTE type;
	WORD id;
	int hp;
};

#pragma pack (pop)