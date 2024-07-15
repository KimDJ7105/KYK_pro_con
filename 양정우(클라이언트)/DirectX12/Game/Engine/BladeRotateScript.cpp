#include "pch.h"
#include "BladeRotateScript.h"
#include "SceneManager.h"
#include "Timer.h"
#include "Transform.h"

BladeRotateScript::BladeRotateScript()
{
}

BladeRotateScript::~BladeRotateScript()
{
}

void BladeRotateScript::LateUpdate()
{
	Vec3 rotation = GetTransform()->GetLocalRotation();

	rotation.x += 1 * DELTA_TIME;

	GetTransform()->SetLocalRotation(rotation);
}
