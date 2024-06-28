#include "stdafx.h"
#include "server.h"
#include "LS_session.h"

void Init_Server()
{
	_wsetlocale(LC_ALL, L"korean");
}


void worker_thread(boost::asio::io_context* service)
{
	service->run();
}

void server_connect_thread(boost::asio::io_context* io_con)
{
	io_con->run();
}

LS_SESSION* lobby_server_session;

int main()
{
	//메인 서버에 접속
	//-------------------------------------------------------------
	boost::asio::io_context io_con;
	tcp::resolver resolver(io_con);
	auto endpoint = resolver.resolve(MY_SERVER_IP, MY_SERVER_PORT);

	tcp::socket sock(io_con);

	lobby_server_session = new LS_SESSION(std::move(sock));
	 
	lobby_server_session->do_connect(endpoint);
	std::thread serverthread(server_connect_thread, &io_con);

	std::cout << "메인 서버 접속\n";
	//-------------------------------------------------------------

	//클라이언트의 접속을 accept
	//-------------------------------------------------------------
	boost::asio::io_context io_service;
	vector <thread > worker_threads;
	SERVER s(io_service, LOBBY_PORT);

	Init_Server();
	//-------------------------------------------------------------
	for (auto i = 0; i < 4; i++) worker_threads.emplace_back(worker_thread, &io_service);
	for (auto& th : worker_threads) th.join();

	serverthread.join();
}
