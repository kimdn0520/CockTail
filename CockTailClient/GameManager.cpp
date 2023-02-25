#include "pch.h"
#include "GameManager.h"

// Scene.h
#include "TitleScene.h"
#include "Scenes/DemoScene.h"
#include "Scenes/LobbyScene.h"
#include "Scenes/TutorialScene.h"
#include "Scenes/Room1Scene.h"
#include "Scenes/BarOperationScene.h"

/// <summary>
/// 담당자 : 이재환
/// </summary>
///  
void ClientSpace::GameManager::InitScene()
{
	// 씬들 생성하는 곳 입니다 
	this->sceneManager = GameEngineSpace::SceneManager::GetInstance();

	std::shared_ptr<ClientSpace::TitleScene> titleScene = std::make_shared<ClientSpace::TitleScene>(TEXT("TitleScene"));
	this->sceneManager->AddGameScene(titleScene);

	std::shared_ptr<ClientSpace::DemoScene> demoScene = std::make_shared<ClientSpace::DemoScene>(TEXT("DemoScene"));
	this->sceneManager->AddGameScene(demoScene);

	std::shared_ptr<ClientSpace::LobbyScene> lobbyScene = std::make_shared<ClientSpace::LobbyScene>(TEXT("LobbyScene"));
	this->sceneManager->AddGameScene(lobbyScene);

	std::shared_ptr<ClientSpace::TutorialScene> tutorialScene = std::make_shared<ClientSpace::TutorialScene>(TEXT("TutorialScene"));
	this->sceneManager->AddGameScene(tutorialScene);

	std::shared_ptr<ClientSpace::Room1Scene> room1Scene = std::make_shared<ClientSpace::Room1Scene>(TEXT("Room1Scene"));
	this->sceneManager->AddGameScene(room1Scene);

	std::shared_ptr<ClientSpace::BarOperationScene> barOperationScene = std::make_shared<ClientSpace::BarOperationScene>(TEXT("BarOperationScene"));
	this->sceneManager->AddGameScene(barOperationScene);

	this->sceneManager->LoadScene(TEXT("TitleScene"));
}
