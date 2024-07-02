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


		//당근칼
		// 아래에는 Main Scene과 연결할 Main Server에 대한 코드
		char ip[16];
		char port[6];

		io_context io_con;
		tcp::resolver resolver(io_con);
		auto endpoint = resolver.resolve(ip,port);

		tcp::socket sock(io_con);

		main_session = new SESSION(std::move(sock));

		main_session->do_connect(endpoint);

		serverthread_p = new std::thread(worker_SM_thread, &io_con);
		//join을 하는데 문제 발생 중
		// 1.이걸 하면 LObbyCameraScript 객체가 소멸되나?
		// 2. 생성되는 인게임 클래스 즉, testCameraScript 객체는 언제 생성되고 언제 소멸하는가?
	}
}