#pragma once
#include "stdafx.h"
#include "object.h"

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
	int remain_bullet;
	int team;

	int equip_weapon;

	array<float, 3> pos;
	array<float, 3> view_dir;

private:
	void Send_Packet(void* packet, unsigned id);
	
	void Process_Packet(unsigned char* packet, int id);

	void do_read();

	void do_write(unsigned char* packet, std::size_t length);

	int find_useable_key();

public:
	SESSION(tcp::socket socket, int new_id);

	void start();

	void Send_Packet(void* packet);
};

extern concurrency::concurrent_unordered_map<int, shared_ptr<SESSION>> players;
extern concurrency::concurrent_unordered_map<int, shared_ptr<OBJECT>> objects;