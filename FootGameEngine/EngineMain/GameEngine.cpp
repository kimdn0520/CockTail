#include "GamePch.h"
#include "GameEngine.h"

#include "InputManager.h"
#include "Timer.h"
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "NavigationManager.h"

namespace GameEngineSpace
{
	std::shared_ptr<GameEngine> GameEngine::gameEngineInstance = nullptr;

	void GameEngine::Init(tstring gameTitle, HINSTANCE hInst, int screenWidth, int screenHeight)
	{

		// 모든 매니저를 Init 합니다.
		WindowManager::GetInstance()->InitWindow(gameTitle, hInst, screenWidth, screenHeight);
		Timer::GetInstance()->Init();
		InputManager::GetInstance();
		GraphicsManager::GetInstance()->Init(WindowManager::GetInstance()->GetHWND(), screenWidth, screenHeight);
		UIManager::GetInstance()->Init();
		SceneManager::GetInstance()->Init();
		NavigationManager::GetInstance();

		// 모든 매니저의 Init이 끝난뒤.. 윈도우를 띄워줍니다.
		WindowManager::GetInstance()->StartWindow();

		// 텍스쳐를 제대로 불러오려면, 윈도우가 켜지고 불러와야합니다 (WIC 텍스쳐의 특징)
		GraphicsManager::GetInstance()->LateInit();
		UIManager::GetInstance()->CreateCanvasAll();
	}

	INT GameEngine::Tick()
	{
		// 매 틱 돌때마다..
			// 윈도우 매니저 내부의 update를 돌립니다
		return WindowManager::GetInstance()->Update();
	}

	void GameEngine::Release()
	{
		Timer::GetInstance()->Release();
		SceneManager::GetInstance()->Release();
		NavigationManager::GetInstance()->Release();
		GraphicsManager::GetInstance()->Release();
		WindowManager::GetInstance()->Release();
	}
}
