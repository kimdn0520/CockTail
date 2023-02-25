#include "pch.h"
#include "InteractionTrigger.h"
#include "InputManager.h"

ClientSpace::InteractionTrigger::InteractionTrigger(std::weak_ptr<GameObject> _obj)
	: Trigger(_obj)
	, isInteracted(false)
	, isSelected(false)
{
	collider = GetGameObject()->GetComponent<Collider>();
	input = GameEngineSpace::InputManager::GetInstance();
}

ClientSpace::InteractionTrigger::~InteractionTrigger()
{

}

void ClientSpace::InteractionTrigger::Awake()
{
	collider->AddTriggerEvent(this);
}

void ClientSpace::InteractionTrigger::Update(float tick)
{

}

void ClientSpace::InteractionTrigger::OnTriggerEnter(std::string name)
{
	std::shared_ptr<GameObject> target = SceneManager::GetInstance()->FindObject(name);
	
	if (target->GetComponent<Collider>()->GetLayer() == "Player")
		isSelected = true;
}

void ClientSpace::InteractionTrigger::OnTriggerExit(std::string name)
{
	std::shared_ptr<GameObject> target = SceneManager::GetInstance()->FindObject(name);

	if (target->GetComponent<Collider>()->GetLayer() == "Player")
		isSelected = false;
}

void ClientSpace::InteractionTrigger::InputUpdate()
{
	if (isSelected == false)
		return;

	if (input->GetInputState('F', KeyState::DOWN))
	{
		isInteracted = true;
	}
}
