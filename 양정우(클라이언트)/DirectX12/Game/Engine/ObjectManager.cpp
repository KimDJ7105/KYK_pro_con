#include "pch.h"
#include "Engine.h"
#include "ObjectManager.h"

void ObjectManager::SetGameObjectData(MyGameObject obj)
{
	m_Obj.m_ObjectType = obj.m_ObjectType;
}

void ObjectManager::AddGameObjectData(shared_ptr<MyGameObject> obj)
{
	// ������ Set���� ObjectData�� ������Ʈ�� ����Ʈ�� ������� �Լ�
	vp_objectManager.push_back(obj);
}
