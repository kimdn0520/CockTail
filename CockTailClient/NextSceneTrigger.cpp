#include "pch.h"
#include "NextSceneTrigger.h"
#include "UIManager.h"

ClientSpace::NextSceneTrigger::NextSceneTrigger(std::weak_ptr<GameObject> _obj)
	: Trigger(_obj)
{

}

ClientSpace::NextSceneTrigger::~NextSceneTrigger()
{

}

void ClientSpace::NextSceneTrigger::Awake()
{
	collider = GetGameObject()->GetComponent<Collider>();
	collider->AddTriggerEvent(this);
}

void ClientSpace::NextSceneTrigger::OnTriggerEnter(std::string name)
{
	std::shared_ptr<GameObject> target = SceneManager::GetInstance()->FindObject(name);
	if (target->GetComponent<Collider>()->GetLayer() == "Player")
		SceneManager::GetInstance()->LoadScene(nextScene);
};