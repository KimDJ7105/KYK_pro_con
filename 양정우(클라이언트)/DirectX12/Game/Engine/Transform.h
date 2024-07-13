#pragma once
#include "Component.h"
#include <DirectXMath.h>

using std::weak_ptr;
using std::min;
using std::max;

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

public:
	// Parent ����
	const Vec3& GetLocalPosition() { return _localPosition; }
	const Vec3& GetLocalRotation() { return _localRotation; }
	const Vec3& GetLocalScale() { return _localScale; }

	// TEMP
	float GetBoundingSphereRadius() { return max(max(_localScale.x, _localScale.y), _localScale.z); }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	//const Vec3& GetWorldPosition() { return _matWorld.Translation(); }'
	Vec3 GetWorldPosition() { return _matWorld.Translation(); }
	XMFLOAT4 GetWorldRotation();

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; }
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vec3& scale) { _localScale = scale; }

	void LookAt(const Vec3& dir);

	static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3 DecomposeRotationMatrix(const Matrix& rotation);


public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

private:
	// Parent ����
	Vec3 _localPosition = {};
	Vec3 _localRotation = {};
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal = {};
	Matrix _matWorld = {};	// ����� ���庯ȯ����� �����ϱ� ���� �뵵

	weak_ptr<Transform> _parent;

public:
	void SetObjectID(int objectID) { _objectID = objectID; }
	int GetObjectID() { return _objectID; }
	void SetObjectType(int objectType) { _objectType = objectType; }
	int GetObjectType() { return _objectType; }

private:
	int _objectID = -1;		
	int _objectType = -1;

	//99�� - �浹�ڽ��� �ִ� ����ƽ ������Ʈ(��)
	//101�� - �÷��̾� �� ������Ʈ ���̵�
	//102�� - �÷��̾ ����ִ� ��UI ���̵�.
	//103�� - �� ������Ʈ - 1���� ��Ʋ, 2���� ��
	//104�� - ü�� UI - 1������ 10������ ������� ü�¹�ȣ
	//105�� - Űī�� - 1������ 3������ ����
	//106�� - �� UI
	//107�� - ������(�Ѿ˰������н�����)UI
	//108�� - �ִ���ź�� 1���� 10���ڸ�, 2���� 1���ڸ�.
	//109�� - ���� �Ѿ��� ��ź�� 
	//		- 0������ 9������ 1���ڸ�
	//		- 10������ 19������ 10���ڸ�
	//110�� - �䳢��
	//111�� - �ʿ� �ߴ� ������Ʈ UI��
};

