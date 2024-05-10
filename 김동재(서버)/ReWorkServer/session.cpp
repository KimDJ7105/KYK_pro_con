#pragma once
#include "session.h"

int box_id = MAX_USER;
int WP_DMG[5]{ 6,0,0,0,0 };

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

			player->Send_Packet(&pos_pack);
		}

		//std::cout << "Packet Sended From Client " << id << " x : " << p->x << " y : " << p->y << " z : " << p->z << std::endl;

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

			player->Send_Packet(&pos_pack);
		}

		break;
	}
	case CS_PICKING_INFO :
	{
		cs_packet_picking_info* p = (cs_packet_picking_info*)packet;

		if(remain_bullet <= 0) break;

		remain_bullet -= 1;

		sc_packet_modify_bullet mb;
		mb.type = SC_MODIFY_BULLET;
		mb.size = sizeof(sc_packet_modify_bullet);
		mb.amount = -1;
		Send_Packet(&mb);
		
		if (p->target_id == -1) break;

		shared_ptr<SESSION> target = players[p->target_id];

		if (target == nullptr) break;

		std::cout << "�÷��̾� " << my_id_ << "�� �÷��̾� " << p->target_id << "�� �����߽��ϴ�.\n";
		
		target->hp -= WP_DMG[equip_weapon];

		std::cout << "�÷��̾� " << p->target_id << " Remain HP : " << target->hp << std::endl;


		
		sc_packet_apply_damage pad;
		pad.type = SC_APPLY_DAMAGE;
		pad.size = sizeof(sc_packet_apply_damage);
		pad.id = p->target_id;
		pad.hp = target->hp;

		target->Send_Packet(&pad);

		if (target->hp <= 0) { //�÷��̾� ü���� 0���� �������� ��� �÷��̾�� ����� �����Ѵ�.
			sc_packet_player_dead pd;
			pd.type = SC_PLAYER_DEAD;
			pd.size = sizeof(sc_packet_player_dead);
			pd.id = target->my_id_;

			for (auto& pl : players) {
				shared_ptr<SESSION> player = pl.second;
				if (player == nullptr) continue;

				player->Send_Packet(&pd);
			}
		}
			
		break;
	}
	case CS_TRY_GET_KEY :
	{
		cs_packet_try_get_key* p = (cs_packet_try_get_key*)packet;

		shared_ptr<OBJECT> card = objects[p->key_id];
		if (card == nullptr) break;

		std::cout << "ī��Ű ȹ�� ��û ����\n";
		card->owner_id = my_id_;

		sc_packet_remove_player rmp;
		rmp.type = SC_REMOVE_PLAYER;
		rmp.size = sizeof(sc_packet_remove_player);
		rmp.id = card->obj_id;
		rmp.obj_type = OT_KEYCARD;
		for (auto& p : players) {
			shared_ptr<SESSION> player = p.second;
			if (player == nullptr) continue;
			player->Send_Packet(&rmp);
		}

		break;
	}
	case CS_TRY_USE_TMN :
	{
		cs_packet_try_use_tmn* p = (cs_packet_try_use_tmn*)packet;

		std::cout << "�͹̳� ��� ��û ����\n";

		shared_ptr<SESSION> user = players[my_id_];
		if (user == nullptr) break;

		shared_ptr<OBJECT> terminal = objects[p->terminal_id];
		if (terminal == nullptr) break;

		int key_id = find_useable_key();

		if (terminal->owner_id == -1) { //���� Ȱ��ȭ ���� �ʾ����� 
			if (key_id == -1) break;

			//owner_id�� my_id�� ��ȯ
			terminal->owner_id = my_id_;

			//�÷��̾ ���� ī��Ű�� ����
			shared_ptr<OBJECT> key = objects[key_id];
			if (key == nullptr) break;
			key->owner_id = -1;

			std::cout << "ī��Ű" << key_id << "����, �ܸ���" << p->terminal_id << " Ȱ��ȭ\n";
		}
		
		else { //�̹� Ȱ��ȭ �� �͹̳��̸�
			
		}

		//��û�� user�� ī��Ű�� ������ �ִ��� Ȯ���ϰ�
		//ī��Ű�� ������ �ƹ��͵� ���ϰ� �Ѿ��
		//ī��Ű�� ������ ���� ī��Ű���� �����ϰ� �͹̳��� Ȱ��ȭ ��Ų��.
		//���� 1. �͹̳��� Ȱ��ȭ �ϸ� Ȱ��ȭ �� ���� �̿ܿ� �ٸ� ������ ��� �����Ѱ�?
		//���� 2. �ѹ� ���� �͹̳��� E�� ���� ��ȣ�ۿ��ϸ� ���ɵ�?
		//���� 2�� ���� �ذ� ��� : �͹̳ο� owner_id�� -1�̸� ī��Ű�� Ȱ��ȭ
		//Ȱ��ȭ �� �͹̳��� owner_id�� Ȱ��ȭ �� ������ id�� ����, ��ȣ�ۿ�� ����..? ����.
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

int SESSION::find_useable_key()
{
	for (auto& object : objects) {
		shared_ptr<OBJECT> key = object.second;
		if (key == nullptr) continue;
		if (key->obj_type != OT_KEYCARD) continue;
		if (key->owner_id == my_id_) return key->obj_id;
	}

	return -1;
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
	remain_bullet = 30;
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
	pl.bullet_amount = 30; //���� ������ ���� ��������� ��ź �� ���� ���� �ʿ�
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

	//Ŭ���̾�Ʈ�� ���������� ��� �ٸ� �������� ����
	for (auto& pl : players) {
		if (pl.second != nullptr)
			pl.second->Send_Packet(&p);
	}

	//�ٸ� �������� ������ Ŭ���̾�Ʈ���� ����
	for (auto& pl : players) {
		if (pl.second->my_id_ != my_id_) {
			p.id = pl.second->my_id_;
			p.x = pl.second->pos[0];
			p.y = pl.second->pos[1];
			Send_Packet(&p);
		}
	}

	//�����Ǿ��ִ� �⺻ ������Ʈ�� ��ġ�� ����
	for (auto& object : objects) {
		shared_ptr<OBJECT> obj = object.second;
		if (obj == nullptr) continue;
		if (obj->owner_id != -1) continue;

		sc_packet_put_object put_obj;
		put_obj.size = sizeof(sc_packet_put_object);
		put_obj.type = SC_PUT_OBJECT;
		put_obj.id = obj->obj_id;
		put_obj.obj_type = obj->obj_type;
		put_obj.approx_num = obj->spawn_num;

		Send_Packet(&put_obj);
	}

}

void SESSION::Send_Packet(void* packet)
{
	int packet_size = reinterpret_cast<unsigned char*>(packet)[0];
	unsigned char* buff = new unsigned char[packet_size];
	memcpy(buff, packet, packet_size);
	do_write(buff, packet_size);
}