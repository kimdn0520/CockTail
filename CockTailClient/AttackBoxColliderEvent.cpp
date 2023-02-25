#include "pch.h"
#include "AttackBoxColliderEvent.h"
#include "Component/BoxCollider.h"

ClientSpace::AttackBoxColliderEvent::AttackBoxColliderEvent(std::weak_ptr<GameObject> _object)
	: EventCollider(_object)
{

}

ClientSpace::AttackBoxColliderEvent::~AttackBoxColliderEvent()
{

}

void ClientSpace::AttackBoxColliderEvent::Awake()
{
	player = GetGameObject()->GetComponent<PlayerController>();
	attackCon = GetGameObject()->GetComponent<PlayerAttackController>();

	for (int i = 0; i < 3; i++)
		attackCon->GetAttackBox(i)->GetComponent<BoxCollider>()->AddColliderEvent(this);
}

void ClientSpace::AttackBoxColliderEvent::Start()
{

}

void ClientSpace::AttackBoxColliderEvent::OnCollisionEnter(std::string name)
{
	UINT uid = std::stoi(name);
	
}

void ClientSpace::AttackBoxColliderEvent::OnCollisionPersist(std::string name)
{

}

void ClientSpace::AttackBoxColliderEvent::OnCollisionExit(std::string name)
{

}
