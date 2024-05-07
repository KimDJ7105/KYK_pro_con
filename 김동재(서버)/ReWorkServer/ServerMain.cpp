#pragma once
#include "stdafx.h"

#include "server.h"
#include "object.h"

const auto X_START_POS = 4;
const auto Y_START_POS = 4;

array<shared_ptr<OBJECT>, 3> Cardkeys;

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
	std::cout << "초기 카드키 생성중" << std::endl;
	Cardkeys[0] = std::make_shared<OBJECT>(OT_KEYCARD);
	Cardkeys[1] = std::make_shared<OBJECT>(OT_KEYCARD);
	Cardkeys[2] = std::make_shared<OBJECT>(OT_KEYCARD);
	
	Cardkeys[0]->select_pos();
	Cardkeys[1]->select_pos();
	Cardkeys[2]->select_pos();
	std::cout << "카드키 생성 완료" << std::endl;

	/*Cardkeys[0]->show_approx_pos();
	Cardkeys[1]->show_approx_pos();
	Cardkeys[2]->show_approx_pos();*/

	boost::asio::io_context io_service;
	vector <thread > worker_threads;
	SERVER s(io_service, MY_PORT);

	Init_Server();

	for (auto i = 0; i < 4; i++) worker_threads.emplace_back(worker_thread, &io_service);
	for (auto& th : worker_threads) th.join();
}

