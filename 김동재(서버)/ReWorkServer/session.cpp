#pragma once
#include "session.h"

int box_id = MAX_USER;

void SESSION::Send_Packet(void* packet, unsigned id)
{
	int packet_size = reinterpret_cast<unsigned char*>(packet)[0];
	unsigned char* buff = new unsigned char[packet_size];
	memcpy(buff, packet, packet_size);
	players[id]->do_write(buff, packet_size);
}

void SESSION::Process_Packet(unsigned char* packet, int id)
{
	auto P = players[id];
	int y = P->pos[1];
	int x = P->pos[0];
	switch (packet[1]) {
	case CS_POS_INFO: {
		cs_packet_pos_info* p = (cs_packet_pos_info*)packet;

		pos[0] = p->x;
		pos[1] = p->y;
		pos[2] = p->z;

		sc_packet_pos pos_pack;
		pos_pack.id = id;
		pos_pack.size = sizeof(sc_packet_pos);
		pos_pack.type = SC_POS;
		pos_pack.x = p->x;
		pos_pack.y = p->y;
		pos_pack.z = p->z;
		pos_pack.dirx = P->view_dir[0];
		pos_pack.diry = P->view_dir[1];
		pos_pack.dirz = P->view_dir[2];

		for (auto& pl : players) {
			shared_ptr<SESSION> player = pl.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;

			player->Send_Packet(&pos_pack);
		}

		//std::cout << "Packet Sended From Client " << id << " x : " << p->x << " y : " << p->y << " z : " << p->z << std::endl;

		break;
	}
	case CS_BOX_CREATE: {
		cs_packet_box_create* p = (cs_packet_box_create*)packet;

		sc_packet_create_box cb;
		cb.size = sizeof(sc_packet_create_box);
		cb.type = SC_CREATE_BOX;
		cb.x = p->x;
		cb.y = p->y;
		cb.z = p->z;
		cb.id = ++box_id;

		for (auto& pl : players) {
			shared_ptr<SESSION> player = pl.second;
			if (player == nullptr) continue;

			std::cout << "Send Create Box Packet\n";

			player->Send_Packet(&cb);
		}

		std::cout << "Box Is Created! orderID : " << cb.id << " x : " << p->x << " y : " << p->y << " z : " << p->z << std::endl;
		break;
	}
	case CS_MOUSE_INFO: {
		cs_packet_mouse_info* p = (cs_packet_mouse_info*)packet;

		view_dir[0] = p->x;
		view_dir[1] = p->y;
		view_dir[2] = p->z;

		sc_packet_pos pos_pack;
		pos_pack.type = SC_POS;
		pos_pack.size = sizeof(sc_packet_pos);
		pos_pack.id = id;
		pos_pack.x = pos[0];
		pos_pack.y = pos[1];
		pos_pack.z = pos[2];
		pos_pack.dirx = p->x;
		pos_pack.diry = p->y;
		pos_pack.dirz = p->z;


		for (auto& pl : players) {
			shared_ptr<SESSION> player = pl.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;

			player->Send_Packet(&pos_pack);
		}

		break;
	}
	case CS_PICKING_INFO :
	{
		cs_packet_picking_info* p = (cs_packet_picking_info*)packet;

		shared_ptr<SESSION> target = players[p->target_id];
		shared_ptr<SESSION> shooter = players[p->shooter_id];


		std::cout << "플레이어 " << p->shooter_id << "가 플레이어 " << p->target_id << "를 공격했습니다.\n";

		if (target != nullptr && shooter != nullptr) {
			target->hp -= WP_DMG[shooter->equip_weapon];

			std::cout << "플레이어 " << p->target_id << " Remain HP : " << target->hp;

			if (target->hp > 0) {
				sc_packet_apply_damage pad;
				pad.type = SC_APPLY_DAMAGE;
				pad.size = sizeof(sc_packet_apply_damage);
				pad.id = p->target_id;
				pad.hp = target->hp;

				target->Send_Packet(&pad);
			}

			else {

			}
			
		}
		break;
	}
	default: cout << "Invalid Packet From Client [" << id << "]\n"; system("pause"); exit(-1);
	}

	/*sc_packet_pos sp;

	sp.id = id;
	sp.size = sizeof(sc_packet_pos);
	sp.type = SC_POS;
	sp.x = P->pos_x;
	sp.y = P->pos_y;

	for (auto& pl : players)
		pl.second->Send_Packet(&sp);*/
}

