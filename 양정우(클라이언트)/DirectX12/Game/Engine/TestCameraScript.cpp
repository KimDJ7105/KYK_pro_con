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

#include "session.h"



struct Quaternion {
	float x, y, z, w;

	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	// ���ʹϾ� ���� ����
	Quaternion operator*(const Quaternion& q) const {
		return Quaternion(
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w,
			w * q.w - x * q.x - y * q.y - z * q.z
		);
	}


	// ���ʹϾ��� ȸ�� ��ȯ
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
	// �÷��̾��� ���� �ӵ� �ʱ�ȭ
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


	// ���� ��ġ ����
	previousPosition = GetTransform()->GetLocalPosition();

	// �� �����Ӹ��� �߷¿� ���� ���� �ӵ��� ���ҽ�Ŵ

	// ���� ���� ���¸� �Ǻ��Ͽ� isGrounded ������ ����
	bool isGrounded = (GetTransform()->GetLocalPosition().y <= 0.0f);

	// ���� ���� ���¿����� �߷¿� ���� �ӵ� ���Ҹ� ����
	if (isGrounded)
	{
		verticalVelocity -= GRAVITY * DELTA_TIME;
	}

	// �÷��̾��� ���� ��ġ�� ������
	Vec3 currentPosition = GetTransform()->GetLocalPosition();

	Vec3 tempPos = currentPosition;

	// �÷��̾��� �̵� �ӵ� �� ���� ���� (���÷� WASD Ű�� �̿��� �̵�)
	const float moveSpeed = 200.0f;
	Vec3 moveDirection = Vec3(0.0f, 0.0f, 0.0f);

