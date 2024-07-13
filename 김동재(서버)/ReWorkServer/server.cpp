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
		//루프로 들어가기 이전에 필요한 이벤트 삽입
		//========================================
		/*TIMER_EVENT item_event_init;
		item_event_init.event_id = EV_SPAWN_ITEM;
		item_event_init.obj_id = 0;
		item_event_init.target_id = 0;
		item_event_init.wakeup_time = chrono::system_clock::now() + 5s;*/

		//timer_queue.push(item_event_init);

		//========================================
		//std::cout << "working\n";
		while (true) {
			TIMER_EVENT ev;
			auto current_time = chrono::system_clock::now();
			if (!timer_queue.empty()) {
				ev = timer_queue.top();
				timer_queue.pop();
				if (ev.wakeup_time > current_time) {
					timer_queue.push(ev);		// 최적화 필요
					// timer_queue에 다시 넣지 않고 처리해야 한다.
					this_thread::sleep_for(1ms);  // 실행시간이 아직 안되었으므로 잠시 대기
					continue;
				}
			}
			else {
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
			}
		}
	}


	//timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(0));
	timer_.expires_from_now(boost::asio::chrono::microseconds(100));
	timer_.async_wait(boost::bind(&SERVER::event_excuter, this, boost::asio::placeholders::error));
}