#pragma once
#include "Component.h"

enum class ColliderType
{
	Sphere,
	Box,
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) = 0;
	virtual bool isColliding(const BoundingBox& boxToCheck) = 0;

	ColliderType GetColliderType() { return _colliderType; }

	//virtual BoundingSphere GetSphereCollider();
	virtual BoundingBox GetBoxCollider() const = 0;

	virtual Vec3 GetMinPoint() = 0;
	virtual Vec3 GetMaxPoint() = 0;

private:
	ColliderType _colliderType = {};
};