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
	// Parent 기준
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
	// Parent 기준
	Vec3 _localPosition = {};
	Vec3 _localRotation = {};
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal = {};
	Matrix _matWorld = {};	// 계산한 월드변환행렬을 저장하기 위한 용도

	weak_ptr<Transform> _parent;

public:
	void SetObjectID(int objectID) { _objectID = objectID; }
	int GetObjectID() { return _objectID; }
	void SetObjectType(int objectType) { _objectType = objectType; }
	int GetObjectType() { return _objectType; }

private:
	int _objectID = -1;		
	int _objectType = -1;

	//99번 - 충돌박스가 있는 스태틱 오브젝트(벽)
	//101번 - 플레이어 손 오브젝트 아이디
	//102번 - 플레이어가 들고있는 총UI 아이디.
	//103번 - 맵 오브젝트 - 1번은 맵틀, 2번은 맵
	//104번 - 체력 UI - 1번부터 10번까지 순서대로 체력번호
	//105번 - 키카드 - 1번부터 3번까지 있음
	//106번 - 총 UI
	//107번 - 슬래시(총알갯수구분슬래시)UI
	//108번 - 최대장탄수 1번은 10의자리, 2번은 1의자리.
	//109번 - 현재 총알의 장탄수 
	//		- 0번부터 9번까지 1의자리
	//		- 10번부터 19번까지 10의자리
	//110번 - 토끼발
	//111번 - 맵에 뜨는 오브젝트 UI들
};

