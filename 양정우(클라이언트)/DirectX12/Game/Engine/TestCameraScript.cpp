#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "BoxCollider.h"
#include "SoundPlayer.h"
#include "Animator.h"

#include "session.h"


struct Quaternion {
	float x, y, z, w;

	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	// 쿼터니언 곱셈 연산
	Quaternion operator*(const Quaternion& q) const {
		return Quaternion(
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w,
			w * q.w - x * q.x - y * q.y - z * q.z
		);
	}


	// 쿼터니언의 회전 변환
	Vec3 Rotate(const Vec3& v) const {
		Vec3 quatVec(x, y, z);
		Vec3 uv = quatVec.Cross(v);
		Vec3 uuv = quatVec.Cross(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;
		return v + uv + uuv;
	}


	Vec3 ToEulerAngles() const {
		// roll (x-axis rotation)
		float sinr_cosp = 2 * (w * x + y * z);
		float cosr_cosp = 1 - 2 * (x * x + y * y);
		float roll = atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		float sinp = 2 * (w * y - z * x);
		float pitch;
		if (fabs(sinp) >= 1)
			pitch = copysign(3.141592 / 2, sinp); // use 90 degrees if out of range
		else
			pitch = asin(sinp);

		// yaw (z-axis rotation)
		float siny_cosp = 2 * (w * z + x * y);
		float cosy_cosp = 1 - 2 * (y * y + z * z);
		float yaw = atan2(siny_cosp, cosy_cosp);

		return Vec3(roll, pitch, yaw);
	}

};

Quaternion QuaternionFromAxisAngle(const Vec3& axis, float angle) {
	float halfAngle = angle * 0.5f;
	float s = sin(halfAngle);
	return Quaternion(axis.x * s, axis.y * s, axis.z * s, cos(halfAngle));
}

extern int playerID;

TestCameraScript::TestCameraScript()
{
	// 플레이어의 수직 속도 초기화
	verticalVelocity = 0.0f;

}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{

	if (playerObject == NULL)
	{
		playerObject = GET_SINGLE(SceneManager)->GetPlayer(playerID);
	}

	if (playerGunObject == NULL)
	{
		playerGunObject = GET_SINGLE(SceneManager)->GetPlayerGun(playerID);
	}


	// 현재 위치 저장
	previousPosition = GetTransform()->GetLocalPosition();

	// 매 프레임마다 중력에 의해 수직 속도를 감소시킴

	// 땅에 닿은 상태를 판별하여 isGrounded 변수에 저장
	bool isGrounded = (GetTransform()->GetLocalPosition().y <= 0.0f);

	// 땅에 닿은 상태에서만 중력에 의한 속도 감소를 적용
	if (isGrounded)
	{
		verticalVelocity -= GRAVITY * DELTA_TIME;
	}

	// 플레이어의 현재 위치를 가져옴
	Vec3 currentPosition = GetTransform()->GetLocalPosition();

	Vec3 tempPos = currentPosition;

	// 플레이어의 이동 속도 및 방향 설정 (예시로 WASD 키를 이용한 이동)
	const float moveSpeed = 200.0f;
	Vec3 moveDirection = Vec3(0.0f, 0.0f, 0.0f);

	if (INPUT->GetButtonDown(KEY_TYPE::W))
	{
		wKeyState = true;
		std::cout << "W키 눌림" << std::endl;
		if (!isMoving) {
			isMoving = true;

			cs_packet_move_key_down mkd;
			mkd.size = sizeof(cs_packet_move_key_down);
			mkd.type = CS_MOVE_KEY_DOWN;

			main_session->Send_Packet(&mkd);
		}
	}
	else if (INPUT->GetButtonUp(KEY_TYPE::W))
	{
		wKeyState = false;
		std::cout << "W키 떼짐" << std::endl;

		if (isMoving && !wKeyState && !aKeyState && !sKeyState && !dKeyState) {
			isMoving = false;

			cs_packet_move_key_up mku;
			mku.size = sizeof(cs_packet_move_key_up);
			mku.type = CS_MOVE_KEY_UP;

			main_session->Send_Packet(&mku);
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::S))
	{
		sKeyState = true;
		if (!isMoving) {
			isMoving = true;

			cs_packet_move_key_down mkd;
			mkd.size = sizeof(cs_packet_move_key_down);
			mkd.type = CS_MOVE_KEY_DOWN;

			main_session->Send_Packet(&mkd);
		}
	}
	else if (INPUT->GetButtonUp(KEY_TYPE::S))
	{
		sKeyState = false;

		if (isMoving && !wKeyState && !aKeyState && !sKeyState && !dKeyState) {
			isMoving = false;

			cs_packet_move_key_up mku;
			mku.size = sizeof(cs_packet_move_key_up);
			mku.type = CS_MOVE_KEY_UP;

			main_session->Send_Packet(&mku);
		}

	}

	if (INPUT->GetButtonDown(KEY_TYPE::A))
	{
		aKeyState = true;
		if (!isMoving) {
			isMoving = true;

			cs_packet_move_key_down mkd;
			mkd.size = sizeof(cs_packet_move_key_down);
			mkd.type = CS_MOVE_KEY_DOWN;

			main_session->Send_Packet(&mkd);
		}
	}
	else if (INPUT->GetButtonUp(KEY_TYPE::A))
	{
		aKeyState = false;
		if (isMoving && !wKeyState && !aKeyState && !sKeyState && !dKeyState) {
			isMoving = false;

			cs_packet_move_key_up mku;
			mku.size = sizeof(cs_packet_move_key_up);
			mku.type = CS_MOVE_KEY_UP;

			main_session->Send_Packet(&mku);
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::D))
	{
		dKeyState = true;
		if (!isMoving) {
			isMoving = true;

			cs_packet_move_key_down mkd;
			mkd.size = sizeof(cs_packet_move_key_down);
			mkd.type = CS_MOVE_KEY_DOWN;

			main_session->Send_Packet(&mkd);
		}
	}
	else if (INPUT->GetButtonUp(KEY_TYPE::D))
	{
		dKeyState = false;

		if (isMoving && !wKeyState && !aKeyState && !sKeyState && !dKeyState) {
			isMoving = false;

			cs_packet_move_key_up mku;
			mku.size = sizeof(cs_packet_move_key_up);
			mku.type = CS_MOVE_KEY_UP;

			main_session->Send_Packet(&mku);
		}
	}

	if (wKeyState)
	{
		moveDirection += XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f));
		std::cout << "앞으로 이동중" << std::endl;
	}
	if (sKeyState)
	{
		moveDirection -= XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f));
	}
	if (aKeyState)
	{
		moveDirection -= GetTransform()->GetRight();
	}
	if (dKeyState)
	{
		moveDirection += GetTransform()->GetRight();
	}

	

	// 이동 방향 벡터의 길이를 1로 정규화하여 이동 속도를 일정하게 함
	if (moveDirection.LengthSquared() > 0.0f)
	{
		moveDirection.Normalize();
	}
	// 플레이어의 위치를 이동 방향과 속도에 따라 업데이트
	currentPosition += moveDirection * moveSpeed * DELTA_TIME;

	if (playerObject != NULL)
	{
		shared_ptr<GameObject> overlap = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, 99);
		if (overlap != NULL)
		{
			isOverlap = true;
			currentPosition = previousPosition; // 충돌 시 이전 위치로 되돌림
		}
		else
		{
			isOverlap = false;
		}
	}

	

	// 플레이어가 땅 밑으로 떨어지는 것을 방지하기 위해 y 좌표를 0으로 고정
	if (currentPosition.y < 0.0f)
	{
		currentPosition.y = 0.0f;
		// 땅에 닿은 상태로 간주하여 수직 속도를 0으로 초기화
		verticalVelocity = 0.0f;
	}


	//위치가 변경되었을때만 서버에 전송하도록 수정
	if (currentPosition != tempPos)
	{
		//------------------------------------
		cs_packet_pos_info packet;
		packet.size = sizeof(cs_packet_pos_info);
		packet.type = CS_POS_INFO;
		packet.x = currentPosition.x;
		packet.y = currentPosition.y;
		packet.z = currentPosition.z;

		main_session->Send_Packet(&packet);
		//-------------------------------------
	}
	else if (currentPosition == tempPos)
	{
	}


	// 업데이트된 위치를 플레이어에 반영
	GetTransform()->SetLocalPosition(currentPosition);



	
	if (INPUT->GetButton(KEY_TYPE::P))
	{
		GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 0.f));
	}


	if (isMouseMod)
	{
		RotationUpdate();
	}
	else if (!isMouseMod)
	{

	}



