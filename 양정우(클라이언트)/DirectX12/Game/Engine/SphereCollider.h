#pragma once
#include "BaseCollider.h"

class SphereCollider : public BaseCollider
{
public:
	SphereCollider();
	virtual ~SphereCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;
	virtual bool isColliding(const BoundingBox& boxToCheck) override {return false; }

	void SetRadius(float radius) { _radius = radius; }
	void SetCenter(Vec3 center) { _center = center; }

	//BoundingSphere GetSphereCollider() { return _boundingSphere; } override;
	virtual BoundingBox GetBoxCollider() const override {return BoundingBox();}

private:
	// Local ±‚¡ÿ
	float		_radius = 1.f;
	Vec3		_center = Vec3(0, 0, 0);

	BoundingSphere _boundingSphere;
};