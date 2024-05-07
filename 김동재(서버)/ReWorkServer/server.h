#pragma once
#include "session.h"
#include "object.h"

class SERVER
{
private:
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	atomic_int g_user_ID;
	atomic_int g_object_ID = MAX_USER + 1;

private:
	void do_accept();
	
	int GetNewClientID();
	int GetNewObjectID();

public:
	SERVER(boost::asio::io_context& io_service, int port);
};