#ifdef DEBUG_ON
	//마우스 디버깅을 위해 P입력시 프로그램이 종료하도록 하였다.
	if (INPUT->GetButton(KEY_TYPE::C))
		PostQuitMessage(0);
#endif

	HWND foregroundWindow = GetForegroundWindow();
	wchar_t windowTitle[256] = { 0 };
	GetWindowTextW(foregroundWindow, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));

	if (previousTitle[0] == L'\0')
	{
		// previousTitle 배열이 비어 있는지 확인하고, 비어 있다면 windowTitle 내용을 복사합니다.
		wcscpy_s(previousTitle, windowTitle);
	}


	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
	{
		if (isMouseMod == false)
		{
			::ShowCursor(false);
			isMouseMod = true;
			isWindowCapture = true;
			::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
			//std::cout << "윈도우의 상태 체크모드가 발동중" << std::endl;
		}
		else if (isMouseMod == true)
		{
			::ShowCursor(true);
			isMouseMod = false;
			isWindowCapture = false;
			//std::cout << "윈도우의 상태 체크모드가 해제됨" << std::endl;
		}
	}
	
	if (isWindowCapture == true)
	{
		//창의 변화가 일어났다면.
		if (wcscmp(windowTitle, previousTitle) != 0)
		{

			//std::cout << "창이 변경되었음" << std::endl;

			int len = WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, NULL, 0, NULL, NULL);
			char* buffer = new char[len];
			WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, buffer, len, NULL, NULL);

			const char* compareString = "PROJECT_ReWork";

			// strcmp 함수를 사용하여 buffer와 비교하고자 하는 문자열을 비교
			if (strstr(buffer, compareString) != nullptr)
			{

				//std::cout << "현재 화면은 게임화면입니다" << std::endl;

				//현재 활성화된 화면이 게임화면이라면
				isMouseMod = true;
				::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
			}
			else {

				//std::cout << "현재 화면은 게임화면이 아닙니다." << std::endl;

				//현재 활성화된 화면이 게임이 아니라면
				isMouseMod = false;
			}

			delete[] buffer;
		}
		else
		{
			//std::cout << "창이 변경되지 않음" << std::endl;
		}
	}
	else if(isWindowCapture == false)
	{
		
	}



	//Picking 입력을 확인
	if (INPUT->GetButton(KEY_TYPE::RBUTTON))
	{
#ifdef DEBUG_ON
		std::cout << "COOLTIME" << std::endl;
#endif
		if (clickCooldown <= timeElapse)
		{
#ifdef DEBUG_ON
			std::cout << "FIRE" << std::endl;
#endif
			const POINT& pos = INPUT->GetMousePos();

			shared_ptr<GameObject> pickedObject;

			pickedObject = GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
			if (pickedObject != NULL)
			{
				int a = pickedObject->GetTransform()->GetObjectType();

				//여기서 타입이 플레이어일때만
				//즉 OT_PLAYER일때만 정보를 전달하도록 한다.
				if (pickedObject->GetTransform()->GetObjectType() == OT_PLAYER)
				{
					cs_packet_picking_info ppi;
					ppi.size = sizeof(cs_packet_picking_info);
					ppi.type = CS_PICKING_INFO;
					ppi.target_id = pickedObject->GetTransform()->GetObjectID();

					main_session->Send_Packet(&ppi);
				}

				else {
					cs_packet_picking_info ppi;
					ppi.size = sizeof(cs_packet_picking_info);
					ppi.type = CS_PICKING_INFO;
					ppi.target_id = -1;

					main_session->Send_Packet(&ppi);
				}
			}

			else {
				cs_packet_picking_info ppi;
				ppi.size = sizeof(cs_packet_picking_info);
				ppi.type = CS_PICKING_INFO;
				ppi.target_id = -1;

				main_session->Send_Packet(&ppi);
			}

			timeElapse = 0.f;
		}
	}

