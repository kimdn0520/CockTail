#include "pch.h"
#include "InGameUIController.h"
#include "UIManager.h"
#include "SceneManager.h"

using namespace GameEngineSpace;
using namespace GraphicsEngineSpace;


/// <summary>
/// / 담당 : 이재환 
/// 설명 : 인게임의 모든 UI들을 컨트롤 합니다
/// </summary>
namespace ClientSpace
{
	ClientSpace::InGameUIController::InGameUIController(std::weak_ptr<GameObject> obj)
		:ComponentBase(obj)
	{

	}

	ClientSpace::InGameUIController::~InGameUIController()
	{

	}

	void ClientSpace::InGameUIController::Awake()
	{
		mainCanvas = UIManager::GetInstance()->GetMainCanvas();
	}

	void ClientSpace::InGameUIController::Start()
	{
		SceneCheck();
	}

	void ClientSpace::InGameUIController::Update(float tick)
	{
		
		if (InputManager::GetInstance()->GetInputState(VK_ESCAPE, KeyState::DOWN) == true)
		{
			if (mainCanvas->GetCanvasUI("InGameMenuCanvas")->GetEnable() == true)
			{
				mainCanvas->GetCanvasUI("InGameMenuCanvas")->SetEnable(false);
			}
			else if (mainCanvas->GetCanvasUI("InGameMenuCanvas")->GetEnable() == false)
			{
				mainCanvas->GetCanvasUI("InGameMenuCanvas")->SetEnable(true);
			}
		}
		
	}

	void ClientSpace::InGameUIController::LateUpdate(float tick)
	{
		
		auto hpBar = mainCanvas->GetProgressBarUI("HpBar");
		auto bossHpBar = mainCanvas->GetProgressBarUI("bossHpBar");

		if (hpBar != nullptr)
		{
			hpBar->SetFillPercent(static_cast<float>(currentHP) / static_cast<float>(maxHP));
		}

		if (bossHpBar!= nullptr)
		{
			bossHpBar->GetEnable() == true;
			bossHpBar->SetFillPercent(static_cast<float>(currentBossHP) / static_cast<float>(bossMaxHP));
		}

	}

	void InGameUIController::SetHP(int HP)
	{
		if (HP > maxHP)
		{
			currentHP = maxHP;
		}
		else
		{
			currentHP = HP;
		}
	}

	void InGameUIController::SetMaxHP(int maxHP)
	{
		this->maxHP = maxHP;
	}

	void InGameUIController::SetBossHP(int HP)
	{
		if (HP > maxHP)
		{
			currentBossHP = bossMaxHP;
		}
		else
		{
			currentBossHP = HP;
		}
	}

	void InGameUIController::SetBossMaxHP(int maxHP)
	{
		this->maxHP = bossMaxHP;
	}

	void InGameUIController::BarEvent(int tickCount)
	{
		// 바 개점시 랜덤으로 손님 텍스쳐를 보여줘야 합니다 
		// 손님 등장은 알파값을 조정해서 페이드 인 아웃 효과 

	}

	void InGameUIController::SetCustomer()
	{

	}

	void InGameUIController::FadeIn()
	{



	}

	void InGameUIController::FadeOut()
	{
	}

	void InGameUIController::CombineItem()
	{
		
	}

	void InGameUIController::SceneCheck()
	{
		auto scene = GameEngineSpace::SceneManager::GetInstance()->GetCurrentScene();

		if (scene->GetSceneName() == L"DemoScene" || scene->GetSceneName() == L"TutorialScene" || scene->GetSceneName() == L"Room1Scene")
		{
			mainCanvas->GetCanvasUI("InGameCanvas")->SetEnable(true);
			mainCanvas->GetCanvasUI("SceneTestCanvas")->SetEnable(true);

		}
		else if (scene->GetSceneName() == L"LobbyScene")
		{
			mainCanvas->GetCanvasUI("InGameCanvas")->SetEnable(false);
			mainCanvas->GetCanvasUI("SceneTestCanvas")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->SetEnable(false);
		}
		else if (scene->GetSceneName() == L"BarOperationScene")
		{
			mainCanvas->GetCanvasUI("CombinationCanvas")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CombinationBackGround")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CombinationBoxBackGround")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("StuffResultTextrue")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("ReQuest")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("Customer1")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CharactersDialog")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetSpriteUI("CashierMachine")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("RumButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("VodkaButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("GinButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("LiquorButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("JuiceButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SodaButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("FruitButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton2")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("StuffButton3")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("BeerMaChineButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("CocktailrecipeButton")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("MixButton")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("ReSetButton")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetButtonUI("SoldOutButton")->SetEnable(true);

			mainCanvas->GetCanvasUI("CombinationCanvas")->GetTextUI("ReQuestText")->SetEnable(true);
			mainCanvas->GetCanvasUI("CombinationCanvas")->GetTextUI("GoldText")->SetEnable(true);
		}
	}


}