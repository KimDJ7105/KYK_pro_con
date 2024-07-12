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
				std::cout << "Accept ����\n";
			}
		});
}

int SERVER::GetNewClientID()
{
	return g_user_ID++;
}

SERVER::SERVER(boost::asio::io_context& io_service, int port)
	: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	socket_(io_service)
{	
	do_accept();
}

void SERVER::event_excuter(const boost::system::error_code& ec, boost::asio::steady_timer* timer)
{
	if (!ec) {
		//������ ���� ������ �ʿ��� �̺�Ʈ ����
		//========================================
		/*TIMER_EVENT item_event_init;
		item_event_init.event_id = EV_SPAWN_ITEM;
		item_event_init.obj_id = 0;
		item_event_init.target_id = 0;
		item_event_init.wakeup_time = chrono::system_clock::now() + 5s;*/

		//timer_queue.push(item_event_init);

		//========================================
		while (true) {
			TIMER_EVENT ev;
			auto current_time = chrono::system_clock::now();
			if (true != timer_queue.empty()) {
				ev = timer_queue.top();
				timer_queue.pop();
				if (ev.wakeup_time > current_time) {
					timer_queue.push(ev);		// ����ȭ �ʿ�
					// timer_queue�� �ٽ� ���� �ʰ� ó���ؾ� �Ѵ�.
					this_thread::sleep_for(1ms);  // ����ð��� ���� �ȵǾ����Ƿ� ��� ���
					continue;
				}
			}
			switch (ev.event_id) {
			case EV_LASER_TRAP: {
				//target_id�� �� ��ȣ
				break;
			}
			case EV_MOVE_GRINDER: {
				//target_id�� ���� (������ / ������)
				break;
			}
			}
		}
	}


	timer->expires_at(timer->expiry() + boost::asio::chrono::seconds(0));
	timer->async_wait(boost::bind(event_excuter, boost::asio::placeholders::error, timer));
}