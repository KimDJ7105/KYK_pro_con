#pragma once
#include "pch.h"
#include "session.h"
#include "SceneManager.cpp"

int playerID = -1;

SESSION::SESSION(tcp::socket socket_) : sock(std::move(socket_))
{
	curr_packet_size_ = 0;
	prev_data_size_ = 0;
	moving = false;
	_activeSessionScene = GET_SINGLE(SceneManager);
	isMapOpen = false;
	haveKeycard = 0;
}

void SESSION::Process_Packet(unsigned char* packet)
{
	int test = 0;
	switch (packet[1]) {
	case SC_LOGIN_INFO: //�������� ���� ������ �޾Ƽ� �ڽ��� ĳ���͸� ����
	{
		sc_packet_login_info* p = reinterpret_cast<sc_packet_login_info*>(packet);

		//scene->AddGameObject(_activeSessionScene->CreateBoxObject(type, p->id, p->x, p->y, p->z, 0, p->dirx, p->diry, p->dirz));
		//_activeSessionScene->CreatePlayerObject(OT_PLAYER, p->id, p->x, p->y - 40.f, p->z, 0, p->dirx, p->diry + 3.14f, p->dirz);

		//p->bullet_amount; �÷��̾� �ʱ� �Ѿ� ���� (30��)
		//���� ���� ���� �ʿ�
		playerID = p->id;
		_activeSessionScene->SetPlayerID(p->id);
		_activeSessionScene->CreatePlayerHandObject(101, p->id, p->x, p->y - 80.f, p->z, 0, p->dirx, p->diry + 3.14f, p->dirz);
		_activeSessionScene->CreatePlayerGunObject(102, p->id, 5, 35, 15, 0, p->dirx, p->diry, p->dirz);
		_activeSessionScene->SetBullet(p->bullet_amount);
		break;
	}
	case SC_PUT_PLAYER: //�ٸ� �÷��̾��� ������ �޾� ĳ���� ����
	{
		sc_packet_put* p = reinterpret_cast<sc_packet_put*>(packet);
		//scene->AddGameObject(_activeSessionScene->CreateBoxObject(type, p->id, p->x, p->y, p->z, 0, p->dirx, p->diry, p->dirz));
		_activeSessionScene->CreatePlayerObject(OT_PLAYER, p->id, p->x, p->y - 40.f, p->z, 0, p->dirx, p->diry + 3.14f, p->dirz);
		break;
	}
	case SC_POS: //�����Ǿ��ִ� ������Ʈ, �ٸ� ĳ���͸� �̵� ȸ��
	{
		sc_packet_pos* p = reinterpret_cast<sc_packet_pos*>(packet);
		_activeSessionScene->ChangeObjectMovement(p->id, p->x, p->y - 40.f, p->z, p->dirx, p->diry + 3.14f, p->dirz, p->animation_id);
		//p->animation_id �� �ִϸ��̼� ���� ����

		//���콺�̵��� �Ҷ��� -1���̴�.
		//�������� �����Ǿ������� p->animation_id�� �����ָ� �ȴ�.

		//_activeSessionScene->ChangeObjectAnimation(p->id, p->animation_id);
		break;
	}
	case SC_REMOVE_PLAYER :
	{
		sc_packet_remove_player* p = reinterpret_cast<sc_packet_remove_player*>(packet);
		//p->id �� ���� id�� ���� ��ü ���� (�÷��̾�, ������Ʈ)
		_activeSessionScene->RemoveObject(p->id);
		break;
	}
	case SC_APPLY_DAMAGE :
	{		
		sc_packet_apply_damage* p = reinterpret_cast<sc_packet_apply_damage*>(packet);
		//�������� �ڽ����� �����ϴ� ��Ŷ

		_activeSessionScene->CalculateHP(p->hp);

		break;
	}
	case SC_PLAYER_DEAD :
	{
		sc_packet_player_dead* p = reinterpret_cast<sc_packet_player_dead*>(packet);

		break;
	}
	case SC_PUT_OBJECT :
	{
		sc_packet_put_object* p = reinterpret_cast<sc_packet_put_object*>(packet);

		_activeSessionScene->CreateGameObject(p->approx_num, p->obj_type, p->id);
		//x, y, z ���� p->approx_pos(���� ��ȣ) �� ���ؼ� ���ϱ�
		//_activeSessionScene->CreatePlayerObject(OT_KEYCARD, p->id, p->x, p->y, p->z, 0, p->dirx, p->diry + 3.14f, p->dirz);
		break;
	}
	case SC_MODIFY_BULLET :
	{
		sc_packet_modify_bullet* p = reinterpret_cast<sc_packet_modify_bullet*>(packet);
		//�Ѿ� ������ �ٲٴ� ��Ŷ��.
		//ui �ֽ�ȭ

		_activeSessionScene->CalculateBullet(p->amount);
		break;
	}
	case SC_SHOW_MAP :
	{
		sc_packet_show_map* p = reinterpret_cast<sc_packet_show_map*>(packet);
		//map ui��  ���� ��
		//if(���� �ȶ�������) �� ����
		//else �� �����
		if (!isMapOpen)
		{
			isMapOpen = true;
			_activeSessionScene->SetMapPosition(0, 0);
		}
		else if (isMapOpen)
		{
			isMapOpen = false;
			_activeSessionScene->SetMapPosition(111111111110, 1111111111110);
		}
		break;
	}
	case SC_SET_ANIMATION :
	{
		sc_packet_set_animation* p = reinterpret_cast<sc_packet_set_animation*>(packet);
		//p->obj_id / p->animation_id
		//obj_id ������Ʈ�� animation ����

		//_activeSessionScene->ChangeObjectAnimation(p->obj_id, p->animation_id);
		break;
	}
	case SC_CARD_USED :
	{
		//���⼭ ī�� UI �ϳ� ����� ��
		_activeSessionScene->SetKeyCardPosition(-1111111111111, -111111111111111, haveKeycard);
		haveKeycard++;
		break;
	}
	default: // �������� ���� ��Ŷ�� ���Ź޾��� ��
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

void SESSION::set_moving(bool mv)
{
	moving = mv;
}

bool SESSION::get_moving()
{
	return moving;
}

SESSION* session;

void MoveSession(tcp::socket& sock)
{
	session = new SESSION(std::move(sock));
}

SESSION* GetSession() { return session; }