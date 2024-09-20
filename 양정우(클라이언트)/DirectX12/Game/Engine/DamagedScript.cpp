#include "pch.h"
#include "DamagedScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"

#include "SoundManager.h"

#include "session.h"

#include <cstdlib>  // rand() �Լ� ����� ���� ��� ����
#include <ctime>    // srand()�� ���� ��� ����

DamagedScript::DamagedScript()
{
}

DamagedScript::~DamagedScript()
{
}

void DamagedScript::LateUpdate()
{ 
	Vec3 rot = GetTransform()->GetLocalRotation();

    //���⿡ �������� �Ծ������� �ۼ�
    if (GET_SINGLE(SceneManager)->Get_Damaged() == true)
    {
        GET_SINGLE(SceneManager)->Set_Damaged(false);

        // 1 ~ 3 �� ���� �� ����
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
        // ��鸮�� ���� Y�� ȸ������ sin �Լ��� ����
        float shakeAmount = sin(shakeTimer * 20.0f) * shakeIntensity;
        rot.y += shakeAmount;

        // Ÿ�̸� ����
        shakeTimer += DELTA_TIME;

        // ��鸲 �ð��� ����Ǹ� ��鸲�� ��Ȱ��ȭ
        if (shakeTimer >= shakeDuration)
        {
            isShaking = false;
            shakeTimer = 0.0f;

            GET_SINGLE(SceneManager)->SetDamagedUI(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f));
            // ��鸲�� ������ ���� ������ ����
            //rot.y = 0.0f; // ���� ���� ������ ���� ������ �� ������ ����
        }
    }

    // ���ŵ� ȸ���� ����
    GetTransform()->SetLocalRotation(rot);

    if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
    {
        //---------------------------------
        // �̰����� rotation������ server�� �Ѱ��ָ� �ȴ�.
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
        //// �̰����� rotation������ server�� �Ѱ��ָ� �ȴ�.
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