#include "pch.h"
#include "EnemySpawner.h"
#include "EnemyController.h"
#include "EnemyAttackRange.h"
#include "EnemyNavAgent.h"
#include "Manager/InputManager.h"

namespace ClientSpace 
{
	EnemySpawner::EnemySpawner(std::weak_ptr<GameObject> _obj)
		: ComponentBase(_obj)
	{

	}

	EnemySpawner::~EnemySpawner()
	{

	}

	void EnemySpawner::Awake()
	{
		position = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
		rotation = GetGameObject()->GetComponent<Transform>()->GetWorldRotation();
		position.y += 1.0f;
	}

	void EnemySpawner::Update(float tick)
	{
		if (InputManager::GetInstance()->GetInputState('P', KeyState::DOWN))
		{
			SpawnEnemy();
		}
	}

	void EnemySpawner::CreateEnemyPrefeb(std::shared_ptr<GameObject> _obj)
	{
		tstring name = _obj->GetName();

		// make_pair 쓰거나 그냥 { } 로 하면됨
		prefabMap.insert(std::make_pair(name, _obj));
	}

	void EnemySpawner::SpawnEnemy()
	{
		auto scene = SceneManager::GetInstance()->GetCurrentScene();

		//std::shared_ptr<GameObject> enemy = scene->CreateEmpty();
		//enemy->SetName(_obj->GetName());

		//for (auto component : _obj->GetComponents())
		//{
		//	enemy->AddComponent<component>();
		//}

		std::shared_ptr<GameObject> enemy = objectPool->AcquireGameObject("dog");

		if(enemy == nullptr)
			return;

		enemy->GetComponent<Transform>()->SetPosition(position);
		enemy->GetComponent<Transform>()->SetRotation(rotation);

		/*std::shared_ptr<GameObject> enemy = scene->CreateEmpty();
		enemy->AddComponent<MeshRenderer>()->Init("dog.model");
		enemy->GetComponent<Transform>()->SetPosition(position);
		enemy->GetComponent<Transform>()->SetRotation(rotation);
		enemy->AddComponent<EnemyController>();

		enemy->GetComponent<MeshRenderer>()->Awake();
		enemy->GetComponent<MeshRenderer>()->Start();
		enemy->GetComponent<EnemyController>()->Awake();
		enemy->GetComponent<EnemyController>()->Start();
		enemy->GetComponent<NavMeshAgent>()->Awake();
		enemy->GetComponent<NavMeshAgent>()->Start();
		enemy->GetComponent<EnemyNavAgent>()->Awake();
		enemy->GetComponent<EnemyNavAgent>()->Start();
		enemy->GetComponent<EnemyAttackRange>()->Awake();
		enemy->GetComponent<EnemyAttackRange>()->Start();
		enemy->GetComponent<Collider>()->Start();
		enemy->GetComponent<Rigidbody>()->Awake();*/
	}

	//std::shared_ptr<GameObject> EnemySpawner::GetPrefeb(std::string name)
	//{
	//	return
	//}
}