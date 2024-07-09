#pragma once
#include "stdafx.h"

#include "server.h"
#include "object.h"

const auto X_START_POS = 4;
const auto Y_START_POS = 4;

void Init_Server()
{
	_wsetlocale(LC_ALL, L"korean");
}


void worker_thread(boost::asio::io_context* service)
{
	service->run();
}

int main()
{
	std::array<boost::asio::io_context, 4> io_service;
	vector <thread > worker_threads;
	vector <std::unique_ptr<SERVER>> servers;

	servers.emplace_back(std::make_unique<SERVER>(io_service[0], MY_PORT0));
	servers.emplace_back(std::make_unique<SERVER>(io_service[1], MY_PORT1));
	servers.emplace_back(std::make_unique<SERVER>(io_service[2], MY_PORT2));
	servers.emplace_back(std::make_unique<SERVER>(io_service[3], MY_PORT3));


	Init_Server();

	for (auto i = 0; i < 4; i++) worker_threads.emplace_back(worker_thread, &io_service[i]);
	for (auto& th : worker_threads) th.join();
	for (auto i = 0; i < 4; i++) io_service[i].stop();

	return 0;
}

