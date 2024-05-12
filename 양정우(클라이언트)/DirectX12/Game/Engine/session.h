#pragma once
#include "pch.h"
class Scene;
class SceneManager;

//================================================================================
//서버 통신을 위한 SESSION Class
class SESSION {
private:
	tcp::socket sock;
	int curr_packet_size_;
	int prev_data_size_;
	unsigned char data_[1024];
	unsigned char packet_[1024];
	bool moving;

	SceneManager* _activeSessionScene{ nullptr };

	bool isMapOpen;

public:

	SESSION(tcp::socket socket_);

	void Process_Packet(unsigned char* packet);

	void do_connect(tcp::resolver::results_type endpoint);

	void do_write(unsigned char* packet, std::size_t length);

	void do_read();

	void Send_Packet(void* packet);

	void set_moving(bool mv);

	bool get_moving();

};

void MoveSession(tcp::socket& sock);

extern SESSION* session;

SESSION* GetSession();


