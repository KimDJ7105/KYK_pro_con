#pragma once
#include "session.h"

class SERVER
{
private:
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	atomic_int g_user_ID;

private:
	void do_accept();
	
	int GetNewClientID();

public:
	SERVER(boost::asio::io_context& io_service, int port);
};