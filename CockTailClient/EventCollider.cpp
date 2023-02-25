#include "pch.h"
#include "pch.h"
#include "EventCollider.h"

ClientSpace::EventCollider::EventCollider(std::weak_ptr<GameObject> _object)
	: ComponentBase(_object)
	, isCollide(false)
{

}

void ClientSpace::EventCollider::Awake()
{

}

void ClientSpace::EventCollider::Start()
{

}

void ClientSpace::EventCollider::OnCollisionEnter(std::string name)
{

}

void ClientSpace::EventCollider::OnCollisionPersist(std::string name)
{

}

void ClientSpace::EventCollider::OnCollisionExit(std::string name)
{

}
