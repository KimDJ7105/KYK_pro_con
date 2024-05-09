#pragma once
#include "server.h"

concurrency::concurrent_unordered_map<int, shared_ptr<SESSION>> players;
concurrency::concurrent_unordered_map<int, shared_ptr<OBJECT>> objects;

void SERVER::do_accept()
{
	acceptor_.async_accept(socket_,
		[this](boost::system::error_code ec)
		{
			if (!ec)
			{
				int p_id = GetNewClientID();
				std::cout << "Client " << p_id << " loged in\n";
				players[p_id] = std::make_shared<SESSION>(std::move(socket_), p_id);
				players[p_id]->start();
				do_accept();
			}
		});
}

int SERVER::GetNewClientID()
{
	if (g_user_ID >= MAX_USER) {
		cout << "MAX USER FULL\n";
		exit(-1);
	}
	return g_user_ID++;
}

int  SERVER::GetNewObjectID()
{
	return g_object_ID++;
}

SERVER::SERVER(boost::asio::io_context& io_service, int port)
	: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	socket_(io_service)
{
	std::cout << "초기 카드키 생성중" << std::endl;

	for (int i = 0; i < 3; i++) {
		int o_id = GetNewObjectID();
		objects[o_id] = std::make_shared<OBJECT>(o_id, OT_KEYCARD);
		objects[o_id]->select_pos();
		objects[o_id]->show_approx_pos();

		int t_id = GetNewObjectID();
		objects[t_id] = std::make_shared<OBJECT>(t_id, OT_TERMINAL);
		objects[t_id]->select_pos();
		objects[t_id]->show_approx_pos();
	}
	
	std::cout << "카드키 생성 완료" << std::endl;

	do_accept();
}
