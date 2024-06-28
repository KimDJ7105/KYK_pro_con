#pragma once
#include "session.h"
#include "game.h"

class SERVER
{
private:
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	atomic_int g_user_ID;
	atomic_int g_object_ID = MAX_USER + 1;

	std::unordered_map<int, std::unique_ptr<GAME>> games;

private:
	void do_accept();
	
	int GetNewClientID();
	int GetNewObjectID();

public:
	SERVER(boost::asio::io_context& io_service, int port);
};