	if (INPUT->GetButton(KEY_TYPE::W))
	{
		moveDirection += XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f));
		//// SoundPlayer ��ü ����
		//SoundPlayer soundPlayer;

		//// ���� ���� ���
		//const wchar_t* filename = L"Footstep01.wav";

		//// ���� ���
		//soundPlayer.PlaySound(filename);

	}
	if (INPUT->GetButton(KEY_TYPE::S))
	{
		moveDirection -= XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)); 

		//GET_SINGLE(SceneManager)->SetMapPosition(0, 0);
	}
	if (INPUT->GetButton(KEY_TYPE::A))
	{
		moveDirection -= GetTransform()->GetRight();
	}
	if (INPUT->GetButton(KEY_TYPE::D))
	{
		moveDirection += GetTransform()->GetRight();
	}

	// �̵� ���� ������ ���̸� 1�� ����ȭ�Ͽ� �̵� �ӵ��� �����ϰ� ��
	if (moveDirection.LengthSquared() > 0.0f)
	{
		moveDirection.Normalize();
	}

	{

		shared_ptr<GameObject> overlap = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, 99);
		if (overlap != NULL)
		{
#ifdef DEBUG_ON
			std::cout << overlap->GetTransform()->GetObjectID() << std::endl;
#endif
			isOverlap = true;
			//GetTransform()->SetLocalPosition(previousPosition);
			// �浹 ���� ���� ���

			//Vec3 playerMin = playerObject->GetCollider()->GetMinPoint();
			//Vec3 playerMax = playerObject->GetCollider()->GetMaxPoint();

			//Vec3 overlapMin = overlap->GetCollider()->GetMinPoint();
			//Vec3 overlapMax = overlap->GetCollider()->GetMaxPoint();

			//// �浹�� �߻��� ���� �Ǵ�
			//bool xAxisOverlap = (playerMax.x >= overlapMin.x && playerMin.x <= overlapMax.x);
			//bool yAxisOverlap = (playerMax.y >= overlapMin.y && playerMin.y <= overlapMax.y);
			//bool zAxisOverlap = (playerMax.z >= overlapMin.z && playerMin.z <= overlapMax.z);

			//// �浹�� �߻��� �࿡ ���� �浹 ������ �Ǵ�
			//if (xAxisOverlap) {
			//	if (playerMax.x > overlapMin.x && playerMin.x < overlapMin.x) {
			//		std::cout << "Overlap occurred on the left side" << std::endl;
			//	}
			//	else {
			//		std::cout << "Overlap occurred on the right side" << std::endl;
			//	}
			//}
			//if (yAxisOverlap) {
			//	if (playerMax.y > overlapMin.y && playerMin.y < overlapMin.y) {
			//		std::cout << "Overlap occurred on the bottom side" << std::endl;
			//	}
			//	else {
			//		std::cout << "Overlap occurred on the top side" << std::endl;
			//	}
			//}
			//if (zAxisOverlap) {
			//	if (playerMax.z > overlapMin.z && playerMin.z < overlapMin.z) {
			//		std::cout << "Overlap occurred on the back side" << std::endl;
			//	}
			//	else {
			//		std::cout << "Overlap occurred on the front side" << std::endl;
			//	}
			//}

			//GetTransform()->SetLocalPosition(previousPosition);


			Vec3 collisionDirection = GetTransform()->GetLocalPosition() - overlap->GetTransform()->GetLocalPosition();

			// �浹 ���� ���Ϳ� ���� �÷��̾��� �̵� ���� ����
			if (moveDirection.x > 0)
			{
				// �浹 ������ x �� ������ ���
				moveDirection.x = -1.f;
			}
			else if (moveDirection.x < 0)
			{
				// �浹 ������ x �� ������ ���
				moveDirection.x = 1.f;
			}
			if (moveDirection.z > 0)
			{
				// �浹 ������ z �� ������ ���
				moveDirection.z = -1.f;
			}
			else if (moveDirection.z < 0)
			{
				// �浹 ������ z �� ������ ���
				moveDirection.z = 1.f;
			}

			//�浹 �߻� �� ���� ��ġ�� �ǵ���
			GetTransform()->SetLocalPosition(previousPosition);
		}
		else if (overlap == NULL)
			isOverlap = false;
	}



	// �÷��̾��� ��ġ�� �̵� ����� �ӵ��� ���� ������Ʈ
	currentPosition += moveDirection * moveSpeed * DELTA_TIME;

	// �÷��̾ �� ������ �������� ���� �����ϱ� ���� y ��ǥ�� 0���� ����
	if (currentPosition.y < 0.0f)
	{
		currentPosition.y = 0.0f;
		// ���� ���� ���·� �����Ͽ� ���� �ӵ��� 0���� �ʱ�ȭ
		verticalVelocity = 0.0f;
	}

	if (currentPosition != tempPos)
	{
		session->set_moving(true);
		//------------------------------------
		cs_packet_pos_info packet;
		packet.size = sizeof(cs_packet_pos_info);
		packet.type = CS_POS_INFO;
		packet.x = currentPosition.x;
		packet.y = currentPosition.y;
		packet.z = currentPosition.z;

		session->Send_Packet(&packet);
		//-------------------------------------
	}
	else if(session->get_moving()){
		session->set_moving(false);

		cs_packet_player_stop packet;
		packet.size = sizeof(cs_packet_player_stop);
		packet.type = CS_PLAYER_STOP;
		session->Send_Packet(&packet);
	}


	// ������Ʈ�� ��ġ�� �÷��̾ �ݿ�
	GetTransform()->SetLocalPosition(currentPosition);



	
	if (INPUT->GetButton(KEY_TYPE::P))
	{
		GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 0.f));
	}



	//�浹�˻�
	{
		
	}

	//if (isOverlap)
	//{
	//	/*shared_ptr<GameObject> playerObject = GET_SINGLE(SceneManager)->GetPlayer(playerID);
	//	Vec3 pos = playerObject->GetTransform()->GetLocalPosition();
	//	pos.x = pos.x - beforePosIncrease.x * _speed * DELTA_TIME;
	//	pos.z = pos.z - beforePosIncrease.z * _speed * DELTA_TIME;
	//	playerObject->GetTransform()->SetLocalPosition(pos);*/

	//}
	//else if (!isOverlap)
	//{
	//	MoveUpdate();
	//}

	if (isMouseMod)
	{
		RotationUpdate();
	}
	else if (!isMouseMod)
	{

	}














#ifdef DEBUG_ON
	//���콺 ������� ���� P�Է½� ���α׷��� �����ϵ��� �Ͽ���.
	if (INPUT->GetButton(KEY_TYPE::C))
		PostQuitMessage(0);
