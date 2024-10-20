#include "pch.h"
#include "Input.h"
#include "Engine.h"

void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void Input::Update()
{
	HWND hwnd = ::GetActiveWindow();
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

    for (uint32 key = 0; key < KEY_TYPE_COUNT; key++) {
        SHORT keyState = ::GetAsyncKeyState(key);
        if (keyState & 0x8000) { // 키가 눌려 있을 때
            KEY_STATE& state = _states[key];
            if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
                state = KEY_STATE::PRESS;
            else
                state = KEY_STATE::DOWN;
        }
        else {
            KEY_STATE& state = _states[key];
            if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
                state = KEY_STATE::UP;
            else
                state = KEY_STATE::NONE;
        }
    }

	// 매 프레임마다의 마우스 위치를 기억한다.
	::GetCursorPos(&_mousePos);
	::ScreenToClient(GEngine->GetWindow().hwnd, &_mousePos);
}