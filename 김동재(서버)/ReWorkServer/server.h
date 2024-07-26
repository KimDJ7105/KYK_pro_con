#pragma once
#include "session.h"
#include "game.h"
#include "timer_event.h"

class SERVER
{
private:
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	atomic_int g_game_ID;

	std::unordered_map<int, std::shared_ptr<GAME>> games;

	boost::asio::steady_timer timer_;

	int next_port;
	char server_ip[16];
private:
	void do_accept();
	
	int GetNewClientID();

public:
	std::priority_queue<TIMER_EVENT> timer_queue;

public:
	SERVER(boost::asio::io_context& io_service, int port, char _ip[16]);

	void set_ip(char ip[16]);
	char* get_ip();

	void event_excuter(const boost::system::error_code& ec);
};