#endif

	HWND foregroundWindow = GetForegroundWindow();
	wchar_t windowTitle[256] = { 0 };
	GetWindowTextW(foregroundWindow, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));

	if (previousTitle[0] == L'\0')
	{
		// previousTitle �迭�� ��� �ִ��� Ȯ���ϰ�, ��� �ִٸ� windowTitle ������ �����մϴ�.
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
			//std::cout << "�������� ���� üũ��尡 �ߵ���" << std::endl;
		}
		else if (isMouseMod == true)
		{
			::ShowCursor(true);
			isMouseMod = false;
			isWindowCapture = false;
			//std::cout << "�������� ���� üũ��尡 ������" << std::endl;
		}
	}
	
	if (isWindowCapture == true)
	{
		//â�� ��ȭ�� �Ͼ�ٸ�.
		if (wcscmp(windowTitle, previousTitle) != 0)
		{

			//std::cout << "â�� ����Ǿ���" << std::endl;

			int len = WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, NULL, 0, NULL, NULL);
			char* buffer = new char[len];
			WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, buffer, len, NULL, NULL);

			const char* compareString = "PROJECT_ReWork";

			// strcmp �Լ��� ����Ͽ� buffer�� ���ϰ��� �ϴ� ���ڿ��� ��
			if (strstr(buffer, compareString) != nullptr)
			{

				//std::cout << "���� ȭ���� ����ȭ���Դϴ�" << std::endl;

				//���� Ȱ��ȭ�� ȭ���� ����ȭ���̶��
				isMouseMod = true;
				::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
			}
			else {

				//std::cout << "���� ȭ���� ����ȭ���� �ƴմϴ�." << std::endl;

				//���� Ȱ��ȭ�� ȭ���� ������ �ƴ϶��
				isMouseMod = false;
			}

			delete[] buffer;
		}
		else
		{
			//std::cout << "â�� ������� ����" << std::endl;
		}
	}
	else if(isWindowCapture == false)
	{
		
	}



	//Picking �Է��� Ȯ��
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

				//���⼭ Ÿ���� �÷��̾��϶���
				//�� OT_PLAYER�϶��� ������ �����ϵ��� �Ѵ�.
				if (pickedObject->GetTransform()->GetObjectType() == OT_PLAYER)
				{
					cs_packet_picking_info ppi;
					ppi.size = sizeof(cs_packet_picking_info);
					ppi.type = CS_PICKING_INFO;
					ppi.target_id = pickedObject->GetTransform()->GetObjectID();

					session->Send_Packet(&ppi);
				}

				else {
					cs_packet_picking_info ppi;
					ppi.size = sizeof(cs_packet_picking_info);
					ppi.type = CS_PICKING_INFO;
					ppi.target_id = -1;

					session->Send_Packet(&ppi);
				}
			}


			timeElapse = 0.f;
		}
	}

#ifdef DEBUG_ON
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();

		shared_ptr<GameObject> pickedObject;

		pickedObject = GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);

		if (pickedObject == pickedMovingObject)
			pickedMovingObject = NULL;
		else
			pickedMovingObject = pickedObject;
	}
