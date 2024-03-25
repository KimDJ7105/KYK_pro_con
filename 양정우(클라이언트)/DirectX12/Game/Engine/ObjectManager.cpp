#include "pch.h"
#include "Engine.h"
#include "ObjectManager.h"

void ObjectManager::SetGameObjectData(MyGameObject obj)
{
	m_Obj.m_ObjectType = obj.m_ObjectType;
}

void ObjectManager::AddGameObjectData(shared_ptr<MyGameObject> obj)
{
	// 위에서 Set해준 ObjectData를 오브젝트의 리스트에 집어넣을 함수
	vp_objectManager.push_back(obj);
}
