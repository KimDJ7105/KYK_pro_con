#include "pch.h"
#include "ButtonScript.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "Engine.h"

ButtonScript::ButtonScript()
{
}

ButtonScript::~ButtonScript()
{
}

void ButtonScript::LateUpdate()
{
    POINT nowMousePos;
    ::GetCursorPos(&nowMousePos);
    ScreenToClient(GetActiveWindow(), &nowMousePos);

    // Get button transform properties
    Vec3 scale = GetTransform()->GetLocalScale();
    Vec3 position = GetTransform()->GetLocalPosition();

    // Convert button position from world to screen coordinates
    Vec2 screenPos = WorldToScreen(position);

    // Calculate button boundaries in screen coordinates
    float halfWidth = scale.x / 2.0f;
    float halfHeight = scale.y / 2.0f;

    float left = screenPos.x - halfWidth;
    float right = screenPos.x + halfWidth;
    float top = screenPos.y - halfHeight;
    float bottom = screenPos.y + halfHeight;

    nowMousePos.y = nowMousePos.y * 1.1;

    // Print mouse click position
    if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
    {
        /*std::cout << "���콺 Ŭ���� X��ǥ : " << nowMousePos.x << std::endl;
        std::cout << "���콺 Ŭ���� Y��ǥ : " << nowMousePos.y << std::endl;

        std::cout << "��ư�� ��ũ�� X��ǥ : " << screenPos.x << std::endl;
        std::cout << "��ư�� ��ũ�� Y��ǥ : " << screenPos.y << std::endl;*/

        // Check if the mouse click is within the button boundaries
        if (nowMousePos.x >= left && nowMousePos.x <= right &&
            nowMousePos.y >= top && nowMousePos.y <= bottom)
        {
            //std::cout << "Button Clicked!" << std::endl;

            GET_SINGLE(SceneManager)->SetButton(GetTransform()->GetObjectType(), GetTransform()->GetObjectID());
        }
        else
        {
            //GET_SINGLE(SceneManager)->SetButton(-1, -1);
        }
    }
}

Vec2 ButtonScript::WorldToScreen(Vec3 worldPos)
{
    // Placeholder for the actual world to screen conversion logic
    // This logic would depend on your rendering engine and camera setup
    // Here, we simply convert world coordinates to screen coordinates
    // assuming a basic orthographic projection for simplicity.

    int screenWidth = WINDOW_WIDTH;  // Example screen width
    int screenHeight = WINDOW_HEIGHT; // Example screen height

    Vec2 screenPos;
    screenPos.x = (worldPos.x + (screenWidth / 2.0f));
    screenPos.y = ((screenHeight / 2.0f) - worldPos.y);

    return screenPos;
}