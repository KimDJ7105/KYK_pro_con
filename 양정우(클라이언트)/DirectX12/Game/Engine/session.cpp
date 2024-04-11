#pragma once
#include "pch.h"
#include "session.h"
#include "SceneManager.cpp"


int type = 5;

SESSION::SESSION(tcp::socket socket_) : sock(std::move(socket_))
{
	curr_packet_size_ = 0;
	prev_data_size_ = 0;
	_activeSessionScene = GET_SINGLE(SceneManager);
}

void SESSION::Process_Packet(unsigned char* packet)
{
	int test = 0;
	switch (packet[1]) {
	case SC_LOGIN_INFO: //서버에서 시작 정보를 받아서 자신의 캐릭터를 생성
	{
		sc_packet_login_info* p = reinterpret_cast<sc_packet_login_info*>(packet);
		scene->AddGameObject(_activeSessionScene->CreateObject(type, p->id, p->x, p->y, p->z, 0, 0.0f));

		//TODO : Direction을 적용하기 위한 추가 인자 요청
		// CreateObject함수의 인자도 이에 맞게 변경 필요
		//scene->AddGameObject(_activeSessionScene->CreateObject(type, p->id, p->x, p->y, p->z, 0, directionX, directionY, directionZ));
		break;
	}
	case SC_PUT_PLAYER: //다른 플레이어의 정보를 받아 캐릭터 생성
	{
		sc_packet_put* p = reinterpret_cast<sc_packet_put*>(packet);
		scene->AddGameObject(_activeSessionScene->CreateObject(type, p->id, p->x, p->y, p->z, 0, 0.0f));

		//TODO : Direction을 적용하기 위한 추가 인자 요청
		// CreateObject함수의 인자도 이에 맞게 변경 필요
		//scene->AddGameObject(_activeSessionScene->CreateObject(type, p->id, p->x, p->y, p->z, 0, directionX, directionY, directionZ));
		break;
	}
	case SC_POS: //생성되어있는 오브젝트, 다른 캐릭터를 이동
	{
		sc_packet_pos* p = reinterpret_cast<sc_packet_pos*>(packet);
		//int object_id = p->id;
		//float x = p->x;
		//float y = p->y;
		//float z = p->z;		// sock log info에 z에 해당하는 것이 없어 0으로 하였음
		//float direction = 0;
		//_activeScene->ChangeObjectLocation(p->id, p->x, p->y, p->z, 0);
		_activeSessionScene->ChangeObjectLocation(p->id, p->x, p->y, p->z, 0);

		//TODO : Direction을 적용하기 위한 추가 인자 요청
		// ChangeObjectLocation함수의 인자도 이에 맞게 변경 필요
		//_activeSessionScene->ChangeObjectLocation(p->id, p->x, p->y, p->z, directionX, directionY, directionZ);

		break;
	}
	case SC_ROTATE_VIEW : //id로 지정된 플레이어를 회전시킴. (시야에 맞춰서)
	{
		sc_packet_rotate_player* p = reinterpret_cast<sc_packet_rotate_player*>(packet);
		break;
	}
	case SC_CREATE_BOX : //테스트용 박스 생성
	{
		sc_packet_create_box* p = reinterpret_cast<sc_packet_create_box*>(packet);

		//TODO : Direction을 적용하기 위한 추가 인자 요청
		// CreateObject함수의 인자도 이에 맞게 변경 필요
		scene->AddGameObject(_activeSessionScene->CreateObject(type, p->id, p->x, p->y, p->z, 0, 0.0f));
		break;
	}
	default: // 지정되지 않은 패킷을 수신받았을 때
		return;
		break;
	}
}

void SESSION::do_connect(tcp::resolver::results_type endpoint)
{
	async_connect(sock, endpoint,
		[this](const boost::system::error_code& ec, const tcp::endpoint& endpoint)
		{
			if (!ec) {
				do_read();
			}

			else return -1;
		}
	);
}

void SESSION::do_write(unsigned char* packet, std::size_t length)
{
	sock.async_write_some(boost::asio::buffer(packet, length),
		[packet, length](boost::system::error_code ec, std::size_t bytes_transferred)
		{
			if (!ec)
			{
				if (length != bytes_transferred) {
					return -1;
				}
				delete packet;
			}
		});
}

void SESSION::do_read()
{
	sock.async_read_some(boost::asio::buffer(data_),
		[this](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				if (ec.value() == boost::asio::error::operation_aborted) return;

				return;
			}

			int data_to_process = static_cast<int>(length);
			unsigned char* buf = data_;
			while (0 < data_to_process) {
				if (0 == curr_packet_size_) {
					curr_packet_size_ = buf[0];
					if (buf[0] > 200) {
						exit(-1);
					}
				}
				int need_to_build = curr_packet_size_ - prev_data_size_;
				if (need_to_build <= data_to_process) {
					memcpy(packet_ + prev_data_size_, buf, need_to_build);
					Process_Packet(packet_);
					curr_packet_size_ = 0;
					prev_data_size_ = 0;
					data_to_process -= need_to_build;
					buf += need_to_build;
				}
				else {
					memcpy(packet_ + prev_data_size_, buf, data_to_process);
					prev_data_size_ += data_to_process;
					data_to_process = 0;
					buf += data_to_process;
				}
			}
			do_read();
		});
}

void SESSION::Send_Packet(void* packet)
{
	int packet_size = reinterpret_cast<unsigned char*>(packet)[0];
	unsigned char* buff = new unsigned char[packet_size];
	memcpy(buff, packet, packet_size);
	do_write(buff, packet_size);
}

SESSION* session;

void MoveSession(tcp::socket& sock)
{
	session = new SESSION(std::move(sock));
}

SESSION* GetSession() { return session; }