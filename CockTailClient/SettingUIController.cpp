#include "pch.h"
#include "SettingUIController.h"
#include "UIManager.h"

using namespace GameEngineSpace;
using namespace GraphicsEngineSpace;

ClientSpace::SettingUIController::SettingUIController(std::weak_ptr<GameObject> obj)
	:ComponentBase(obj)
{
	mainCanvas = UIManager::GetInstance()->GetMainCanvas();
}

ClientSpace::SettingUIController::~SettingUIController()
{
}

void ClientSpace::SettingUIController::Awake()
{
}

void ClientSpace::SettingUIController::Start()
{
}

void ClientSpace::SettingUIController::Update(float tick)
{
}

void ClientSpace::SettingUIController::LateUpdate(float tick)
{
}
