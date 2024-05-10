#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"

BoxCollider::BoxCollider() : BaseCollider(ColliderType::Box)
{



	//_myNum = GET_SINGLE(SceneManager)->RenderAABBBox(_center, _extents);
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::FinalUpdate()
{
	if (isStatic == false)
	{
		Vec3 pos = GetGameObject()->GetTransform()->GetWorldPosition();
		//Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();

		/*if (scale.x < 5.f)
		{
			float max = std::max({ scale.x, scale.y, scale.z });
			scale.x = max;
		}
		if (scale.y < 5.f)
		{
			float max = std::max({ scale.x, scale.y, scale.z });
			scale.y = max;
		}
		if (scale.z < 5.f)
		{
			float max = std::max({ scale.x, scale.y, scale.z });
			scale.z = max;
		}*/
		Vec3 rotation = GetGameObject()->GetTransform()->GetLocalRotation();

		_boundingBox.Center = pos;
		_boundingBox.Extents = _extents;
		//_boundingBox.Extents = _extents;

		/*_boundingOrientedBox.Center = GetGameObject()->GetTransform()->GetWorldPosition();
		_boundingOrientedBox.Extents = _extents;
		_boundingOrientedBox.Orientation = XMFLOAT4(rotation.x, rotation.y, rotation.z, 1.f);*/

		//GET_SINGLE(SceneManager)->UpdateAABBBox(_myNum, pos, _extents * 2.f, rotation);
	}
	else if (isStatic == true)
	{
		Vec3 rotation = GetGameObject()->GetTransform()->GetLocalRotation();
		_boundingBox.Center = _center;
		_boundingBox.Extents = _extents;
		//GET_SINGLE(SceneManager)->UpdateAABBBox(_myNum, _center, _extents, rotation);
	}
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


