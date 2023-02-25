#include "pch.h"
#include "OnHitTrigger.h"
#include "Component/BoxCollider.h"
#include "DamageTest.h"

namespace ClientSpace
{
	OnHitTrigger::OnHitTrigger(std::weak_ptr<GameObject> _object)
		: Trigger(_object)
	{

	}

	OnHitTrigger::~OnHitTrigger()
	{

	}

	void OnHitTrigger::Awake()
	{
		attackCon = GetGameObject()->GetComponent<PlayerAttackController>();

		for (int i = 1; i < 4; i++)
		{
			attackCon->GetAttackBox(i)->GetComponent<BoxCollider>()->AddTriggerEvent(this);
		}

		transform = GetGameObject()->GetComponent<Transform>();
	}

	void OnHitTrigger::Start()
	{

	}

	void OnHitTrigger::OnTriggerEnter(std::string name)
	{
		std::shared_ptr<GameObject> collider = SceneManager::GetInstance()->FindObject(name);
		if (collider != nullptr)
		{
			if (collider->GetComponent<Collider>()->GetLayer() == "Enemy")
			{
				std::shared_ptr<DamageTest> test = collider->GetComponent<DamageTest>();
				Vector3 pos = transform->GetWorldPosition();
				test->SetDirection(pos);
				test->SetOn();
			}
		}
	}

	void OnHitTrigger::OnTriggerExit(std::string name)
	{

	}

	void OnHitTrigger::SetTriggerEvent(bool value)
	{

	}
}