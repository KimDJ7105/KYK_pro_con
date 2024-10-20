#include "pch.h"
#include "RotationComponent.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"

#include "SoundManager.h"

RotationComponent::RotationComponent()
{
}

RotationComponent::~RotationComponent()
{
}

void RotationComponent::LateUpdate()
{

	Vec3 rot = GetTransform()->GetLocalRotation();

	rot.y += 0.05;

	GetTransform()->SetLocalRotation(rot);

}
