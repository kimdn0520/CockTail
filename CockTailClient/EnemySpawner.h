#pragma once

namespace ClientSpace
{

	class EnemySpawner : public ComponentBase
	{
	private:
		std::map <tstring, std::shared_ptr<GameObject>> prefabMap;
		Vector3 position;
		Vector3 rotation;

		std::shared_ptr<ObjectPool> objectPool;

	public:
		EnemySpawner(std::weak_ptr<GameObject> _obj);
		virtual ~EnemySpawner();

		virtual void Awake();
		virtual void Update(float tick);

		void CreateEnemyPrefeb(std::shared_ptr<GameObject> _obj);
		void SpawnEnemy();

		void TestObjectPool(std::shared_ptr<ObjectPool> _objectPool) { objectPool = _objectPool; }

		std::shared_ptr<GameObject> GetPrefeb(std::string name);
	};
}