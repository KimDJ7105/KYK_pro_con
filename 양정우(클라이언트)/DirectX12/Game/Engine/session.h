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
	int haveKeycard;

	int guntype;
public:

	SESSION(tcp::socket socket_);

	void Process_Packet(unsigned char* packet);

	void do_connect(tcp::resolver::results_type endpoint);

	void do_write(unsigned char* packet, std::size_t length);

	void do_read();

	void Send_Packet(void* packet);

	void set_moving(bool mv);

	bool get_moving();

	bool get_isMapOpen();

	void close_socket();

	void set_guntype(int gt);

	int get_guntype();
};

void MoveSession(tcp::socket& sock);
void MoveGuntype();

extern SESSION* session;
extern SESSION* main_session;

SESSION* GetSession();

extern io_context main_io_con;
extern std::thread* serverthread_p;

extern char main_server_ip[16];
extern char main_server_port[6];
