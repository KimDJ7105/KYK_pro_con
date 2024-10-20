#pragma once
#include "MonoBehaviour.h"

class DamagedScript : public MonoBehaviour
{
public:
	DamagedScript();
	virtual ~DamagedScript();

	virtual void LateUpdate() override;


private:
    float shakeDuration = 0.0f;    // 흔들리는 시간
    float shakeIntensity = 0.1f;   // 흔들림의 세기
    float shakeTimer = 0.0f;       // 현재 흔들리고 있는 시간
    bool isShaking = false;        // 흔들림 활성화 여부

public:
    void TriggerShake(float duration);


};

