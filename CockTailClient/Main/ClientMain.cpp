#include "pch.h"

#include "EngineMain/GameEngine.h"
#include "SceneManager.h"
#include "Scenes/DemoScene.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::shared_ptr<GameEngine> gameEngine = GameEngine::GetEngineInstance();
	gameEngine->Init(TEXT("CockTailPrototype"), hInstance, 1000, 800);

	// 틱들어가기 전에
	std::shared_ptr<ClientSpace::DemoScene> demo = std::make_shared<ClientSpace::DemoScene>(TEXT("Demo"));
	SceneManager::GetInstance()->AddGameScene(demo);
	SceneManager::GetInstance()->LoadScene(TEXT("Demo"));

	gameEngine->Tick();

	gameEngine->Release();

	return 0;
}
