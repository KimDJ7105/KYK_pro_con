#pragma once
#include "BaseCollider.h"

class BoxCollider : public BaseCollider
{
public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual void FinalUpdate() override;
	virtual bool Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance) override;

	bool isColliding(const BoundingBox& boxToCheck) override;

	//void SetRadius(float radius) { _radius = radius; }
	void SetCenter(Vec3 center) { _center = center; }
	void SetExtents(Vec3 extents) { _extents = extents; }

	virtual BoundingBox GetBoxCollider() const override { return _boundingBox; }

	void SetStatic(bool _static) { isStatic = _static; }

	virtual Vec3 GetMinPoint();
	virtual Vec3 GetMaxPoint();


	virtual void LateUpdate() override;

private:
	// Local ±‚¡ÿ
	Vec3		_center = Vec3(0, 0, 0);
	Vec3		_extents = Vec3(1, 1, 1);

	int _myNum = -1;

	BoundingBox _boundingBox;

	BoundingOrientedBox _boundingOrientedBox;

	bool isStatic = true;

	
	
};

