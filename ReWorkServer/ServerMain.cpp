﻿#pragma once
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
	char input_ip[16]{"127.0.0.1"};

	std::cout << "서버의 ip 입력 : ";
	std::cin >> input_ip;

	std::array<boost::asio::io_context, 4> io_service;
	vector <thread > worker_threads;
	vector <std::unique_ptr<SERVER>> servers;

	for (int i = 0; i < 4; i++)
	{
		servers.emplace_back(std::make_unique<SERVER>(io_service[i], MY_PORT0 + i, input_ip));
	}

	Init_Server();

	for (auto i = 0; i < 4; i++) worker_threads.emplace_back(worker_thread, &io_service[i]);
	for (auto& th : worker_threads) th.join();
	for (auto i = 0; i < 4; i++) io_service[i].stop();

	return 0;
}