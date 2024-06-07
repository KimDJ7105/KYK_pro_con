#pragma once
#include "stdafx.h"

//================================================================================
//���� ����� ���� SESSION Class
class LS_SESSION {
private:
	tcp::socket sock;
	int curr_packet_size_;
	int prev_data_size_;
	unsigned char data_[1024];
	unsigned char packet_[1024];

public:

	LS_SESSION(tcp::socket socket_);

	void Process_Packet(unsigned char* packet);

	void do_connect(tcp::resolver::results_type endpoint);

	void do_write(unsigned char* packet, std::size_t length);

	void do_read();

	void Send_Packet(void* packet);

};