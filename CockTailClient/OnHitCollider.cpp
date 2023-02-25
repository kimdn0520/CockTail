#include "pch.h"
#include "OnHitCollider.h"
#include "Component/BoxCollider.h"
#include "SceneManager.h"

ClientSpace::OnHitCollider::OnHitCollider(std::weak_ptr<GameObject> _object)
	: EventCollider(_object)
{

}

ClientSpace::OnHitCollider::~OnHitCollider()
{

}

void ClientSpace::OnHitCollider::Awake()
{
	attackCon = GetGameObject()->GetComponent<PlayerAttackController>();
	
	for (int i = 1; i < 4; i++)
	{
		attackCon->GetAttackBox(i)->GetComponent<BoxCollider>()->AddColliderEvent(this);
	}
}

void ClientSpace::OnHitCollider::Start()
{

}

void ClientSpace::OnHitCollider::OnCollisionEnter(std::string name)
{
	std::shared_ptr<GameObject> collider = SceneManager::GetInstance()->FindObject(name);
	if (collider != nullptr)
	{
		if (collider->GetComponent<Collider>()->GetLayer() == "ENEMY")
		{
			int a = 0;
		}
	}
}

void ClientSpace::OnHitCollider::OnCollisionExit(std::string name)
{

}

void ClientSpace::OnHitCollider::SetColliderEvent(bool value)
{

}
