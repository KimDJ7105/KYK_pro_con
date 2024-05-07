#pragma once
#include "server.h"

concurrency::concurrent_unordered_map<int, shared_ptr<SESSION>> players;
concurrency::concurrent_unordered_map<int,shared_ptr<OBJECT>> Cardkeys;

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


SERVER::SERVER(boost::asio::io_context& io_service, int port)
	: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	socket_(io_service)
{
	std::cout << "초기 카드키 생성중" << std::endl;
	Cardkeys[0] = std::make_shared<OBJECT>(OT_KEYCARD);
	Cardkeys[1] = std::make_shared<OBJECT>(OT_KEYCARD);
	Cardkeys[2] = std::make_shared<OBJECT>(OT_KEYCARD);

	Cardkeys[0]->select_pos();
	Cardkeys[1]->select_pos();
	Cardkeys[2]->select_pos();
	std::cout << "카드키 생성 완료" << std::endl;

	Cardkeys[0]->show_approx_pos();
	Cardkeys[1]->show_approx_pos();
	Cardkeys[2]->show_approx_pos();

	do_accept();
}
