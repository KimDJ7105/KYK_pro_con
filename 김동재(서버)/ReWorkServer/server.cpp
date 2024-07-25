#pragma once
#include "server.h"

std::shared_ptr<SESSION> lobby;

atomic_int g_user_ID;

void SERVER::do_accept()
{
	acceptor_.async_accept(socket_,
		[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				int p_id = GetNewClientID();
				
				//if (p_id == LOBBY_ID) {
				if(p_id == LOBBY_ID) {
					std::cout << "Lobby server connected\n";
					lobby = std::make_shared<SESSION>(std::move(socket_), p_id, -1);	
					lobby->start();
				}
				else {
					std::cout << "Client " << p_id << " loged in\n";
					auto it = games.find(g_game_ID);
					if (it == games.end())
					{
						games[g_game_ID] = std::make_shared<GAME>(g_game_ID);
					}

					games[g_game_ID]->ingame_player[p_id] = std::make_shared<SESSION>(std::move(socket_), p_id, games[g_game_ID]->get_team_num());
					games[g_game_ID]->ingame_player[p_id]->set_mygame(games[g_game_ID]);
					games[g_game_ID]->ingame_player[p_id]->set_myserver(this);
					games[g_game_ID]->ingame_player[p_id]->start();

					if (games[g_game_ID]->ingame_player.size() > MAX_USER + 1) g_game_ID++;
					if (games.size() == MAX_GAME) {
						sl_packet_set_port set_port;
						set_port.size = sizeof(sl_packet_set_port);
						set_port.type = SL_SET_PORT;
						strcpy_s(set_port.port, std::to_string(next_port).c_str());

						lobby->Send_Packet(&set_port);
					}
				}
				do_accept();
			}

			else {
				std::cout << "Accept 실패\n";
			}
		});
}

int SERVER::GetNewClientID()
{
	return g_user_ID++;
}

SERVER::SERVER(boost::asio::io_context& io_service, int port)
	: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	socket_(io_service),
	timer_(io_service, boost::asio::chrono::microseconds(100))
{	
	if (port == MY_PORT3) next_port = MY_PORT0;
	else next_port = port + 1;

	timer_.async_wait(boost::bind(&SERVER::event_excuter, this, boost::asio::placeholders::error));

	do_accept();
}

