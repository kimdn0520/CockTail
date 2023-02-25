#include "pch.h"
#include "EnemyAttackRange.h"
#include "Component/SphereCollider.h"

namespace ClientSpace
{


	void EnemyAttackRange::SetRadiusToCollider()
	{
	}

	EnemyAttackRange::EnemyAttackRange(std::weak_ptr<GameObject> _obj)
		: Trigger(_obj)
		, radius(2)
		, isCollide(false)
	{
		attackRangeObject = SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		attackRangeObject->AddComponent<SphereCollider>()->SetTrigger(true);
		this->GetGameObject()->SetChild(attackRangeObject);
		attackRangeObject->GetComponent<Transform>()->SetPosition({ 0.f,1.5f,0.f }, Space::LOCAL);
		attackRangeObject->GetComponent<SphereCollider>()->AddTriggerEvent(this);
		attackRangeObject->GetComponent<SphereCollider>()->InitRenderObj();
		attackRangeObject->GetComponent<SphereCollider>()->SetRadius(radius);
		radius = 2.0f;
		SetRadiusToCollider();

		attackRangeObject->GetComponent<Collider>()->Start();
	}

	EnemyAttackRange::~EnemyAttackRange()
	{

	}

	void EnemyAttackRange::Awake()
	{
		radius = 2.0f;
		SetRadiusToCollider();
	}

	void EnemyAttackRange::OnTriggerEnter(std::string name)
	{
		std::shared_ptr<GameObject> collier = SceneManager::GetInstance()->FindObject(name);
		if (collier != nullptr)
		{
			if (collier->GetComponent<Collider>()->GetLayer() == "Player")
			{
				isCollide = true;
			}
		}
	}

	void EnemyAttackRange::OnTriggerExit(std::string name)
	{
		std::shared_ptr<GameObject> collier = SceneManager::GetInstance()->FindObject(name);
		if (collier != nullptr)
		{
			if (collier->GetComponent<Collider>()->GetLayer() == "Player")
			{
				isCollide = false;
			}
		}
	}

	void EnemyAttackRange::SetRadius(float value)
	{
		radius = value;
		SetRadiusToCollider();
	}

}
