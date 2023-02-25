#include "GamePch.h"
#include "WindowManager.h"

#include "Component/Camera.h"
#include "InputManager.h"
#include "SceneBase.h"
#include "Timer.h"
#include "GraphicsManager.h"
#include "../../FootGraphicsEngine/Manager/ImGUIManager.h"

#include "SceneManager.h"
#include "UIManager.h"

std::shared_ptr<GameEngineSpace::WindowManager> GameEngineSpace::WindowManager::instance = nullptr;

namespace GameEngineSpace
{
	// 윈도우 클래스에 등록하기 위한 목적으로, 전역 콜백함수 등록
	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return WindowManager::GetInstance()->MsgProc(hWnd, message, wParam, lParam);
	}

	LRESULT WindowManager::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (GraphicsEngineSpace::ImGUIManager::GetInstance()->SetWndProcHandler(hWnd, message, wParam, lParam) == true)
			return true;

		// 메시지 프로시저..
			// 일단 가져와보자.
		switch (message)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		// 리사이즈..
		case WM_SIZE:
		{
			// 게임 엔진이 가지고 있는 렌더러에 OnResize를 해주는 식으로 짰었다..
				// 그래픽스 엔진을 스태틱으로 가지고 있을 것인가..게임엔진이 물고 있는 식으로 할 것인가...
				// 윈도우 매니저 내부에서 게임엔진을 가리키는 식으로..
			clientHeight = HIWORD(lParam);
			clientWidth = LOWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				isMinimized = true;
				isMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				isMinimized = false;
				isMaximized = true;
			}
			else if (wParam == SIZE_RESTORED)
			{
				// 최소화 되어 있다 복구
				if (isMinimized == true)
				{
					isMinimized = false;
				}
				else if (isMaximized == true)
				{
					isMaximized = false;
				}

			}

			if (wParam != SIZE_MINIMIZED)
			{
				// 현재 크기 얻어와서 on Resize
				RECT rect;
				GetClientRect(hWnd, &rect);
				this->SetWndSize(rect);
				this->OnResize();

				// 현재 씬의 메인카메라의 비율을 바꿔준다.
				SceneManager::GetInstance()->UpdateMainCameraAspectRatio(clientWidth, clientHeight);
			}
			break;
		}

		// 윈도우창 활성화
		case WM_ACTIVATE:
		{
			if (wParam != false)
				InputManager::GetInstance()->isWindowActivated = true;
			else
				InputManager::GetInstance()->isWindowActivated = false;

			break;
		}

		// 타이머 종료 관련..
		case WM_ENTERSIZEMOVE:
		{
			isResizing = true;

			Timer::GetInstance()->Stop();

			break;
		}
		case WM_EXITSIZEMOVE:
		{
			isResizing = false;

			Timer::GetInstance()->Start();

			// 리사이즈.
			RECT rect;
			GetClientRect(hWnd, &rect);
			this->SetWndSize(rect);
			this->OnResize();

			break;
		}
		case WM_MOUSEMOVE:
		{
			this->OnMouseMove(lParam);

			break;
		}
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	std::shared_ptr<WindowManager> WindowManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<WindowManager>();

		return instance;
	}

	void WindowManager::InitWindow(tstring windowCaption, HINSTANCE hInst, int screenWidth, int screenHeight)
	{
		clientHeight = screenHeight;
		clientWidth = screenWidth;

		const TCHAR* szAppName = windowCaption.c_str();

		HWND _hWnd;

		// 윈도우 클래스 등록
		WNDCLASS wndClass;

		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = MainWndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hInst;
		wndClass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = szAppName;

		RegisterClass(&wndClass);

		_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW, 100, 100,
			screenWidth, screenHeight, NULL, NULL, hInst, NULL);

		if (_hWnd == nullptr)
		{
			assert("Failed to Create Window");

			return;
		}

		// 윈도우 핸들 보관
		hWnd = _hWnd;

		printf("Window Init Clear\n");

		//ImGui초기화
		ImGUIManager::GetInstance()->Init();
		ImGUIManager::GetInstance()->InitImplWin(hWnd);

		printf("IMGUI Window Init Clear\n");
	}

	void WindowManager::StartWindow()
	{
		ShowWindow(hWnd, SW_SHOWNORMAL /* nCmdShow로 조절 가능한 부분.. */);

		UpdateWindow(hWnd);
	}

	void WindowManager::Release()
	{
	}

	INT WindowManager::Update()
	{
		// 메시지 루프
		MSG _msg;

		while (TRUE)
		{
			if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
			{
				if (_msg.message == WM_QUIT) break;
				TranslateMessage(&_msg);
				DispatchMessage(&_msg);
			}
			else
			{
				Timer::GetInstance()->Tick();
				// 각종 매니저 업데이트... => 실제 게임 루프가 일어나는 부분
				InputManager::GetInstance()->Update();
				//UIManager::GetInstance()->Update();
				GraphicsManager::GetInstance()->Update();
				SceneManager::GetInstance()->Update();
				//SoundManager::GetInstance()->Update();

				InputManager::GetInstance()->LateUpdate();

			}
		}

		return static_cast<int>(_msg.wParam);
	}

	void WindowManager::SetWndSize(uint32 width, uint32 height)
	{
		clientWidth = width;
		clientHeight = height;
	}

	void WindowManager::SetWndSize(RECT rect)
	{
		// 큰 거 에서 작은 걸 빼는 겁니다..
		clientWidth = rect.right - rect.left;
		clientHeight = rect.bottom - rect.top;
	}

	void WindowManager::OnResize()
	{
		if (onResize == nullptr)
			return;

		onResize(GetClientWidth(), GetClientHeight());
	}

	void WindowManager::OnMouseMove(LPARAM lParam)
	{
		if (onMouseMove == nullptr)
			return;

		onMouseMove(static_cast<float>(lParam & 0xffff), static_cast<float>((lParam >> (sizeof(WORD) * 8)) & 0xffff));
	}

	void WindowManager::SetOnResizeFunc(std::function<void(uint32, uint32)> onResizeFunc)
	{
		this->onResize = onResizeFunc;
	}

	void WindowManager::SetOnMouseMoveFunc(std::function<void(float, float)> mouseMoveFunc)
	{
		this->onMouseMove = mouseMoveFunc;
	}
}