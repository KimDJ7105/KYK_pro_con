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
	boost::asio::io_context io_service;
	vector <thread > worker_threads;
	SERVER s(io_service, MY_PORT);
	//server 객체를 port 번호 별로 따로 생성한 뒤에 각자의 쓰레드를 생성
	//server 클래스 내부에 game 객체를 관리하기 위한 컨테이너 생성 필요

	
	Init_Server();

	for (auto i = 0; i < 1; i++) worker_threads.emplace_back(worker_thread, &io_service);
	for (auto& th : worker_threads) th.join();
}

