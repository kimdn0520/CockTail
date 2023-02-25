#include "GamePch.h"
#include "ObjectPool.h"

namespace GameEngineSpace
{
	ObjectPool::ObjectPool()
	{}

	ObjectPool::~ObjectPool()
	{}

	void ObjectPool::RegisterGameObject(std::string name, int size, std::function<std::shared_ptr<GameObject>()> gameObjectFunction)
	{
		std::vector<std::shared_ptr<GameObject>> objects;

		for (int i = 0; i < size; ++i)
		{
			objects.push_back(gameObjectFunction());
		}

		pool[name] = objects;
	}

	std::shared_ptr<GameObject> ObjectPool::AcquireGameObject(std::string name)
	{
		for(auto& [name, objects] : pool)
		{
			for(auto& obj : objects)
			{
				if(!obj->GetIsEnable())
				{
					obj->SetGameObjectEnable(true);

					return obj;
				}
			}
		}
	}

	void ObjectPool::ReturnGameObject(std::shared_ptr<GameObject> gameObject)
	{
		gameObject->SetGameObjectEnable(false);
	}
		
	void ObjectPool::Release()
	{
		for (auto& objectList : pool)
		{
			for (auto& obj : objectList.second)
			{
				obj.reset();
			}
			objectList.second.clear();
		}
		pool.clear();
	}
}