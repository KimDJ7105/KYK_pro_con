#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"

BoxCollider::BoxCollider() : BaseCollider(ColliderType::Box)
{


}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::FinalUpdate()
{
	_boundingBox.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();
	Vec3 rotation = GetGameObject()->GetTransform()->GetLocalRotation();

	//_boundingBox.Extents = _extents * scale;
	_boundingBox.Extents = _extents;

	/*_boundingOrientedBox.Center = GetGameObject()->GetTransform()->GetWorldPosition();
	_boundingOrientedBox.Extents = scale;
	_boundingOrientedBox.Orientation = XMFLOAT4(rotation.x, rotation.y, rotation.z, 1.f);*/
}

bool BoxCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	//return _boundingBox.Intersects(rayOrigin, rayDir, OUT distance);
	return _boundingBox.Intersects(rayOrigin, rayDir, OUT distance);
}


bool BoxCollider::isColliding(const BoundingBox& boxToCheck) {
	// DirectX Toolkit의 Intersects 함수를 사용하여 충돌 여부를 판정합니다.
	return _boundingBox.Intersects(boxToCheck);
}


