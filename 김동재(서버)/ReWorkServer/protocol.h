#pragma once
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255

#define BOARD_WIDTH   52
#define BOARD_HEIGHT  30

#define MAX_USER 12

#define MY_SERVER_IP "127.0.0.1"
#define MY_SERVER_PORT  "55000"
#define MY_PORT0 55000
#define MY_PORT1 55001
#define MY_PORT2 55002
#define MY_PORT3 55003

//Lobby Info-----------------------------
#define LOBBY_IP "127.0.0.1"
#define MY_LOBBY_PORT "50000"
#define LOBBY_PORT 50000
#define LOBBY_ID 0
//---------------------------------------

//Object Type----------------------------
#define OT_PLAYER			0
#define OT_KEYCARD			1
#define OT_ROOM				2
#define OT_CORRIDOR			3
#define OT_TERMINAL			4
#define OT_RESURRECTION_PAD 5
#define OT_GRINDER          6
#define OT_RABBITFOOT       7
#define OT_EXIT				8

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
#define OT_UI_RABBITFOOT	110
#define OT_UI_MAPOBJECTS	111
#define OT_PLAYER_CAMERA	112
#define OT_CRUSHER			113
#define OT_CRUSHER_BLADE	114
#define OT_UI_MAP_BUTTON	115
//---------------------------------------

//Animation Type-------------------------
#define AT_IDLE 0
#define AT_WALKING 1
#define AT_RUNNING 2
#define AT_SHOOTING 3
//---------------------------------------

//Packet type----------------------------
#define CS_POS_INFO			  1
#define CS_BOX_CREATE         2
#define CS_MOUSE_INFO         3
#define CS_PICKING_INFO       4
#define CS_TRY_GET_KEY        5
#define CS_TRY_USE_TMN        6
#define CS_RELOAD_MAG         7
#define CS_MOVE_KEY_DOWN      8
#define CS_MOVE_KEY_UP        9
#define CS_TRY_GET_RABBITFOOT 10
#define CS_SET_LASER_TRAP     11
#define CS_TRY_ESCAPE		  12
#define CS_HIT_BY_GRINDER	  13
#define CS_TRIGGER_LASER	  14

#define SC_POS             1 //������Ʈ �̵� & ȸ��
#define SC_PUT_PLAYER      2 //������Ʈ ����
#define SC_REMOVE_PLAYER   3 //������Ʈ ����
#define SC_LOGIN_INFO	   4 //�α��� ����
#define SC_APPLY_DAMAGE    5 //�÷��̾��� HP�� ���ҽ�Ŵ
#define SC_PLAYER_DEAD     6 //�÷��̾��� HP�� 0�� �ɶ�
#define SC_PUT_OBJECT      7 //������Ʈ�� ����
#define SC_MODIFY_BULLET   8 //�Ѿ� ������ ����
#define SC_SHOW_MAP        9 //�ܸ��⸦ ���� ���� ���
#define SC_SET_ANIMATION   10 //��ü�� �ִϸ��̼��� ����
#define SC_CARD_USED       11 //ī�尡 ���Ǿ����� �˸�
#define SC_PUT_OBJECT_POS  12 //������Ʈ�� �濡 ����
#define SC_SHOW_OBJECT_LOC 13 //�͹̳� ���� ������Ʈ ��ġ�� ȭ�鿡 ǥ��
#define SC_PUT_OBJECT_COOR 14 //������Ʈ�� Ư�� ��ǥ�� ����
#define SC_PLAYER_WIN	   15 //�÷��̾� ���� �䳢�� ��� Ż��
#define SC_PLAYER_LOSE     16 //�÷��̾� �й�

#define SL_SET_PORT    101 //�κ񼭹��� �����ϴ� ������ ��Ʈ��ȣ�� ����
#define SL_SET_IP      102 //�κ񼭹��� �����ϴ� ������ ip�� ����

#define LS_PLAYER_INFO    201 //������ �÷��̾��� ����(id, ���, �� ��)�� ����

#define LC_SET_SERVER_INFO 255 //�κ� Ŭ���̾�Ʈ�� ���� ������ ip, port ����

#define CL_START_GAME      150 //�κ� ������ ���� ������ ��ȯ

#define TEST_SPAWN_RBF 80
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

struct cs_packet_try_get_rabbitfoot {
	BYTE size;
	BYTE type;
	int obj_id;
	float x;
	float y;
	float z;
};

struct cs_packet_set_laser_trap {
	BYTE size;
	BYTE type;
	int room_num;
};

struct cs_packet_try_escape {
	BYTE size;
	BYTE type;
};

struct cs_packet_hit_by_grinder {
	BYTE size;
	BYTE type;
};

struct cs_packet_trigger_laser {
	BYTE size;
	BYTE type;
	int room_num;
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
	int team_num;
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

struct sc_packet_put_object_pos {
	BYTE size;
	BYTE type;
	WORD id;
	int obj_type;
	int approx_num;
};

struct sc_packet_show_object_loc {
	BYTE size;
	BYTE type;
	int obj_type;
	int loc_type;
	int approx_num;
};

struct sc_packet_put_object_coor {
	BYTE size;
	BYTE type;
	int obj_type;
	int obj_id;
	float x;
	float y;
	float z;
	float dirx;
	float diry;
	float dirz;
};

struct sc_packet_player_win {
	BYTE size;
	BYTE type;
};

struct sc_packet_player_lose {
	BYTE size;
	BYTE type;
};

struct test_packet {
	BYTE size;
	BYTE type;
};
//------------server to lobby packet-------------
struct sl_packet_set_port {
	BYTE size;
	BYTE type;
	char port[6];
};

struct sl_packet_set_ip {
	BYTE size;
	BYTE type;
	char ip[16];
};

//------------lobby to server packet-------------
struct ls_packet_player_info {
	BYTE size;
	BYTE type;
};

//------------lobby to client packet-------------
struct lc_packet_set_server_info{
	BYTE size;
	BYTE type;
	char ip[16];
	char port[6];
};

//------------client to lobby packet-------------
struct cl_packet_start_game {
	BYTE size;
	BYTE type;
};
#pragma pack (pop)