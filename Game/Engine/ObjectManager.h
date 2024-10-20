#pragma once
#include "Component.h"



class ObjectManager : Component
{
private:
	std::shared_ptr<MyGameObject> m_Obj;


public:
	std::shared_ptr<MyGameObject> GetObjectData() { return m_Obj; }

	void SetGameObjectData(std::shared_ptr<MyGameObject> obj);

	void AddGameObjectData(MyGameObject obj);
};

