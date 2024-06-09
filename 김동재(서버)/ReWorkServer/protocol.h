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
#define MY_SERVER_PORT  "55000"
#define MY_PORT 55000

//Lobby Info-----------------------------
#define LOBBY_PORT 50000
#define LOBBY_ID 0
//---------------------------------------

//Object Type----------------------------
#define OT_PLAYER		0
#define OT_KEYCARD		1
#define OT_ROOM			2
#define OT_CORRIDOR		3
#define OT_TERMINAL		4

#define OT_WALLAABB			99
#define OT_UI_PLAYERHAND	101
#define OT_UI_SMG_3D		102
#define OT_UI_MAP			103
#define OT_UI_HP			104
#define OT_UI_KEYCARD		105
#define OT_UI_SMG			106
#define OT_UI_SLASH			107
#define OT_UI_MAXBULLET		108
#define OT_UI_CURRENTBULLET	109
//---------------------------------------

//Animation Type-------------------------
#define AT_IDLE 0
#define AT_WALKING 1
#define AT_RUNNING 2
#define AT_SHOOTING 3
//---------------------------------------

//Packet type----------------------------
#define CS_POS_INFO      1
#define CS_BOX_CREATE    2
#define CS_MOUSE_INFO    3
#define CS_PICKING_INFO  4
#define CS_TRY_GET_KEY   5
#define CS_TRY_USE_TMN   6
#define CS_RELOAD_MAG    7
#define CS_MOVE_KEY_DOWN 8
#define CS_MOVE_KEY_UP   9

#define SC_POS           1 //오브젝트 이동 & 회전
#define SC_PUT_PLAYER    2 //오브젝트 생성
#define SC_REMOVE_PLAYER 3 //오브젝트 제거
#define SC_LOGIN_INFO	 4 //로그인 정보
#define SC_APPLY_DAMAGE  5 //플레이어의 HP를 감소시킴
#define SC_PLAYER_DEAD   6 //플레이어의 HP가 0이 될때
#define SC_PUT_OBJECT    7 //오브젝트를 생성
#define SC_MODIFY_BULLET 8 //총알 개수를 변경
#define SC_SHOW_MAP      9 //단말기를 통해 맵을 출력
#define SC_SET_ANIMATION 10 //객체의 애니메이션을 세팅
#define SC_CARD_USED     11 //카드가 사용되었음을 알림
//---------------------------------------

//Weapon Info----------------------------
#define WP_SMG 0
#define WP_RIFLE 1
#define WP_SHOTGUN 2
#define WP_HANDGUN 3
#define WP_HAMMER 4
//---------------------------------------

#pragma pack (push, 1)

//---------Client to Server-----------------

struct cs_packet_pos_info {
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
	int target_id;
};

struct cs_packet_try_get_key {
	BYTE size;
	BYTE type;
	int key_id;
	float x;
	float y;
	float z;
};

struct cs_packet_try_use_tmn {
	BYTE size;
	BYTE type;
	int terminal_id;
};

struct cs_packet_reload_mag {
	BYTE size;
	BYTE type;
};

struct cs_packet_move_key_down {
	BYTE size;
	BYTE type;
};

struct cs_packet_move_key_up {
	BYTE size;
	BYTE type;
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
	int animation_id;
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
	int bullet_amount;
};

struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
	int obj_type;
};

struct sc_packet_apply_damage {
	BYTE size;
	BYTE type;
	WORD id;
	int hp;
};

struct sc_packet_player_dead {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_put_object {
	BYTE size;
	BYTE type;
	WORD id;
	int obj_type;
	int approx_num;
};

struct sc_packet_modify_bullet {
	BYTE size;
	BYTE type;
	int amount;
};

struct sc_packet_show_map {
	BYTE size;
	BYTE type;
};

struct sc_packet_set_animation {
	BYTE size;
	BYTE type;
	int obj_id;
	int animation_id;
};

struct sc_packet_card_used {
	BYTE size;
	BYTE type;
};

#pragma pack (pop)