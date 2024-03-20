#pragma once
#include "Component.h"

enum class GAMEOBJECT_TYPE
{
	//1 = player, 2 = other player, 3 = static object, 4 = moveable object
	DEFAULT,
	PLAYER,
	OTHER_PLAYER,
	STATIC_OBJECT,
	MOVEABLE_OBJECT,

	TYPE_END
};

struct MyGameObject
{
	int m_ObjectType;
	int m_ObjectID;
	Vec3 m_ObjectLocation;
	int m_animationID;
	Vec3 m_Direction;
};

class ObjectManager : Component
{
private:
	MyGameObject m_Obj;

	//연걸리스트 혹은 B+트리 형식으로 오브젝트의 배열을 새로 만들 공간

public:
	MyGameObject GetObjectData() { return m_Obj; }

	void SetGameObjectData(MyGameObject obj);

	void AddGameObjectData();
};

