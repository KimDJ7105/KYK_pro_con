#pragma once

#include "MonoBehaviour.h"
#include "Quaternion.h"
struct Quaternion;

class CrusherScript : public MonoBehaviour
{
public:
	CrusherScript();
	virtual ~CrusherScript();

	virtual void LateUpdate() override;

private:
	bool isStart = false;


	shared_ptr<GameObject> blade_upper;
	shared_ptr<GameObject> blade_middle;
	shared_ptr<GameObject> blade_bottom;

	float crusherScale = 0.0f;

	// ���̵� ��ü ȸ�� �ӵ� (����/��)
	float bladeRotationSpeed = 1.f; // �ʿ信 ���� ����

	int my_NUM = -1;
};

