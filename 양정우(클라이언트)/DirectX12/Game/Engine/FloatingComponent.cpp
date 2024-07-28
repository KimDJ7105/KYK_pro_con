#include "pch.h"
#include "FloatingComponent.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"

#include "SoundManager.h"

FloatingComponent::FloatingComponent()
{
}

FloatingComponent::~FloatingComponent()
{
}

void FloatingComponent::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	pos.y += sin(ElapsedTime) * 0.05;

	GetTransform()->SetLocalPosition(pos);

	ElapsedTime += DELTA_TIME;
}
