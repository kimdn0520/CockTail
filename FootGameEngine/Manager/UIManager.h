#pragma once

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

class GraphicsEngineSpace::ImGUIManager;

namespace GameEngineSpace
{
	class GraphicsManager;
	class InputManager;

	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;

	/**
	 * \brief UI, 텍스트 등 화면에 2차원으로 존재하는 UI 들만을 렌더링 하는 UI Manager 클래스
	 *
	 * 2022.08.08(월) Deb.BlackFoot
	 */

	class UIManager final
	{
	private:
		// UI Manager의 인스턴스
		static std::shared_ptr<UIManager> instance;

		// 내부적으로 그래픽스 매니저 캐싱
		std::shared_ptr<GraphicsManager> graphicsInstance;
		std::shared_ptr<InputManager> inputInstance;

		// 캔버스
			// 게임에 있는 모든 UI 캔버스들을 init시 만들 수 있지 않을까.
			// UI로 렌더되는 모든 녀석들을 처음 Init 할 때 만들고 enable을 false로 지정해주고
			// 특정 상황에 필요할 때 해당 UI 아이디를 가져와서 만들어준다.
		std::shared_ptr<Canvas> mainCanvas;
		// IMGui 캐싱용
		std::shared_ptr<GraphicsEngineSpace::ImGUIManager> imgui;

		// 화면 정보를 파악하고. 스케일을 늘려보자..
		uint32 nowWidth;
		uint32 nowHeight;

	public:
		UIManager() = default;
		~UIManager() = default;
		UIManager(const UIManager& other) = delete;
		UIManager(const UIManager&& other) = delete;
		UIManager& operator= (const UIManager& other) = delete;
		UIManager& operator= (const UIManager&& other) = delete;

		static std::shared_ptr<UIManager> GetInstance();

		void Init();

		// WIC 이슈를 피하기 위한 Canvas 생성
		void CreateCanvasAll();

		// 렌더
		void Render(float tick);

		// 버튼 입력을 위한 업데이트와 LateUpdate
		void Update();
		void LateUpdate();

		void Release();

		std::shared_ptr<Canvas> CreateCanvas(const std::string& UIIDstr);

		// 특정 캔버스를 얻고 싶을 때
		std::shared_ptr<Canvas> GetCanvas(const std::string& canvasIDStr);
		std::shared_ptr<Canvas> GetMainCanvas() { return mainCanvas; }

		// 캔버스 리사이즈
		void OnResize(uint32 width, uint32 height);

		void ClearUI();

		// ImGUI Render
		void SelectGUIRender();

	private:
		void CreateTestCanvas();
		void CreatePopUpCanvase();
	};

}
