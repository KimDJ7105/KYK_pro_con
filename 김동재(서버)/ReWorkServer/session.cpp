#pragma once
#include "session.h"
#include "game.h"
#include "server.h"

int WP_DMG[5]{ 8,80,70,20,10 };
int WP_MAG[5]{ 30,8,5,25,15 };
float WP_COOLTIME[5]{ 0.1f, 1.f, 1.f, 0.125f, 0.333f };

void SESSION::Send_Packet(void* packet, unsigned id)
{
	int packet_size = reinterpret_cast<unsigned char*>(packet)[0];
	unsigned char* buff = new unsigned char[packet_size];
	memcpy(buff, packet, packet_size);
	my_game->ingame_player[id]->do_write(buff, packet_size);
}

void SESSION::Process_Packet(unsigned char* packet, int id)
{

	auto P = my_game->ingame_player[id];
	int y = P->pos[1];
	int x = P->pos[0];
	//ready 상태일때는 마우스 이동 동기화 말고는 아무것도 안하기
	if (my_game->get_game_state() == ST_READY && packet[1] != CS_MOUSE_INFO && packet[1] != CS_SEND_GUNTYPE && packet[1] != CS_CHANGE_GUN) {
		std::cout << "Nope\n";
		return;
	}

	switch (packet[1]) {
	case CS_POS_INFO: { //플레이어 이동
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
		pos_pack.animation_id = AT_WALKING;

		for (auto& pl : my_game->ingame_player) {
			shared_ptr<SESSION> player = pl.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;

			player->Send_Packet(&pos_pack);
		}

		//std::cout << "Packet Sended From Client " << id << " x : " << p->x << " y : " << p->y << " z : " << p->z << std::endl;

		break;
	}
	case CS_MOUSE_INFO: { //플레이어 마우스 움직임
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
		pos_pack.animation_id = -1;

		for (auto& pl : my_game->ingame_player) {
			shared_ptr<SESSION> player = pl.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;

			player->Send_Packet(&pos_pack);
		}

		break;
	}
	case CS_PICKING_INFO : { //플레이어 사격 시도
		cs_packet_picking_info* p = (cs_packet_picking_info*)packet;

		if(remain_bullet[select_gun] <= 0) break;

		remain_bullet[select_gun] -= 1;

		sc_packet_set_animation set_anima;
		set_anima.type = SC_SET_ANIMATION;
		set_anima.size = sizeof(sc_packet_set_animation);
		set_anima.obj_id = my_id_;
		set_anima.animation_id = AT_SHOOTING;

		for (auto& p : my_game->ingame_player) {
			shared_ptr<SESSION> player = p.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;
			player->Send_Packet(&set_anima);
		}

		sc_packet_modify_bullet mb;
		mb.type = SC_MODIFY_BULLET;
		mb.size = sizeof(sc_packet_modify_bullet);
		mb.amount = remain_bullet[select_gun];
		Send_Packet(&mb);
		
		if (p->target_id == -1) break;

		shared_ptr<SESSION> target = my_game->ingame_player[p->target_id];

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
			target->is_core_state = true;
			sc_packet_player_dead pd;
			pd.type = SC_PLAYER_DEAD;
			pd.size = sizeof(sc_packet_player_dead);
			pd.id = target->my_id_;

			for (auto& pl : my_game->ingame_player) {
				pl.second->Send_Packet(&pd);
			}

			for (auto& object : my_game->ingame_object) {
				auto& obj = object.second;
				if (obj->obj_type == OT_KEYCARD && obj->owner_id == my_id_) {
					obj->owner_id = -1;
					obj->set_pos(my_game->select_pos());

					sc_packet_put_object put_obj;
					put_obj.size = sizeof(sc_packet_put_object);
					put_obj.type = SC_PUT_OBJECT;
					put_obj.id = obj->obj_id;
					put_obj.obj_type = obj->obj_type;
					put_obj.approx_num = obj->spawn_num;

					for (auto& player : my_game->ingame_player) {
						player.second->Send_Packet(&put_obj);
					}
				}

				else if (obj->obj_type == OT_RABBITFOOT && obj->owner_id == my_id_) {
					obj->owner_id = -1;
					my_game->set_rabbitfoot_owner(-1);
					obj->set_pos(my_game->select_room_pos());

					sc_packet_put_object put_obj;
					put_obj.size = sizeof(sc_packet_put_object);
					put_obj.type = SC_PUT_OBJECT;
					put_obj.id = obj->obj_id;
					put_obj.obj_type = obj->obj_type;
					put_obj.approx_num = obj->spawn_num;

					for (auto& player : my_game->ingame_player) {
						player.second->Send_Packet(&put_obj);
					}
				}
			}
		}
			
		break;
	}
	case CS_USE_MEDIKIT: {
	
		cs_packet_use_medikit* p = (cs_packet_use_medikit*)packet;

		shared_ptr<OBJECT> kit = my_game->ingame_object[p->kit_id];
		if (kit == nullptr) break;

		my_game->set_free_space(OT_MEDIKIT, kit->spawn_num);

		//플레이어 체력을 증가
		hp += 30;
		if (hp > 100) hp = 100;

		sc_packet_apply_damage pad;
		pad.type = SC_APPLY_DAMAGE;
		pad.size = sizeof(sc_packet_apply_damage);
		pad.id = my_id_;
		pad.hp = hp;

		Send_Packet(&pad);

		//사용된 메디킷을 삭제
		sc_packet_remove_player rmp;
		rmp.type = SC_REMOVE_PLAYER;
		rmp.size = sizeof(sc_packet_remove_player);
		rmp.id = kit->obj_id;
		rmp.obj_type = OT_MEDIKIT;

		for (auto& player : my_game->ingame_player) {
			auto& p = player.second;
			if (p == nullptr) continue;
			p->Send_Packet(&rmp);
		}

		auto it = my_game->ingame_object.find(p->kit_id);
		if (it != my_game->ingame_object.end()) {
			my_game->ingame_object.erase(it);
		}

		break;
	}
	case CS_TRY_GET_KEY : { //카드키 획득 시도
	
		cs_packet_try_get_key* p = (cs_packet_try_get_key*)packet;

		shared_ptr<OBJECT> card = my_game->ingame_object[p->key_id];
		if (card == nullptr) break;

		std::cout << "카드키 획득 요청 수신\n";
		card->owner_id = my_id_;
		my_game->set_free_space(OT_KEYCARD,card->spawn_num);

		sc_packet_remove_player rmp;
		rmp.type = SC_REMOVE_PLAYER;
		rmp.size = sizeof(sc_packet_remove_player);
		rmp.id = card->obj_id;
		rmp.obj_type = OT_KEYCARD;
		for (auto& player : my_game->ingame_player) {
			auto& p = player.second;
			if (p == nullptr) continue;
			p->Send_Packet(&rmp);
		}

		break;
	}
	case CS_TRY_USE_TMN : { //터미널 사용 시도
	
		cs_packet_try_use_tmn* p = (cs_packet_try_use_tmn*)packet;

		std::cout << "터미널 사용 요청 수신\n";

		if (using_tml_id != -1) {
			using_tml_id = -1;

			sc_packet_show_map sm;
			sm.type = SC_SHOW_MAP;
			sm.size = sizeof(sc_packet_show_map);

			Send_Packet(&sm);
			break;
		}

		shared_ptr<SESSION> user = my_game->ingame_player[my_id_];
		if (user == nullptr) break;

		shared_ptr<OBJECT> terminal = my_game->ingame_object[p->terminal_id];
		if (terminal == nullptr) break;

		int key_id = find_useable_key();

		if (terminal->owner_id == -1) { //아직 활성화 되지 않았으면 
			if (key_id == -1) break;

			//owner_id를 my_id로 변환
			terminal->owner_id = my_id_;

			//플레이어가 가진 카드키를 삭제
			auto it = my_game->ingame_object.find(key_id);
			if (it != my_game->ingame_object.end()) {
				my_game->ingame_object.erase(it);
			}

			std::cout << "카드키" << key_id << "사용됨, 단말기" << p->terminal_id << " 활성화\n";

			using_tml_id = terminal->obj_id;

			sc_packet_card_used cu;
			cu.size = sizeof(sc_packet_card_used);
			cu.type = SC_CARD_USED;

			sc_packet_show_map sm;
			sm.type = SC_SHOW_MAP;
			sm.size = sizeof(sc_packet_show_map);

			Send_Packet(&cu);
			Send_Packet(&sm);

			for (auto& object : my_game->ingame_object) {
				shared_ptr<OBJECT> obj = object.second;
				if (obj->obj_type == OT_GRINDER) continue; //분쇄기는 맵에 표시 X
				//아직 주인이 없는 키카드, 터미널, 토끼발이 있다면 토끼발 위치
				if (obj->obj_type == OT_KEYCARD && obj->owner_id != -1) continue;
				if (obj->obj_type == OT_RABBITFOOT && obj->owner_id != -1) continue;
				if (obj->obj_type == OT_EXIT) continue;

				sc_packet_show_object_loc sol;
				sol.type = SC_SHOW_OBJECT_LOC;
				sol.size = sizeof(sc_packet_show_object_loc);
				sol.obj_type = obj->obj_type;
				sol.approx_num = obj->spawn_num;
				if (obj->obj_type == OT_KEYCARD || obj->obj_type == OT_TERMINAL) sol.loc_type = OT_CORRIDOR;
				else sol.loc_type = OT_ROOM;

				Send_Packet(&sol);
			}

			//만약 모든 터미널이 활성화 되었으면 토끼발을 생성
			if (my_game->IsTerminalOn()) {
				auto& rabbitfoot = my_game->CreateObjectApprox(OT_RABBITFOOT);

				sc_packet_put_object_pos pop;
				pop.size = sizeof(sc_packet_put_object_pos);
				pop.type = SC_PUT_OBJECT_POS;
				pop.id = rabbitfoot->obj_id;
				pop.obj_type = rabbitfoot->obj_type;
				pop.approx_num = rabbitfoot->spawn_num;

				for (auto& p : my_game->ingame_player) {
					auto& player = p.second;
					if (player == nullptr) continue;
					player->Send_Packet(&pop);
				}

				TIMER_EVENT tm_grind;
				tm_grind.event_id = EV_MOVE_GRINDER;
				tm_grind.game_id = my_game->get_game_id();
				tm_grind.target_id = -1;
				tm_grind.wakeup_time = chrono::system_clock::now() + 1s;

				my_server->timer_queue.emplace(tm_grind);

				TIMER_EVENT tm_exit;
				tm_exit.event_id = EV_SPAWN_EXIT;
				tm_exit.game_id = my_game->get_game_id();
				tm_exit.target_id = -1;
				tm_exit.wakeup_time = chrono::system_clock::now() + 90s;

				my_server->timer_queue.emplace(tm_exit);
			}

			
		}
		
		else { //이미 활성화 된 터미널이면
			using_tml_id = terminal->obj_id;

			sc_packet_show_map sm;
			sm.type = SC_SHOW_MAP;
			sm.size = sizeof(sc_packet_show_map);

			Send_Packet(&sm);

			//다른 오브젝트의 위치를 전달
			//현재 존재하는 오브젝트 : 키카드, 터미널, 토끼발, 부활패드, 탈출구, 분쇄기
			for (auto& object : my_game->ingame_object) {
				shared_ptr<OBJECT> obj = object.second;
				if (obj->obj_type == OT_GRINDER) continue; //분쇄기는 맵에 표시 X
				//아직 주인이 없는 키카드, 터미널, 토끼발이 있다면 토끼발 위치
				if (obj->obj_type == OT_KEYCARD && obj->owner_id != -1) continue;
				if (obj->obj_type == OT_RABBITFOOT && obj->owner_id != -1) continue;

				sc_packet_show_object_loc sol;
				sol.type = SC_SHOW_OBJECT_LOC;
				sol.size = sizeof(sc_packet_show_object_loc);
				sol.obj_type = obj->obj_type;
				sol.approx_num = obj->spawn_num;
				if (obj->obj_type == OT_KEYCARD || obj->obj_type == OT_TERMINAL) sol.loc_type = OT_CORRIDOR;
				else sol.loc_type = OT_ROOM;

				Send_Packet(&sol);
			}
		}

		break;
	}
	case CS_RELOAD_MAG: { //재장전
		remain_bullet[select_gun] = WP_MAG[equip_weapon];

		sc_packet_set_animation set_anima;
		set_anima.type = SC_SET_ANIMATION;
		set_anima.size = sizeof(sc_packet_set_animation);
		set_anima.obj_id = my_id_;
		set_anima.animation_id = AT_RELOADING;

		for (auto& p : my_game->ingame_player) {
			shared_ptr<SESSION> player = p.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;
			player->Send_Packet(&set_anima);
		}

		sc_packet_modify_bullet mb;
		mb.type = SC_MODIFY_BULLET;
		mb.size = sizeof(sc_packet_modify_bullet);
		mb.amount = WP_MAG[equip_weapon];

		Send_Packet(&mb);
		break;
	}
	case CS_MOVE_KEY_DOWN: { //이동 애니메이션 동기화를 위해 이동 키가 눌림을 수신
		is_moving = true;

		sc_packet_set_animation set_anima;
		set_anima.type = SC_SET_ANIMATION;
		set_anima.size = sizeof(sc_packet_set_animation);
		set_anima.obj_id = my_id_;
		if (!is_running) set_anima.animation_id = AT_WALKING;
		else set_anima.animation_id = AT_RUNNING;

		for (auto& p : my_game->ingame_player) {
			shared_ptr<SESSION> player = p.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;
			player->Send_Packet(&set_anima);
		}

		break;
	}
	case CS_MOVE_KEY_UP: { //이동 키가 떨어짐
		is_moving = false;
		sc_packet_set_animation set_anima;
		set_anima.type = SC_SET_ANIMATION;
		set_anima.size = sizeof(sc_packet_set_animation);
		set_anima.obj_id = my_id_;
		set_anima.animation_id = AT_IDLE;

		for (auto& p : my_game->ingame_player) {
			shared_ptr<SESSION> player = p.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;
			player->Send_Packet(&set_anima);
		}

		break;
	}
	case CS_RUN_KEY_DOWN: {

		is_running = true;

		if (is_moving) {
			sc_packet_set_animation set_anima;
			set_anima.type = SC_SET_ANIMATION;
			set_anima.size = sizeof(sc_packet_set_animation);
			set_anima.obj_id = my_id_;
			set_anima.animation_id = AT_RUNNING;

			for (auto& p : my_game->ingame_player) {
				shared_ptr<SESSION> player = p.second;
				if (player == nullptr) continue;
				if (player->my_id_ == my_id_) continue;
				player->Send_Packet(&set_anima);
			}
		}
		break;
	}
	case CS_RUN_KEY_UP: {

		is_running = false;
		break;
	}
	case CS_TRY_GET_RABBITFOOT: {
		cs_packet_try_get_rabbitfoot* p = (cs_packet_try_get_rabbitfoot*)packet;

		shared_ptr<OBJECT> foot = my_game->ingame_object[p->obj_id];
		if (foot == nullptr) break;

		std::cout << "토끼발 획득 요청 수신\n";
		foot->owner_id = my_id_;
		my_game->set_rabbitfoot_owner(my_id_);

		sc_packet_remove_player rmp;
		rmp.type = SC_REMOVE_PLAYER;
		rmp.size = sizeof(sc_packet_remove_player);
		rmp.id = foot->obj_id;
		rmp.obj_type = OT_RABBITFOOT;

		for (auto& p : my_game->ingame_player) {
			auto& player = p.second;
			if (player == nullptr) continue;
			p.second->Send_Packet(&rmp);
		}
		break;
	}
	case CS_TRY_ESCAPE: {
		if (my_game->get_rabbitfoot_owner() == my_id_) {
			//탈출 성공
			my_game->set_game_state(ST_END);

			sc_packet_player_win pw;
			pw.size = sizeof(sc_packet_player_win);
			pw.type = SC_PLAYER_WIN;

			sc_packet_player_lose pl;
			pl.size = sizeof(sc_packet_player_lose);
			pl.type = SC_PLAYER_LOSE;

			for (auto& p : my_game->ingame_player) {
				auto& player = p.second;
				if (player == nullptr) continue;

				if (player->team == team) {
					player->Send_Packet(&pw);
				}

				else player->Send_Packet(&pl);

			}

			for (auto& p : my_game->ingame_player) {
				auto& player = p.second;
				if (player == nullptr) continue;

				player->socket_.close();
			}
			my_game->ingame_player.clear();
		}
		break;
	}
	case CS_HIT_BY_GRINDER : {
		sc_packet_remove_player rp;
		rp.size = sizeof(sc_packet_remove_player);
		rp.type = SC_REMOVE_PLAYER;
		rp.id = my_id_;
		rp.obj_type = OT_PLAYER;

		for (auto& p : my_game->ingame_player) {
			auto& player = p.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;

			player->Send_Packet(&rp);
		}

		for (auto& object : my_game->ingame_object) {
			auto& obj = object.second;
			if (obj->obj_type == OT_KEYCARD && obj->owner_id == my_id_) {
				obj->owner_id = -1;
				obj->set_pos(my_game->select_pos());

				sc_packet_put_object put_obj;
				put_obj.size = sizeof(sc_packet_put_object);
				put_obj.type = SC_PUT_OBJECT;
				put_obj.id = obj->obj_id;
				put_obj.obj_type = obj->obj_type;
				put_obj.approx_num = obj->spawn_num;

				for (auto& player : my_game->ingame_player) {
					auto& p = player.second;
					if (p == nullptr) continue;

					p->Send_Packet(&put_obj);
				}
			}

			else if (obj->obj_type == OT_RABBITFOOT && obj->owner_id == my_id_) {
				obj->owner_id = -1;
				my_game->set_rabbitfoot_owner(-1);
				obj->set_pos(my_game->select_room_pos());

				sc_packet_put_object_pos pop;
				pop.size = sizeof(sc_packet_put_object_pos);
				pop.type = SC_PUT_OBJECT_POS;
				pop.id = obj->obj_id;
				pop.obj_type = obj->obj_type;
				pop.approx_num = obj->spawn_num;

				for (auto& player : my_game->ingame_player) {
					auto& p = player.second;
					if (p == nullptr) continue;

					p->Send_Packet(&pop);
				}
			}
		}

		sc_packet_player_lose pl;
		pl.size = sizeof(sc_packet_player_lose);
		pl.type = SC_PLAYER_LOSE;

		Send_Packet(&pl);

		socket_.close();

		my_game->ingame_player.erase(my_id_);

		break;
	}
	case CS_TRIGGER_LASER: {
		if (using_tml_id == -1) break;
		auto& terminal = my_game->ingame_object[using_tml_id];
		if (terminal->triggered_laser >= 2) break;
		
		cs_packet_trigger_laser* p = (cs_packet_trigger_laser*)packet;

		std::cout << "Laser Trap Triggered\n";

		sc_packet_show_object_loc sol;
		sol.type = SC_SHOW_OBJECT_LOC;
		sol.size = sizeof(sc_packet_show_object_loc);
		sol.obj_type = OT_LASER;
		sol.approx_num = p->room_num;
		sol.loc_type = OT_ROOM;

		Send_Packet(&sol);

		if (my_game->is_free_room(p->room_num)) {
			my_game->set_room_unable(p->room_num);

			terminal->triggered_laser += 1;

			TIMER_EVENT tm_laser;
			tm_laser.event_id = EV_LASER_TRAP_ON;
			tm_laser.game_id = my_game->get_game_id();
			tm_laser.target_id = p->room_num;
			tm_laser.wakeup_time = chrono::system_clock::now() + 3ms;
			tm_laser.x = p->x - 150.0f;
			tm_laser.y = p->y;
			tm_laser.z = p->z;

			my_server->timer_queue.emplace(tm_laser);
		}
		break;
	}
	case CS_HIT_BY_LASER: {
		is_core_state = true;
		sc_packet_player_dead pd;
		pd.type = SC_PLAYER_DEAD;
		pd.size = sizeof(sc_packet_player_dead);
		pd.id = my_id_;

		for (auto& pl : my_game->ingame_player) {
			pl.second->Send_Packet(&pd);
		}

		for (auto& object : my_game->ingame_object) {
			auto& obj = object.second;
			if (obj->obj_type == OT_KEYCARD && obj->owner_id == my_id_) {
				obj->owner_id = -1;
				obj->set_pos(my_game->select_pos());

				sc_packet_put_object put_obj;
				put_obj.size = sizeof(sc_packet_put_object);
				put_obj.type = SC_PUT_OBJECT;
				put_obj.id = obj->obj_id;
				put_obj.obj_type = obj->obj_type;
				put_obj.approx_num = obj->spawn_num;

				for (auto& player : my_game->ingame_player) {
					auto& p = player.second;
					if (p == nullptr) continue;
					p->Send_Packet(&put_obj);
				}
			}

			else if (obj->obj_type == OT_RABBITFOOT && obj->owner_id == my_id_) {
				obj->owner_id = -1;
				my_game->set_rabbitfoot_owner(-1);
				obj->set_pos(my_game->select_room_pos());

				sc_packet_put_object put_obj;
				put_obj.size = sizeof(sc_packet_put_object);
				put_obj.type = SC_PUT_OBJECT;
				put_obj.id = obj->obj_id;
				put_obj.obj_type = obj->obj_type;
				put_obj.approx_num = obj->spawn_num;

				for (auto& player : my_game->ingame_player) {
					player.second->Send_Packet(&put_obj);
				}
			}
		}
		break;
	}
	case CS_USE_RESURRECTION: {
		if (is_core_state) {
			is_core_state = false;

			hp = 100;

			sc_packet_apply_damage ad;
			ad.size = sizeof(sc_packet_apply_damage);
			ad.type = SC_APPLY_DAMAGE;
			ad.id = my_id_;
			ad.hp = 100;

			sc_packet_set_player_gun spg;
			spg.size = sizeof(sc_packet_set_player_gun);
			spg.type = SC_SET_PLAYER_GUN;
			spg.id = my_id_;
			spg.gun_type = GT_PT;
			select_gun = 1;

			sc_packet_resurrection res;
			res.size = sizeof(sc_packet_resurrection);
			res.type = SC_RESURRECTION;
			res.id = my_id_;

			for (auto& p : my_game->ingame_player) {
				auto& player = p.second;
				if (player == nullptr) continue;

				player->Send_Packet(&res);
				if (player->my_id_ != my_id_) player->Send_Packet(&spg);
			}
		}
		break;
	}
	case CS_SEND_GUNTYPE: {
		cs_packet_send_guntype* p = (cs_packet_send_guntype*)packet;
		gun_type = p->gun_type;
		remain_bullet[0] = WP_MAG[gun_type];

		sc_packet_put put;
		put.id = my_id_;
		put.size = sizeof(sc_packet_put);
		put.type = SC_PUT_PLAYER;
		put.x = pos[0];
		put.y = pos[1];
		put.z = pos[2];
		put.dirx = view_dir[0];
		put.diry = view_dir[1];
		put.dirz = view_dir[2];
		put.gun_type = gun_type;
		put.team = team;

		//클라이언트가 입장했음을 모든 다른 유저에게 전송
		for (auto& pl : my_game->ingame_player) {
			shared_ptr<SESSION> player = pl.second;
			if (player->my_id_ == my_id_) continue;
			player->Send_Packet(&put);
		}

		break;
	}
	case CS_CHANGE_GUN: {
		cs_packet_change_gun* p = (cs_packet_change_gun*)packet;

		if (p->pressed_key == 1) {
			if (equip_weapon == gun_type) break;
			equip_weapon = gun_type;
			select_gun = 0;
		}

		else if (p->pressed_key == 2) {
			if (equip_weapon == GT_PT) break;
			equip_weapon = GT_PT;
			select_gun = 1;
		}

		sc_packet_set_animation set_anima;
		set_anima.type = SC_SET_ANIMATION;
		set_anima.size = sizeof(sc_packet_set_animation);
		set_anima.obj_id = my_id_;
		set_anima.animation_id = AT_CHANGE;
		

		for (auto& p : my_game->ingame_player) {
			shared_ptr<SESSION> player = p.second;
			if (player == nullptr) continue;
			if (player->my_id_ == my_id_) continue;
			player->Send_Packet(&set_anima);
		}

		sc_packet_set_player_gun spg;
		spg.size = sizeof(sc_packet_set_player_gun);
		spg.type = SC_SET_PLAYER_GUN;
		spg.id = my_id_;
		spg.gun_type = equip_weapon;

		for (auto& p : my_game->ingame_player) {
			auto& player = p.second;
			if (player == nullptr) continue;

			if (player->my_id_ == my_id_) {
				sc_packet_modify_bullet mb;
				mb.type = SC_MODIFY_BULLET;
				mb.size = sizeof(sc_packet_modify_bullet);
				mb.amount = remain_bullet[select_gun];

				player->Send_Packet(&mb);
				continue;
			}
			player->Send_Packet(&spg);
		}

		break;
	}
	case TEST_SPAWN_RBF: { //test
		//-------------Test
		std::cout << "테스트 패킷 수신\n";

		auto& rabbitfoot = my_game->CreateObjectApprox(OT_RABBITFOOT);

		sc_packet_put_object_pos pop;
		pop.size = sizeof(sc_packet_put_object_pos);
		pop.type = SC_PUT_OBJECT_POS;
		pop.id = rabbitfoot->obj_id;
		pop.obj_type = rabbitfoot->obj_type;
		pop.approx_num = rabbitfoot->spawn_num;

		for (auto& p : my_game->ingame_player) {
			auto& player = p.second;
			if (player == nullptr) continue;
			player->Send_Packet(&pop);
		}
		break;
	}
	default: cout << "Invalid Packet From Client [" << id << "]\n"; system("pause"); exit(-1);
	}
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
				my_game->ingame_player[my_id_] = nullptr;
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
	for (auto& object : my_game->ingame_object) {
		shared_ptr<OBJECT> key = object.second;
		if (key->obj_type != OT_KEYCARD) continue;
		if (key->owner_id == my_id_) return key->obj_id;
	}

	return -1;
}

