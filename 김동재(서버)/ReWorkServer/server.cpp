#pragma once
#include "server.h"

std::shared_ptr<SESSION> lobby;

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
					games[g_game_ID]->ingame_player[p_id]->start();
					games[g_game_ID]->ingame_player[p_id]->set_myserver(this);

					if (games[g_game_ID]->ingame_player.size() > MAX_USER) g_game_ID++;
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
			case EV_LASER_TRAP: {
				//target_id는 방 번호
				break;
			}
			case EV_MOVE_GRINDER: {
				//target_id는 방향 (정방향 / 역방향)
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