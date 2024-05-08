#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"

BoxCollider::BoxCollider() : BaseCollider(ColliderType::Box)
{


}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::FinalUpdate()
{
	_boundingBox.Center = GetGameObject()->GetTransform()->GetWorldPosition();
	_boundingBox.Extents = XMFLOAT3(1.f, 1.f, 1.f);
}

bool BoxCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	return _boundingBox.Intersects(rayOrigin, rayDir, OUT distance);
}