SESSION::SESSION(tcp::socket socket, int new_id, int team_num)
	: socket_(std::move(socket)), my_id_(new_id)
{
	curr_packet_size_ = 0;
	prev_data_size_ = 0;

	if (team_num == 0) {
		pos[0] = 1200.f;
		pos[1] = 40.f;
		pos[2] = 2400.f;

		view_dir[0] = 0.0f;
		view_dir[1] = 3.14f;
		view_dir[2] = 0.0f;
	}

	else if (team_num == 1) {
		pos[0] = 0.f;
		pos[1] = 40.f;
		pos[2] = 1200.f;

		view_dir[0] = 0.0f;
		view_dir[1] = 1.57f;
		view_dir[2] = 0.0f;
	}

	else if (team_num == 2) {
		pos[0] = 2400.f;
		pos[1] = 40.f;
		pos[2] = 1200.f;

		view_dir[0] = 0.0f;
		view_dir[1] = -1.57f;
		view_dir[2] = 0.0f;
	}

	else if (team_num == 3) {
		pos[0] = 1200.f;
		pos[1] = 40.f;
		pos[2] = 0.f;

		view_dir[0] = 0.0f;
		view_dir[1] = 0.0f;
		view_dir[2] = 0.0f;
	}

	hp = 100;
	remain_bullet[0] = WP_MAG[0];
	remain_bullet[1] = WP_MAG[GT_PT];

	select_gun = 1;

	equip_weapon = GT_PT;

	team = team_num;

	is_core_state = false;
	is_running = false;

	using_tml_id = -1;
}