#ifdef DEBUG_ON
	/*if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();

		shared_ptr<GameObject> pickedObject;

		pickedObject = GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);

		if (pickedObject == pickedMovingObject)
			pickedMovingObject = NULL;
		else
			pickedMovingObject = pickedObject;
	}*/
#endif

	if (pickedMovingObject != NULL)
		RotatingPickedObject();


	if (INPUT->GetButtonDown(KEY_TYPE::E))
	{

		

		//플레이어 ID를 탐색
		shared_ptr<GameObject> playerObject = GET_SINGLE(SceneManager)->GetPlayer(playerID);
		shared_ptr<GameObject> keyCard;

		//playerObject와 OT_KEYCARD타입간의 충돌이 일어났는지를 확인
		keyCard = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_KEYCARD);

		if (keyCard != NULL)
		{
			Vec3 pos = playerObject->GetTransform()->GetLocalPosition();

			cs_packet_try_get_key tgk;
			tgk.size = sizeof(cs_packet_try_get_key);
			tgk.type = CS_TRY_GET_KEY;
			tgk.x = pos.x;
			tgk.y = pos.y;
			tgk.z = pos.z;
			tgk.key_id = keyCard->GetTransform()->GetObjectID();
			//pos xyz 는 플레이어의 현재 위치
			//key_id는 충돌한 카드키의 id값
			main_session->Send_Packet(&tgk);

			GET_SINGLE(SceneManager)->SetKeyCardPosition(150 + 75 * haveKeycard, (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)) - 130, haveKeycard);
			haveKeycard++;
		}

		shared_ptr<GameObject> terminal = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_TERMINAL);

		if (terminal != NULL)
		{
			cs_packet_try_use_tmn tut;
			tut.size = sizeof(cs_packet_try_use_tmn);
			tut.type = CS_TRY_USE_TMN;
			tut.terminal_id = terminal->GetTransform()->GetObjectID();

			main_session->Send_Packet(&tut);
		}

		shared_ptr<GameObject> rabbitfoot = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_RABBITFOOT);

		if (rabbitfoot != NULL)
		{
			Vec3 pos = playerObject->GetTransform()->GetLocalPosition();

			cs_packet_try_get_rabbitfoot tgr;
			tgr.size = sizeof(cs_packet_try_get_rabbitfoot);
			tgr.type = CS_TRY_GET_RABBITFOOT;
			tgr.x = pos.x;
			tgr.y = pos.y;
			tgr.z = pos.z;
			tgr.obj_id = rabbitfoot->GetTransform()->GetObjectID();
			
			main_session->Send_Packet(&tgr);

			//2024-07-06
			//토끼발을 획득 시도(방식은 카드키와 동일)
			//토끼발을 획득하면 UI적으로 표시 필요
		}
	}
	
	if (INPUT->GetButtonDown(KEY_TYPE::R))
	{
		/*auto& obj = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
		for (auto& a : obj)
		{
			if (a->GetTransform()->GetObjectType() != OT_PLAYER)
				continue;
			a->GetAnimator()->Play(1);
		}*/

		cs_packet_reload_mag lm;
		lm.size = sizeof(cs_packet_reload_mag);
		lm.type = CS_RELOAD_MAG;

		main_session->Send_Packet(&lm);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::T))
	{
		std::cout << "test go on\n";
		test_packet tp;
		tp.size = sizeof(test_packet);
		tp.type = TEST_SPAWN_RBF;

		main_session->Send_Packet(&tp);
	}
	
	{

		Vec3 rotation = GetTransform()->GetLocalRotation();

		Vec3 gunOffset(5.0f, -5.0f, 15.0f); // 아래로 2, 오른쪽으로 2, z축은 이전과 동일하게 유지

		// 플레이어의 회전값을 쿼터니언으로 변환
		Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
			QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

		// gunOffset을 회전시킴
		Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

		// playerGunObject의 위치를 플레이어의 위치로 이동
		if(playerGunObject != NULL) playerGunObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

		// 총의 회전 오프셋을 적용하여 쿼터니언 생성
		Vec3 gunRotationOffset(-1.57f, 3.14f, 0.0f); // 총의 회전 오프셋
		Quaternion gunRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), gunRotationOffset.y) *
			QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), gunRotationOffset.x);

		// 플레이어의 회전값에 총의 회전을 추가하여 총의 최종 회전 쿼터니언 생성
		Quaternion finalGunRotationQuat = playerRotationQuat * gunRotationQuat;

		Vec3 gunRotation = finalGunRotationQuat.ToEulerAngles();

		// 회전을 적용
		if (playerGunObject != NULL) playerGunObject->GetTransform()->SetLocalRotation(gunRotation);

		// 플레이어를 기준으로 한 반대 방향으로 이동
		Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
		if (playerGunObject != NULL) playerGunObject->GetTransform()->SetLocalPosition(newPosition);
	}

	{
		Vec3 rotation = GetTransform()->GetLocalRotation();

		Vec3 gunOffset(0.0f, -40.0f, 10.0f); // 아래로 2, 오른쪽으로 2, z축은 이전과 동일하게 유지

		// 플레이어의 회전값을 쿼터니언으로 변환
		Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
			QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

		// gunOffset을 회전시킴
		Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

		// playerGunObject의 위치를 플레이어의 위치로 이동
		if (playerGunObject != NULL) playerObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

		// 총의 회전 오프셋을 적용하여 쿼터니언 생성
		Vec3 gunRotationOffset(0.f, 3.14f, 0.0f); // 총의 회전 오프셋
		Quaternion gunRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), gunRotationOffset.y) *
			QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), gunRotationOffset.x);

		// 플레이어의 회전값에 총의 회전을 추가하여 총의 최종 회전 쿼터니언 생성
		Quaternion finalGunRotationQuat = playerRotationQuat * gunRotationQuat;

		Vec3 gunRotation = finalGunRotationQuat.ToEulerAngles();

		// 회전을 적용
		if (playerObject != NULL) playerObject->GetTransform()->SetLocalRotation(gunRotation);

		// 플레이어를 기준으로 한 반대 방향으로 이동
		Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
		if (playerObject != NULL) playerObject->GetTransform()->SetLocalPosition(newPosition);

		if (playerObject != NULL) Vec3 hispos = playerObject->GetTransform()->GetLocalPosition();
	}
	

	wcscpy_s(previousTitle, windowTitle);

	timeElapse += DELTA_TIME;


}



