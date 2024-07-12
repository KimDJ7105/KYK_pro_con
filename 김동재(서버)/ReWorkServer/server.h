#pragma once
#include "session.h"
#include "game.h"
#include "timer_event.h"

class SERVER
{
private:
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	atomic_int g_user_ID;
	atomic_int g_game_ID;

	std::unordered_map<int, std::shared_ptr<GAME>> games;

private:
	void do_accept();
	
	int GetNewClientID();

public:
	std::priority_queue<TIMER_EVENT> timer_queue;

public:
	SERVER(boost::asio::io_context& io_service, int port);

	void event_excuter(const boost::system::error_code& ec, boost::asio::steady_timer* timer);
};