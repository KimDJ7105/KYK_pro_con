#include "pch.h"
#include "LobbyCameraScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"

#include "session.h"

SESSION* main_session;
std::thread* serverthread_p;

void worker_SM_thread(boost::asio::io_context* io_con)
{
	io_con->run();
}

LobbyCameraScript::LobbyCameraScript()
{
}

LobbyCameraScript::~LobbyCameraScript()
{
}


void LobbyCameraScript::LateUpdate()
{
	if (INPUT->GetButtonDown(KEY_TYPE::SPACEBAR))
	{
		GET_SINGLE(SceneManager)->LoadMainScene(L"TestScene");


		//���Į
		// �Ʒ����� Main Scene�� ������ Main Server�� ���� �ڵ�
		char ip[16];
		char port[6];

		io_context io_con;
		tcp::resolver resolver(io_con);
		auto endpoint = resolver.resolve(ip,port);

		tcp::socket sock(io_con);

		main_session = new SESSION(std::move(sock));

		main_session->do_connect(endpoint);

		serverthread_p = new std::thread(worker_SM_thread, &io_con);
		//join�� �ϴµ� ���� �߻� ��
		// 1.�̰� �ϸ� LObbyCameraScript ��ü�� �Ҹ�ǳ�?
		// 2. �����Ǵ� �ΰ��� Ŭ���� ��, testCameraScript ��ü�� ���� �����ǰ� ���� �Ҹ��ϴ°�?
	}
}