void SESSION::do_read()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				if (ec.value() == boost::asio::error::operation_aborted) return;
				cout << "Receive Error on Session[" << my_id_ << "] EC[" << ec << "]\n";
				players[my_id_] = nullptr;
				//players.unsafe_erase(my_id_);
				return;
			}

			int data_to_process = static_cast<int>(length);
			unsigned char* buf = data_;
			while (0 < data_to_process) {
				if (0 == curr_packet_size_) {
					curr_packet_size_ = buf[0];
					if (buf[0] > 200) {
						cout << "Invalid Packet Size [ << buf[0] << ] Terminating Server!\n";
						exit(-1);
					}
				}
				int need_to_build = curr_packet_size_ - prev_data_size_;
				if (need_to_build <= data_to_process) {
					memcpy(packet_ + prev_data_size_, buf, need_to_build);
					Process_Packet(packet_, my_id_);
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

void SESSION::do_write(unsigned char* packet, std::size_t length)
{
	auto self(shared_from_this());
	socket_.async_write_some(boost::asio::buffer(packet, length),
		[this, self, packet, length](boost::system::error_code ec, std::size_t bytes_transferred)
		{
			if (!ec)
			{
				if (length != bytes_transferred) {
					cout << "Incomplete Send occured on session[" << my_id_ << "]. This session should be closed.\n";
				}
				delete packet;
			}
		});
}

SESSION::SESSION(tcp::socket socket, int new_id)
	: socket_(std::move(socket)), my_id_(new_id)
{
	curr_packet_size_ = 0;
	prev_data_size_ = 0;
	pos[0] = 0.f;
	pos[1] = 40.f;
	pos[2] = 0.f;

	view_dir[0] = 0.0f;
	view_dir[1] = 0.0f;
	view_dir[2] = 0.0f;

	hp = 100;
	remain_bullet = 0;
	team = 0;

	equip_weapon = WP_SMG;
}

void SESSION::start()
{
	do_read();

	sc_packet_login_info pl;
	pl.id = my_id_;
	pl.size = sizeof(sc_packet_login_info);
	pl.type = SC_LOGIN_INFO;
	pl.x = pos[0];
	pl.y = pos[1];
	pl.z = pos[2];
	pl.dirx = view_dir[0];
	pl.diry = view_dir[1];
	pl.dirz = view_dir[2];
	Send_Packet(&pl);

	sc_packet_put p;
	p.id = my_id_;
	p.size = sizeof(sc_packet_put);
	p.type = SC_PUT_PLAYER;
	p.x = pos[0];
	p.y = pos[1];
	p.z = pos[2];
	p.dirx = view_dir[0];
	p.diry = view_dir[1];
	p.dirz = view_dir[2];

	//클라이언트가 입장했음을 모든 다른 유저에게 전송
	for (auto& pl : players) {
		if (pl.second != nullptr)
			pl.second->Send_Packet(&p);
	}

	//다른 유저들의 정보를 클라이언트에게 전송
	for (auto& pl : players) {
		if (pl.second->my_id_ != my_id_) {
			p.id = pl.second->my_id_;
			p.x = pl.second->pos[0];
			p.y = pl.second->pos[1];
			Send_Packet(&p);
		}
	}

}

void SESSION::Send_Packet(void* packet)
{
	int packet_size = reinterpret_cast<unsigned char*>(packet)[0];
	unsigned char* buff = new unsigned char[packet_size];
	memcpy(buff, packet, packet_size);
	do_write(buff, packet_size);
}