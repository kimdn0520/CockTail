#include "pch.h"
#include "TitleMenuUIController.h"
#include "UIManager.h"

using namespace GameEngineSpace;
using namespace GraphicsEngineSpace;

namespace ClientSpace
{

	ClientSpace::TitleMenuUIController::TitleMenuUIController(std::weak_ptr<GameObject> obj)
		:ComponentBase(obj)
	{
		mainCanvas = UIManager::GetInstance()->GetCanvas("TitleCanvas");

	}

	ClientSpace::TitleMenuUIController::~TitleMenuUIController()
	{
	}

	void ClientSpace::TitleMenuUIController::Awake()
	{
		mainCanvas->SetEnable(true);
	}

	void ClientSpace::TitleMenuUIController::Start()
	{
		//// 나중에 사운드 
		//auto ending = UIManager::GetInstance()->GetCanvas("EndingCanvas");
		//ending->SetEnable(false);
	}

	void ClientSpace::TitleMenuUIController::Update(float tick)
	{
		
	}

	void ClientSpace::TitleMenuUIController::LateUpdate(float tick)
	{
	}
}