void SESSION::start()
{
	do_read();

	if (my_id_ == LOBBY_ID) {
		sl_packet_set_ip s_ip;
		s_ip.type = SL_SET_IP;
		s_ip.size = sizeof(sl_packet_set_ip);
		strcpy_s(s_ip.ip, my_server->get_ip());

		Send_Packet(&s_ip);

		sl_packet_set_port sip;
		sip.type = SL_SET_PORT;
		sip.size = sizeof(sl_packet_set_port);
		strcpy_s(sip.port, MY_SERVER_PORT);

		Send_Packet(&sip);

		return;
	}

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
	pl.team_num = team;
	pl.bullet_amount = WP_MAG[GT_PT]; //현재 유일한 무기 기관단총의 장탄 수 차후 수정 필요
	Send_Packet(&pl);

	//std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;

	//다른 유저들의 정보를 클라이언트에게 전송
	for (auto& pl : my_game->ingame_player) {
		shared_ptr<SESSION> player = pl.second;
		if (player->my_id_ != my_id_) {
			sc_packet_put p;
			p.id = player->my_id_;
			p.size = sizeof(sc_packet_put);
			p.type = SC_PUT_PLAYER;
			p.x = player->pos[0];
			p.y = player->pos[1];
			p.z = player->pos[2];
			p.dirx = player->view_dir[0];
			p.diry = player->view_dir[1];
			p.dirz = player->view_dir[2];
			p.gun_type = player->gun_type;
			p.team = player->team;
			Send_Packet(&p);
		}
	}

	//생성되어있는 기본 오브젝트의 위치를 전송
	for (auto& object : my_game->ingame_object) {
		shared_ptr<OBJECT> obj = object.second;
		if (obj->owner_id != -1 && obj->obj_type == OT_KEYCARD) continue;

		//복도에 생성될 객체들(키카드, 터미널)
		if (obj->obj_type == OT_KEYCARD || obj->obj_type == OT_TERMINAL) {
			sc_packet_put_object put_obj;
			put_obj.size = sizeof(sc_packet_put_object);
			put_obj.type = SC_PUT_OBJECT;
			put_obj.id = obj->obj_id;
			put_obj.obj_type = obj->obj_type;
			put_obj.approx_num = obj->spawn_num;

			Send_Packet(&put_obj);
		}
		
		//방에 생성될 객체들(토끼발, 부활패드, 탈출구 등등)
		if (obj->obj_type == OT_RABBITFOOT || obj->obj_type == OT_RESURRECTION_PAD || obj->obj_type == OT_EXIT) {
			sc_packet_put_object_pos pop;
			pop.type = SC_PUT_OBJECT_POS;
			pop.size = sizeof(sc_packet_put_object_pos);
			pop.obj_type = obj->obj_type;
			pop.id = obj->obj_id;
			pop.approx_num = obj->spawn_num;
			
			Send_Packet(&pop);
		}

		//좌표로 생성될 객체들(분쇄기)
		if (obj->obj_type == OT_GRINDER) {
			sc_packet_put_object_coor poc;
			poc.type = SC_PUT_OBJECT_COOR;
			poc.size = sizeof(sc_packet_put_object_coor);
			poc.obj_type = obj->obj_type;
			poc.obj_id = obj->obj_id;
			poc.x = obj->pos[0];
			poc.y = obj->pos[1];
			poc.z = obj->pos[2];
			poc.dirx = obj->rot[0];
			poc.diry = obj->rot[1];
			poc.dirz = obj->rot[2];

			Send_Packet(&poc);
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

void SESSION::set_mygame(std::shared_ptr<GAME> p)
{
	my_game = p;
}

void SESSION::set_myserver(SERVER* p)
{
	my_server = p;
}