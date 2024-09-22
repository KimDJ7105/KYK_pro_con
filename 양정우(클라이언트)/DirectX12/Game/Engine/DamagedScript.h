#pragma once
#include "MonoBehaviour.h"

class DamagedScript : public MonoBehaviour
{
public:
	DamagedScript();
	virtual ~DamagedScript();

	virtual void LateUpdate() override;


private:
    float shakeDuration = 0.0f;    // ��鸮�� �ð�
    float shakeIntensity = 0.1f;   // ��鸲�� ����
    float shakeTimer = 0.0f;       // ���� ��鸮�� �ִ� �ð�
    bool isShaking = false;        // ��鸲 Ȱ��ȭ ����

public:
    void TriggerShake(float duration);


};

