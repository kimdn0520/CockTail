#pragma once
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	/**
	 * \brief 클라이언트 영역에 존재하던 윈도우 관련 함수, 변수들을 래핑한 클래스
	 *
	 * 2022.07.28(목)
	 */
	class WindowManager final
	{
		static std::shared_ptr<WindowManager> instance;

		// 창 사이즈 조절에 필요한 변수
		bool isMinimized;
		bool isMaximized;
		bool isResizing;

		// 창 크기 변수
		uint32 clientWidth;
		uint32 clientHeight;

		// 윈도우 핸들
		HWND hWnd;

		// 창 변경 함수.
		std::function<void(uint32, uint32)> onResize;
		std::function<void(float, float)> onMouseMove;

	public:
		WindowManager() = default;
		~WindowManager() = default;

		WindowManager(const WindowManager & other) = delete;
		WindowManager& operator=(const WindowManager & other) = delete;
		WindowManager(const WindowManager && other) = delete;
		WindowManager& operator=(const WindowManager && other) = delete;

		// 메시지 프로시져 함수
		LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		// 내부 변수 제거
		void Release();

		// 인스턴스 겟
		static std::shared_ptr<WindowManager> GetInstance();

		// 윈도우 초기화
		void InitWindow(tstring windowCaption, HINSTANCE hInst, int screenWidth, int screenHeight);

		// 윈도우 화면에 표시
		void StartWindow();

		// Tick..
		INT Update();

		// 각종 게터, 세터
		HWND GetHWND() { return hWnd; }
		uint32 GetClientWidth() { return clientWidth; }
		uint32 GetClientHeight() { return clientHeight; }

		void SetWndSize(uint32 width, uint32 height);
		void SetWndSize(RECT rect);
		// OnResize
		void OnResize();
		void OnMouseMove(LPARAM lParam);

		// 펑터 세팅
		void SetOnResizeFunc(std::function<void(uint32, uint32)> onResizeFunc);
		void SetOnMouseMoveFunc(std::function<void(float, float)> mouseMoveFunc);
	};
}