void SERVER::event_excuter(const boost::system::error_code& ec)
{
	if (!ec) {
		//std::cout << "working\n";
		if (!timer_queue.empty()) {
			TIMER_EVENT ev;
			auto current_time = chrono::system_clock::now();
			
			ev = timer_queue.top();
			timer_queue.pop();
			if (ev.wakeup_time > current_time) {
				timer_queue.push(ev);

				timer_.expires_from_now(boost::asio::chrono::microseconds(100));
				timer_.async_wait(boost::bind(&SERVER::event_excuter, this, boost::asio::placeholders::error));
				return;
			}

			switch (ev.event_id) {
			case EV_LASER_TRAP_ON: {
				//target_id는 방 번호
				std::cout << "Laser on Evnet Called\n";
				auto& laser = games[ev.game_id]->CreateObject(OT_LASER, ev.x, ev.y, ev.z, -1.57f, 1.57f, 0.f, WAY_RIGHT);
				laser->end_pos = ev.x + 300.f;

				sc_packet_put_object_coor poc;
				poc.size = sizeof(sc_packet_put_object_coor);
				poc.type = SC_PUT_OBJECT_COOR;
				poc.obj_id = laser->obj_id;
				poc.obj_type = OT_LASER;
				poc.x = ev.x;
				poc.y = ev.y;
				poc.z = ev.z;
				poc.dirx = -1.57f;
				poc.diry = 1.57f;
				poc.dirz = 0.f;

				for (auto& player : games[ev.game_id]->ingame_player) {
					player.second->Send_Packet(&poc);
				}

				if (!games[ev.game_id]->is_laser_on) {
					games[ev.game_id]->is_laser_on = true;
					TIMER_EVENT tm_move_laser;

					tm_move_laser.game_id = ev.game_id;
					tm_move_laser.event_id = EV_MOVE_LASER_TRAP;
					tm_move_laser.target_id = -1;
					tm_move_laser.wakeup_time = chrono::system_clock::now() + 50ms;
					
					timer_queue.emplace(tm_move_laser);
				}
				break;
			}
			case EV_MOVE_LASER_TRAP: {
				//std::cout << "Laser moving\n";
				for (auto& obj : games[ev.game_id]->ingame_object) {
					auto& laser = obj.second;
					if (laser->obj_type != OT_LASER) continue;

					if (laser->way == WAY_RIGHT) {
						laser->pos[0] += 10.0f;
						if (laser->pos[0] >= laser->end_pos) {
							laser->way = WAY_LEFT;
							laser->end_pos -= 300.0f;
						}
					}

					else if (laser->way == WAY_LEFT) {
						laser->pos[0] -= 10.0f;
						if (laser->pos[0] <= laser->end_pos) {
							laser->way = WAY_RIGHT;
							laser->end_pos += 300.0f;
						}
					}

					sc_packet_pos laser_pos;
					laser_pos.size = sizeof(laser_pos);
					laser_pos.type = SC_POS;
					laser_pos.id = laser->obj_id;
					laser_pos.x = laser->pos[0];
					laser_pos.y = laser->pos[1];
					laser_pos.z = laser->pos[2];
					laser_pos.dirx = laser->rot[0];
					laser_pos.diry = laser->rot[1];
					laser_pos.dirz = laser->rot[2];
					laser_pos.animation_id = -1;

					for (auto& player : games[ev.game_id]->ingame_player) {
						player.second->Send_Packet(&laser_pos);
					}
				}

				TIMER_EVENT tm_move_laser;

				tm_move_laser.game_id = ev.game_id;
				tm_move_laser.event_id = EV_MOVE_LASER_TRAP;
				tm_move_laser.target_id = -1;
				tm_move_laser.wakeup_time = chrono::system_clock::now() + 30ms;

				timer_queue.emplace(tm_move_laser);
				break;
			}
			case EV_MOVE_GRINDER: {
				for (auto& object : games[ev.game_id]->ingame_object) {
					auto& obj = object.second;
					if (obj->obj_type != OT_GRINDER) continue;

					switch (obj->way) {
					case WAY_UP :
						obj->pos[2] += 10.0f;
						if (obj->pos[2] >= 2400.0f) {
							obj->way = WAY_DOWN;
							obj->rot[1] = 3.14f;
						}
						break;
					case WAY_DOWN:
						obj->pos[2] -= 10.0f;
						if (obj->pos[2] <= 0.0f) {
							obj->way = WAY_UP;
							obj->rot[1] = 0.0f;
						}
						break;
					case WAY_LEFT:
						obj->pos[0] -= 10.0f;
						if (obj->pos[0] <= 0.0f) {
							obj->way = WAY_RIGHT;
							obj->rot[1] = 1.57f;
						}
						break;
					case WAY_RIGHT:
						obj->pos[0] += 10.0f;
						if (obj->pos[0] >= 2400.0f) {
							obj->way = WAY_LEFT;
							obj->rot[1] = -1.57f;
						}
						break;
					}
					
					sc_packet_pos grinder_pos;
					grinder_pos.size = sizeof(sc_packet_pos);
					grinder_pos.type = SC_POS;
					grinder_pos.id = obj->obj_id;
					grinder_pos.x = obj->pos[0];
					grinder_pos.y = obj->pos[1];
					grinder_pos.z = obj->pos[2];
					grinder_pos.dirx = obj->rot[0];
					grinder_pos.diry = obj->rot[1];
					grinder_pos.dirz = obj->rot[2];
					grinder_pos.animation_id = -1;

					for (auto& players : games[ev.game_id]->ingame_player) {
						players.second->Send_Packet(&grinder_pos);
					}
				}

				TIMER_EVENT tm_grind;
				tm_grind.event_id = EV_MOVE_GRINDER;
				tm_grind.game_id = ev.game_id;
				tm_grind.target_id = -1;
				tm_grind.wakeup_time = chrono::system_clock::now() + 50ms;

				timer_queue.emplace(tm_grind);

				break;
			}
			case EV_SPAWN_EXIT: {
				auto& exit = games[ev.game_id]->CreateObjectApprox(OT_EXIT);

				sc_packet_put_object_pos pop;
				pop.type = SC_PUT_OBJECT_POS;
				pop.size = sizeof(sc_packet_put_object_pos);
				pop.obj_type = exit->obj_type;
				pop.id = exit->obj_id;
				pop.approx_num = exit->spawn_num;

				for (auto& player : games[ev.game_id]->ingame_player) {
					player.second->Send_Packet(&pop);
				}

				std::cout << "Exit Spawned\n";
				break;
			}
			}
		}
	}

	//timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(0));
	timer_.expires_from_now(boost::asio::chrono::microseconds(100));
	timer_.async_wait(boost::bind(&SERVER::event_excuter, this, boost::asio::placeholders::error));
}