void TestCameraScript::MoveUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	Vec3 tempPos = pos;

	if (INPUT->GetButton(KEY_TYPE::W))
	{
		//pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

		//std::cout << "W 입력 처리중" << std::endl;

		// Right벡터와 수직벡터의 외적값 -> 정면
		pos += XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::S))
	{
		//pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		//std::cout << "S 입력 처리중" << std::endl;

		pos -= XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::A))
	{
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		//std::cout << "A 입력 처리중" << std::endl;

		// Look벡터와 수직벡터의 외적값 -> 오른쪽
		//pos -= XMVector3Cross(GetTransform()->GetLook(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::D))
	{
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		//std::cout << "D 입력 처리중" << std::endl;

		//pos += XMVector3Cross(GetTransform()->GetLook(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	pos.y = tempPos.y;

	//위치가 변경되었을때만 서버에 전송하도록 수정
	if (pos != tempPos)
	{
		//------------------------------------
		cs_packet_pos_info packet;
		packet.size = sizeof(cs_packet_pos_info);
		packet.type = CS_POS_INFO;
		packet.x = pos.x;
		packet.y = pos.y;
		packet.z = pos.z;

		main_session->Send_Packet(&packet);
		//-------------------------------------
	}

	GetTransform()->SetLocalPosition(pos);
}


void TestCameraScript::RotationUpdate()
{
	//먼저 현재 마우스의 포지션값을 읽어와서 저장한다.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	Vec3 rotation = GetTransform()->GetLocalRotation();

	//만약 마우스가 움직임이 발생했다면
	if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	{
		//움직인 값을 저장한다.
		int moveX = nowMousePos.x - WINDOW_MIDDLE_X;
		int moveY = nowMousePos.y - WINDOW_MIDDLE_Y;

		//오른쪽
		if (moveX > 0)
		{
			rotation.y += DELTA_TIME * moveX;

			GetTransform()->SetLocalRotation(rotation);
		}

		// 왼쪽
		if (moveX < 0)
		{
			rotation.y += DELTA_TIME * moveX;

			GetTransform()->SetLocalRotation(rotation);
		}

		// 아래
		if (moveY > 0)
		{
			rotation.x += DELTA_TIME * moveY;

			//playerRotation.x -= DELTA_TIME * moveY;

			GetTransform()->SetLocalRotation(rotation);
		}

		//위
		if (moveY < 0)
		{
			rotation.x += DELTA_TIME * moveY;

			//playerRotation.x -= DELTA_TIME * moveY;

			GetTransform()->SetLocalRotation(rotation);
		}

		
		//---------------------------------
		// 이곳에서 rotation정보를 server에 넘겨주면 된다.
		cs_packet_mouse_info mi;
		mi.size = sizeof(cs_packet_mouse_info);
		mi.type = CS_MOUSE_INFO;
		mi.x = rotation.x;
		mi.y = rotation.y;
		mi.z = 0.0f;

		main_session->Send_Packet(&mi);
		//---------------------------------
	}


	//마우스의 위치를 중앙으로 초기화 해준다.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}

void TestCameraScript::RotatingPickedObject()
{
	Vec3 pos = pickedMovingObject->GetTransform()->GetLocalPosition();
	Vec3 rotation = pickedMovingObject->GetTransform()->GetLocalRotation();
	Vec3 scale = pickedMovingObject->GetTransform()->GetLocalScale();

	
	if (INPUT->GetButton(KEY_TYPE::UP))
	{
		pos.z += _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::DOWN))
	{
		pos.z -= _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::LEFT))
	{
		pos.x -= _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::RIGHT))
	{
		pos.x += _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numUP))
	{
		rotation.x += _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numDOWN))
	{
		rotation.x -= _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numLEFT))
	{
		rotation.y += _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numRIGHT))
	{
		rotation.y -= _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numONE))
	{
		scale.x += _objspeed * DELTA_TIME;
		scale.y += _objspeed * DELTA_TIME;
		scale.z += _objspeed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::numTHREE))
	{
		scale.x -= _objspeed * DELTA_TIME;
		scale.y -= _objspeed * DELTA_TIME;
		scale.z -= _objspeed * DELTA_TIME;
	}


	pickedMovingObject->GetTransform()->SetLocalPosition(pos);
	pickedMovingObject->GetTransform()->SetLocalRotation(rotation);
	pickedMovingObject->GetTransform()->SetLocalScale(scale);

#ifdef DEBUG_ON
	std::cout << "Object Pos : (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	std::cout << "Object Rotation : (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")" << std::endl;
	std::cout << "Object Scale : (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
#endif
}