#endif

	if (pickedMovingObject != NULL)
		RotatingPickedObject();


	if (INPUT->GetButtonDown(KEY_TYPE::E))
	{
		//�÷��̾� ID�� Ž��
		shared_ptr<GameObject> playerObject = GET_SINGLE(SceneManager)->GetPlayer(playerID);
		shared_ptr<GameObject> keyCard;

		//playerObject�� OT_KEYCARDŸ�԰��� �浹�� �Ͼ������ Ȯ��
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
			//pos xyz �� �÷��̾��� ���� ��ġ
			//key_id�� �浹�� ī��Ű�� id��
			session->Send_Packet(&tgk);

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

			session->Send_Packet(&tut);
		}
	}

	{
		//// rotation ���� ���� �� �ʱ�ȭ
		//Vec3 rotation = GetTransform()->GetLocalRotation(); // �÷��̾��� ���� ȸ����

		//// �÷��̾� �߽����κ��� ������ ��ġ �� ȸ�� ����
		//Vec3 gunOffset(5.0f, -5.0f, 15.0f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

		//// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
		//Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
		//	QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

		//// gunOffset�� ȸ����Ŵ
		//Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

		//// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
		//playerGunObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

		//// ȸ���� ����
		//playerGunObject->GetTransform()->SetLocalRotation(GetTransform()->GetLocalRotation());

		//// �÷��̾ �������� �� �ݴ� �������� �̵�
		//Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
		//playerGunObject->GetTransform()->SetLocalPosition(newPosition);
	}
	
	{

		Vec3 rotation = GetTransform()->GetLocalRotation();

		Vec3 gunOffset(5.0f, -5.0f, 15.0f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

		// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
		Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
			QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

		// gunOffset�� ȸ����Ŵ
		Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

		// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
		playerGunObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

		// ���� ȸ�� �������� �����Ͽ� ���ʹϾ� ����
		Vec3 gunRotationOffset(-1.57f, 3.14f, 0.0f); // ���� ȸ�� ������
		Quaternion gunRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), gunRotationOffset.y) *
			QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), gunRotationOffset.x);

		// �÷��̾��� ȸ������ ���� ȸ���� �߰��Ͽ� ���� ���� ȸ�� ���ʹϾ� ����
		Quaternion finalGunRotationQuat = playerRotationQuat * gunRotationQuat;

		Vec3 gunRotation = finalGunRotationQuat.ToEulerAngles();

		// ȸ���� ����
		playerGunObject->GetTransform()->SetLocalRotation(gunRotation);

		// �÷��̾ �������� �� �ݴ� �������� �̵�
		Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
		playerGunObject->GetTransform()->SetLocalPosition(newPosition);
	}

	{
		Vec3 rotation = GetTransform()->GetLocalRotation();

		Vec3 gunOffset(0.0f, -40.0f, 10.0f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

		// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
		Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
			QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

		// gunOffset�� ȸ����Ŵ
		Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

		// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
		playerObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

		// ���� ȸ�� �������� �����Ͽ� ���ʹϾ� ����
		Vec3 gunRotationOffset(0.f, 3.14f, 0.0f); // ���� ȸ�� ������
		Quaternion gunRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), gunRotationOffset.y) *
			QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), gunRotationOffset.x);

		// �÷��̾��� ȸ������ ���� ȸ���� �߰��Ͽ� ���� ���� ȸ�� ���ʹϾ� ����
		Quaternion finalGunRotationQuat = playerRotationQuat * gunRotationQuat;

		Vec3 gunRotation = finalGunRotationQuat.ToEulerAngles();

		// ȸ���� ����
		playerObject->GetTransform()->SetLocalRotation(gunRotation);

		// �÷��̾ �������� �� �ݴ� �������� �̵�
		Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
		playerObject->GetTransform()->SetLocalPosition(newPosition);

		Vec3 hispos = playerObject->GetTransform()->GetLocalPosition();
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

		//std::cout << "W �Է� ó����" << std::endl;

		// Right���Ϳ� ���������� ������ -> ����
		pos += XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::S))
	{
		//pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		//std::cout << "S �Է� ó����" << std::endl;

		pos -= XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::A))
	{
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		//std::cout << "A �Է� ó����" << std::endl;

		// Look���Ϳ� ���������� ������ -> ������
		//pos -= XMVector3Cross(GetTransform()->GetLook(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::D))
	{
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		//std::cout << "D �Է� ó����" << std::endl;

		//pos += XMVector3Cross(GetTransform()->GetLook(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	pos.y = tempPos.y;

	//��ġ�� ����Ǿ������� ������ �����ϵ��� ����
	if (pos != tempPos)
	{
		//------------------------------------
		cs_packet_pos_info packet;
		packet.size = sizeof(cs_packet_pos_info);
		packet.type = CS_POS_INFO;
		packet.x = pos.x;
		packet.y = pos.y;
		packet.z = pos.z;

		session->Send_Packet(&packet);
		//-------------------------------------
	}

	GetTransform()->SetLocalPosition(pos);
}


void TestCameraScript::RotationUpdate()
{
	//���� ���� ���콺�� �����ǰ��� �о�ͼ� �����Ѵ�.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	Vec3 rotation = GetTransform()->GetLocalRotation();

	//���� ���콺�� �������� �߻��ߴٸ�
	if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	{
		//������ ���� �����Ѵ�.
		int moveX = nowMousePos.x - WINDOW_MIDDLE_X;
		int moveY = nowMousePos.y - WINDOW_MIDDLE_Y;

		//������
		if (moveX > 0)
		{
			rotation.y += DELTA_TIME * moveX;

			GetTransform()->SetLocalRotation(rotation);
		}

		// ����
		if (moveX < 0)
		{
			rotation.y += DELTA_TIME * moveX;

			GetTransform()->SetLocalRotation(rotation);
		}

		// �Ʒ�
		if (moveY > 0)
		{
			rotation.x += DELTA_TIME * moveY;

			//playerRotation.x -= DELTA_TIME * moveY;

			GetTransform()->SetLocalRotation(rotation);
		}

		//��
		if (moveY < 0)
		{
			rotation.x += DELTA_TIME * moveY;

			//playerRotation.x -= DELTA_TIME * moveY;

			GetTransform()->SetLocalRotation(rotation);
		}

		
		//---------------------------------
		// �̰����� rotation������ server�� �Ѱ��ָ� �ȴ�.
		cs_packet_mouse_info mi;
		mi.size = sizeof(cs_packet_mouse_info);
		mi.type = CS_MOUSE_INFO;
		mi.x = rotation.x;
		mi.y = rotation.y;
		mi.z = 0.0f;

		session->Send_Packet(&mi);
		//---------------------------------
	}


	//���콺�� ��ġ�� �߾����� �ʱ�ȭ ���ش�.
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
