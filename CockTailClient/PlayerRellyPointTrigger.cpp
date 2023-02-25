#include "pch.h"
#include "PlayerRellyPointTrigger.h"
#include "Scripts/PlayerController.h"

ClientSpace::PlayerRellyPointTrigger::PlayerRellyPointTrigger(std::weak_ptr<GameObject> _obj)
	: Trigger(_obj)
{

}

ClientSpace::PlayerRellyPointTrigger::~PlayerRellyPointTrigger()
{

}

void ClientSpace::PlayerRellyPointTrigger::Awake()
{
	collider = GetGameObject()->GetComponent<Collider>();
	collider->AddTriggerEvent(this);
}

void ClientSpace::PlayerRellyPointTrigger::OnTriggerEnter(std::string name)
{
	std::shared_ptr<GameObject> target = SceneManager::GetInstance()->FindObject(name);
	std::shared_ptr<PlayerController> player = target->GetComponent<PlayerController>();

	if (player != nullptr)
	{
		player->SetBool(true);
		collider->Release();
		this->Release();
	}
}