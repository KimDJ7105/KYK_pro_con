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

		std::cout << "플레이어 " << my_id_ << "가 플레이어 " << p->target_id << "를 공격했습니다.\n";
		
		target->hp -= WP_DMG[equip_weapon];

		std::cout << "플레이어 " << p->target_id << " Remain HP : " << target->hp << std::endl;


		
		sc_packet_apply_damage pad;
		pad.type = SC_APPLY_DAMAGE;
		pad.size = sizeof(sc_packet_apply_damage);
		pad.id = p->target_id;
		pad.hp = target->hp;

		target->Send_Packet(&pad);

		if (target->hp <= 0) { //플레이어 체력이 0보다 낮아지면 모든 플레이어에게 사망을 전달한다.
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

		std::cout << "카드키 획득 요청 수신\n";
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

		std::cout << "터미널 사용 요청 수신\n";

		shared_ptr<SESSION> user = players[my_id_];
		if (user == nullptr) break;

		shared_ptr<OBJECT> terminal = objects[p->terminal_id];
		if (terminal == nullptr) break;

		int key_id = find_useable_key();

		if (terminal->owner_id == -1) { //아직 활성화 되지 않았으면 
			if (key_id == -1) break;

			//owner_id를 my_id로 변환
			terminal->owner_id = my_id_;

			//플레이어가 가진 카드키를 삭제
			shared_ptr<OBJECT> key = objects[key_id];
			if (key == nullptr) break;
			key->owner_id = -1;

			std::cout << "카드키" << key_id << "사용됨, 단말기" << p->terminal_id << " 활성화\n";
		}
		
		else { //이미 활성화 된 터미널이면
			
		}

		//요청한 user가 카드키를 가지고 있는지 확인하고
		//카드키가 없으면 아무것도 안하고 넘어가고
		//카드키가 있으면 소지 카드키에서 제외하고 터미널을 활성화 시킨다.
		//질문 1. 터미널을 활성화 하면 활성화 한 본인 이외에 다른 유저도 사용 가능한가?
		//질문 2. 한번 켜진 터미널은 E를 통해 상호작용하면 어케됨?
		//질문 2에 따른 해결 방법 : 터미널에 owner_id가 -1이면 카드키로 활성화
		//활성화 된 터미널은 owner_id가 활성화 한 유저의 id로 변함, 상호작용시 맵을..? 띄운다.
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
	pl.bullet_amount = 30; //현재 유일한 무기 기관단총의 장탄 수 차후 수정 필요
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

	//생성되어있는 기본 오브젝트의 위치를 전송
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