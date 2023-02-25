#include "pch.h"

#include "EngineMain/GameEngine.h"
#include "SceneManager.h"
#include "GameManager.h"

#include <format>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
		
	if (AllocConsole())
	{
		FILE* fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
	}

		
	std::shared_ptr<GameEngine> gameEngine = GameEngine::GetEngineInstance();
	gameEngine->Init(TEXT("CockTailPrototype"), hInstance, 1920, 1080);

	// 틱들어가기 전에
	//std::shared_ptr<ClientSpace::DemoScene> demo = std::make_shared<ClientSpace::DemoScene>(TEXT("Demo"));
	/*SceneManager::GetInstance()->AddGameScene(demo);
	SceneManager::GetInstance()->LoadScene(TEXT("Demo"));*/

	/*std::shared_ptr<ClientSpace::TitleScene> title = std::make_shared<ClientSpace::TitleScene>(TEXT("Title"));
	SceneManager::GetInstance()->AddGameScene(title);
	SceneManager::GetInstance()->LoadScene(TEXT("Title"));*/

	ClientSpace::GameManager gameManager;
	gameManager.InitScene();

	gameEngine->Tick();

	gameEngine->Release();

	FreeConsole();

	return 0;
}
