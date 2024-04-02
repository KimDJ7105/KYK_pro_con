#include "pch.h"
#include "Engine.h"
#include "ObjectManager.h"



void ObjectManager::SetGameObjectData(std::shared_ptr<MyGameObject> obj)
{
	m_Obj = obj;
}

void ObjectManager::AddGameObjectData(MyGameObject obj)
{
	//vp_ObjectManager.push_back(obj);
}
