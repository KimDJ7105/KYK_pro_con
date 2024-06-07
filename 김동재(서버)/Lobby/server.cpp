#include "server.h"


concurrency::concurrent_unordered_map<int, shared_ptr<SESSION>> players;

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
	return g_user_ID++;
}

SERVER::SERVER(boost::asio::io_context& io_service, int port)
	: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	socket_(io_service)
{
	do_accept();
}
