#pragma once
#include "stdafx.h"
#include "object.h"
#include "timer_event.h"

class GAME;
class SERVER;

struct MOVE_LOG
{
	float x;
	float z;
	long move_time;
};

class SESSION
	: public std::enable_shared_from_this<SESSION>
{
private:
	tcp::socket socket_;
	int my_id_;
	enum { max_length = 1024 };
	unsigned char data_[max_length];
	unsigned char packet_[max_length];
	int curr_packet_size_;
	int prev_data_size_;

	int hp;
	int remain_bullet[2];
	int select_gun;
	int team;

	int equip_weapon;

	array<float, 3> pos;
	array<float, 3> view_dir;

	std::weak_ptr<GAME> my_game;
	SERVER* my_server;

	bool is_core_state;
	bool is_running;
	bool is_moving;

	int gun_type;
	int using_tml_id;

	std::array<MOVE_LOG, 3> m_move_log;
	int log_index;

	bool are_pos_equal(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2) const;
private:
	void Send_Packet(void* packet, unsigned id);
	
	void Process_Packet(unsigned char* packet, int id);

	void do_read();

	void do_write(unsigned char* packet, std::size_t length);

	int find_useable_key();

public:
	SESSION(tcp::socket socket, int new_id, int team_num);

	void start();

	void Send_Packet(void* packet);

	void set_serverinfo(std::shared_ptr<GAME> p, SERVER* server);

	void close_socket();
};

extern shared_ptr<SESSION> lobby; //만일 이게 여러 쓰레드에서 호출되면 락을 걸어야 함.... 현재 상태는 그럼