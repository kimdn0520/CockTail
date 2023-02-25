#pragma once

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;

	/**
	 * \brief 게임에서 생성될 모든 UI를 만들어주는 역할을 하는 클래스입니다.
	 * 여기서 만들어진 캔버스들은 모든 UIManager의 mainCanvas안에 존재하게 될 것이므로
	 * 클래스를 지역 변수로 선언해도 무방합니다.
	 *
	 * 사용할 캔버스 자체는 UI Manager의 Init에서 생성됩니다.
	 * 이 클래스에서는 생성된 캔버스 안에 만들어질 모든 스프라이트 및 텍스트를 생성합니다.
	 *
	 * 게임이 시작 되고, UI Manager를 Init할 때 호출됩니다.
	 */
	class UICreator
	{
	private:
		std::shared_ptr<Canvas> mainCanvas;

	public:
		void CreateCanvasAll();

	private:
		// 실제로 생성할 캔버스들
		void CreateTitleCanvas();
		void CreateGameHUD();
		void CreateSettingCanvas();
		void CreateInMenuCanvas();
		void CreateCombinationCanvas();
		void CreateDialogCanvas();
		void CreateTestSceneCanvas();
	};

}
