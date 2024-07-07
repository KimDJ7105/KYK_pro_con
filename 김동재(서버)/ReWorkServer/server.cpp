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
					lobby = std::make_shared<SESSION>(std::move(socket_), p_id);	
					lobby->start();
				}
				else {
					std::cout << "Client " << p_id << " loged in\n";
					auto it = games.find(g_game_ID);
					if (it == games.end())
					{
						games[g_game_ID] = std::make_shared<GAME>(g_game_ID);
					}

					games[g_game_ID]->ingame_player[p_id] = std::make_shared<SESSION>(std::move(socket_), p_id);
					games[g_game_ID]->ingame_player[p_id]->set_mygame(games[g_game_ID]);
					games[g_game_ID]->ingame_player[p_id]->start();

					if (games[g_game_ID]->ingame_player.size() > MAX_USER) g_game_ID++;
				}
				do_accept();
			}

			else {
				std::cout << "Accept ½ÇÆÐ\n";
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
