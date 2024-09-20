#include "pch.h"
#include "DamagedScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"

#include "SoundManager.h"

#include "session.h"

#include <cstdlib>  // rand() 함수 사용을 위한 헤더 파일
#include <ctime>    // srand()를 위한 헤더 파일

DamagedScript::DamagedScript()
{
}

DamagedScript::~DamagedScript()
{
}

void DamagedScript::LateUpdate()
{ 
	Vec3 rot = GetTransform()->GetLocalRotation();

    //여기에 데미지를 입었을때를 작성
    if (GET_SINGLE(SceneManager)->Get_Damaged() == true)
    {
        GET_SINGLE(SceneManager)->Set_Damaged(false);

        // 1 ~ 3 중 랜덤 값 생성
        int randomSound = rand() % 3 + 1;

        switch (randomSound)
        {
        case 1:
            GET_SINGLE(SoundManager)->soundPlay(PLAYER_HIT_01, GetTransform()->GetLocalPosition(), false);
            break;
        case 2:
            GET_SINGLE(SoundManager)->soundPlay(PLAYER_HIT_02, GetTransform()->GetLocalPosition(), false);
            break;
        case 3:
            GET_SINGLE(SoundManager)->soundPlay(PLAYER_HIT_03, GetTransform()->GetLocalPosition(), false);
            break;
        }

        std::cout << "Damaged!" << std::endl;

        GET_SINGLE(SceneManager)->SetDamagedUI(Vec3(0.f, 0.f, 500.f));

        TriggerShake(0.5f);
    }


    if (isShaking)
    {
        // 흔들리는 동안 Y축 회전값을 sin 함수로 조절
        float shakeAmount = sin(shakeTimer * 20.0f) * shakeIntensity;
        rot.y += shakeAmount;

        // 타이머 갱신
        shakeTimer += DELTA_TIME;

        // 흔들림 시간이 종료되면 흔들림을 비활성화
        if (shakeTimer >= shakeDuration)
        {
            isShaking = false;
            shakeTimer = 0.0f;

            GET_SINGLE(SceneManager)->SetDamagedUI(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f));
            // 흔들림이 끝나면 원래 각도로 복귀
            //rot.y = 0.0f; // 원래 각도 저장한 값이 있으면 그 값으로 복구
        }
    }

    // 갱신된 회전값 적용
    GetTransform()->SetLocalRotation(rot);

    if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
    {
        //---------------------------------
        // 이곳에서 rotation정보를 server에 넘겨주면 된다.
        cs_packet_mouse_info mi;
        mi.size = sizeof(cs_packet_mouse_info);
        mi.type = CS_MOUSE_INFO;
        //mi.x = rotation.x;
        mi.x = 0.f;
        mi.y = rot.y + 3.14f;
        mi.z = 0.0f;

        main_session->Send_Packet(&mi);
        //---------------------------------
    }
    else if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
    {
        //---------------------------------
        //// 이곳에서 rotation정보를 server에 넘겨주면 된다.
        //cs_packet_mouse_info mi;
        //mi.size = sizeof(cs_packet_mouse_info);
        //mi.type = CS_MOUSE_INFO;
        ////mi.x = rotation.x;
        //mi.x = -1.57f;
        //mi.y = rot.y;
        //mi.z = 0.0f;

        //main_session->Send_Packet(&mi);
        //---------------------------------
    }
}

void DamagedScript::TriggerShake(float duration)
{
    shakeDuration = duration;
    shakeTimer = 0.0f;
    isShaking = true;
}