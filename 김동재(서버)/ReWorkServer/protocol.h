#pragma once
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255

#define MAX_USER 12
#define MAX_GAME 6

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
constexpr int OT_PLAYER = 0;
constexpr int OT_KEYCARD = 1;
constexpr int OT_ROOM = 2;
constexpr int OT_CORRIDOR = 3;
constexpr int OT_TERMINAL = 4;
constexpr int OT_RESURRECTION_PAD = 5;
constexpr int OT_GRINDER = 6;
constexpr int OT_RABBITFOOT = 7;
constexpr int OT_EXIT = 8;
constexpr int OT_LASER = 9;
constexpr int OT_AMMOBOX = 10;
constexpr int OT_MEDIKIT = 11;

constexpr int OT_WALLAABB = 99;
constexpr int OT_UI_MAIN_WEAPON_3D = 100;
constexpr int OT_UI_PLAYERHAND = 101;
constexpr int OT_UI_SUB_WEAPON_3D = 102;
constexpr int OT_UI_MAP = 103;
constexpr int OT_UI_HP = 104;
constexpr int OT_UI_KEYCARD = 105;
constexpr int OT_UI_GUN = 106;
constexpr int OT_UI_SLASH = 107;
constexpr int OT_UI_MAXBULLET = 108;
constexpr int OT_UI_CURRENTBULLET = 109;
constexpr int OT_UI_RABBITFOOT = 110;
constexpr int OT_UI_MAPOBJECTS = 111;
constexpr int OT_PLAYER_CAMERA = 112;
constexpr int OT_CRUSHER = 113;
constexpr int OT_CRUSHER_BLADE = 114;
constexpr int OT_UI_MAP_BUTTON = 115;
constexpr int OT_CURSOR = 116;
constexpr int OT_HEADCORE = 117;
constexpr int OT_UI_START_BTN = 118;
constexpr int OT_UI_WEAPON_BTN = 119;
constexpr int OT_UI_EXIT_BTN = 120;
constexpr int OT_UI_TITLE = 121;
constexpr int OT_UI_WEAPON_CHANGE = 122;
constexpr int OT_UI_WEAPON_SELECT = 123;
constexpr int OT_OTHER_PLAYER_SUB = 124;
constexpr int OT_OTHER_PLAYER_MAIN = 125;
constexpr int OT_PARTICLE_GUNSHOT = 126;
constexpr int OT_LIGHT_GUNSHOT = 127;

constexpr int OT_UI_LOADING = 128;
constexpr int OT_UI_WATING = 129;
constexpr int OT_UI_STARTING = 130;
constexpr int OT_UI_INTERACT = 131;
constexpr int OT_UI_REVIVE = 132;
constexpr int OT_UI_ESCAPE = 133;

constexpr int OT_UI_FIRSTWAVE = 134;
constexpr int OT_UI_SECONDWAVE = 135;
constexpr int OT_UI_SHOW_ESCAPE = 136;
constexpr int OT_UI_DAMAGED_RED = 137;
//---------------------------------------

//Gun Type-------------------------------
constexpr int GT_SM = 0;
constexpr int GT_SG = 1;
constexpr int GT_SR = 2;
constexpr int GT_AR = 3;
constexpr int GT_PT = 4;
//---------------------------------------


//Animation Type-------------------------
constexpr int AT_IDLE = 0;
constexpr int AT_WALKING = 1;
constexpr int AT_RUNNING = 2;

constexpr int AT_SHOOTING = 3;
constexpr int AT_RELOADING = 4;
constexpr int AT_CHANGE = 5;
//---------------------------------------

//Packet type----------------------------
constexpr BYTE CS_POS_INFO = 1;
constexpr BYTE CS_BOX_CREATE = 2;
constexpr BYTE CS_MOUSE_INFO = 3;
constexpr BYTE CS_PICKING_INFO = 4;
constexpr BYTE CS_TRY_GET_KEY = 5;
constexpr BYTE CS_TRY_USE_TMN = 6;
constexpr BYTE CS_RELOAD_MAG = 7;
constexpr BYTE CS_MOVE_KEY_DOWN = 8;
constexpr BYTE CS_MOVE_KEY_UP = 9;
constexpr BYTE CS_TRY_GET_RABBITFOOT = 10;
constexpr BYTE CS_SET_LASER_TRAP = 11;
constexpr BYTE CS_TRY_ESCAPE = 12;
constexpr BYTE CS_HIT_BY_GRINDER = 13;
constexpr BYTE CS_TRIGGER_LASER = 14;
constexpr BYTE CS_HIT_BY_LASER = 15;
constexpr BYTE CS_USE_RESURRECTION = 16;
constexpr BYTE CS_SEND_GUNTYPE = 17;
constexpr BYTE CS_CHANGE_GUN = 18;
constexpr BYTE CS_RUN_KEY_DOWN = 19;
constexpr BYTE CS_RUN_KEY_UP = 20;
constexpr BYTE CS_USE_MEDIKIT = 21;
constexpr BYTE CS_RECVED_RTT_TEST = 22;

