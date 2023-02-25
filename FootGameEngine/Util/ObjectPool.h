#pragma once
#include "Object/GameObject.h"

namespace GameEngineSpace
{
	class ObjectPool
	{
	public:
		ObjectPool();

		~ObjectPool();

		void RegisterGameObject(std::string name, int size, std::function<std::shared_ptr<GameObject>()> gameObjectFunction);

		std::shared_ptr<GameObject> AcquireGameObject(std::string name);

		void ReturnGameObject(std::shared_ptr<GameObject> gameObject);

		// 씬이 바뀔때마다 초기화 해주자
		void Release();

	private:
		std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> pool;
	};
}
