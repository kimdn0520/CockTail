#include "pch.h"
#include "EnemyHitTrigger.h"
#include "Scripts/PlayerController.h"
#include "PlayerAttackController.h"
#include "Component/BoxCollider.h"
#include "EnemyController.h"

namespace ClientSpace
{

	EnemyHitTrigger::EnemyHitTrigger(std::weak_ptr<GameObject> _object)
		: Trigger(_object)
	{

	}

	EnemyHitTrigger::~EnemyHitTrigger()
	{

	}

	void EnemyHitTrigger::Awake()
	{
		attackCon = GetGameObject()->GetComponent<PlayerAttackController>();

		for (int i = 1; i < 4; i++)
		{
			attackCon->GetAttackBox(i)->GetComponent<BoxCollider>()->AddTriggerEvent(this);
		}

		transform = GetGameObject()->GetComponent<Transform>();
	}

	void EnemyHitTrigger::Start()
	{

	}

	void EnemyHitTrigger::OnTriggerEnter(std::string name)
	{
		std::shared_ptr<GameObject> collider = SceneManager::GetInstance()->FindObject(name);
		if (collider != nullptr)
		{
			if (collider->GetComponent<Collider>()->GetLayer() == "Enemy")
			{
				std::shared_ptr<EnemyController> enemy = collider->GetComponent<EnemyController>();
				Vector3 pos = transform->GetWorldPosition();
				enemy->SetDirection(pos);
				enemy->SetOn();
			}
		}
	}

	void EnemyHitTrigger::OnTriggerExit(std::string name)
	{

	}

	void EnemyHitTrigger::SetTriggerEvent(bool value)
	{

	}

}