constexpr BYTE SC_POS = 1; //오브젝트 이동 & 회전
constexpr BYTE SC_PUT_PLAYER = 2; //오브젝트 생성
constexpr BYTE SC_REMOVE_PLAYER = 3; //오브젝트 제거
constexpr BYTE SC_LOGIN_INFO = 4; //로그인 정보
constexpr BYTE SC_APPLY_DAMAGE = 5; //플레이어의 HP를 감소시킴
constexpr BYTE SC_PLAYER_DEAD = 6; //플레이어의 HP가 0이 될때
constexpr BYTE SC_PUT_OBJECT = 7; //오브젝트를 생성
constexpr BYTE SC_MODIFY_BULLET = 8; //총알 개수를 변경
constexpr BYTE SC_SHOW_MAP = 9; //단말기를 통해 맵을 출력
constexpr BYTE SC_SET_ANIMATION = 10; //객체의 애니메이션을 세팅
constexpr BYTE SC_CARD_USED = 11; //카드가 사용되었음을 알림
constexpr BYTE SC_PUT_OBJECT_POS = 12; //오브젝트를 방에 생성
constexpr BYTE SC_SHOW_OBJECT_LOC = 13; //터미널 사용시 오브젝트 위치를 화면에 표시
constexpr BYTE SC_PUT_OBJECT_COOR = 14; //오브젝트를 특정 좌표에 생성
constexpr BYTE SC_PLAYER_WIN = 15; //플레이어 팀이 토끼발 들고 탈출
constexpr BYTE SC_PLAYER_LOSE = 16; //플레이어 패배
constexpr BYTE SC_RESURRECTION = 17; //플레이어 코어 상태에서 부활
constexpr BYTE SC_SET_PLAYER_GUN = 18; //플레이어 총 설정
constexpr BYTE SC_GAME_START = 19; //게임 시작을 전달
constexpr BYTE SC_CHANGE_PHASE = 20; //게임 단계 변환
constexpr BYTE SC_SEND_RTT_TEST = 21; //지연시간 확인

constexpr BYTE SL_SET_PORT = 101; //로비서버가 관리하는 서버의 포트번호를 변경
constexpr BYTE SL_SET_IP = 102; //로비서버가 관리하는 서버의 ip를 변경

constexpr BYTE LS_PLAYER_INFO = 201; //서버에 플레이어의 정보(id, 장비, 팀 등)을 전달

constexpr BYTE LC_SET_SERVER_INFO = 255; //로비가 클라이언트에 메인 서버의 ip, port 전송

constexpr BYTE CL_START_GAME = 150; //로비 씬에서 메인 씬으로 전환

#define TEST_SPAWN_RBF 80
//---------------------------------------

#pragma pack (push, 1)

//---------Client to Server-----------------

struct cs_packet_pos_info {
	BYTE size;
	BYTE type;
	float x;
	float y;
	float z;
	long move_time;
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

struct cs_packet_run_key_down {
	BYTE size;
	BYTE type;
};

struct cs_packet_run_key_up {
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
	float x;
	float y;
	float z;
};

struct cs_packet_hit_by_laser {
	BYTE size;
	BYTE type;
};

struct cs_packet_use_resurrection {
	BYTE size;
	BYTE type;
};

struct cs_packet_send_guntype {
	BYTE size;
	BYTE type;
	int gun_type;
};

struct cs_packet_change_gun {
	BYTE size;
	BYTE type;
	int pressed_key;
};

struct cs_packet_use_medikit {
	BYTE size;
	BYTE type;
	int kit_id;
};

struct cs_packet_recved_rtt_test {
	BYTE size;
	BYTE type;
	long recved_t;
	long send_t;
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
	int gun_type;
	int team;
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

struct sc_packet_resurrection {
	BYTE size;
	BYTE type;
	int id;
	int team;
};

struct sc_packet_set_player_gun {
	BYTE size;
	BYTE type;
	int id;
	int gun_type;
};

struct sc_packet_game_start {
	BYTE size;
	BYTE type;
};

struct sc_packet_change_phase {
	BYTE size;
	BYTE type;
};

struct sc_packet_send_rtt_test {
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