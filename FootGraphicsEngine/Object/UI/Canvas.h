#pragma once


#include <map>
#include "TextUI.h"
#include "SpriteUI.h"
#include "ButtonUI.h"

namespace GraphicsEngineSpace
{
	// UI가 올라갈 캔버스 클래스
		// 모든 UIBase들은 캔버스 위에 올라간다.
	class Canvas : public UIBase
	{
		// 각종 UI 정보들을 저장해둘 map
		std::map<std::string, std::shared_ptr<TextUI>> textUIMap;
		std::map<std::string, std::shared_ptr<SpriteUI>> spriteUIMap;
		std::map<std::string, std::shared_ptr<ButtonUI>> buttonUIMap;
		std::map<std::string, std::shared_ptr<Canvas>> canvasMap;

		// 기존에 선택된 PickedUI
		std::shared_ptr<UIBase> selectedUI;
		bool selectMode;

	public:
		Canvas(float width, float height);
		~Canvas();

		// 외부에서 사용 가능하도록 virtual.
		virtual std::shared_ptr<TextUI> CreateTextUI(const std::string& name);
		virtual std::shared_ptr<TextUI> GetTextUI(const std::string& name);

		virtual std::shared_ptr<SpriteUI> CreateSpriteUI(const std::string& name);
		virtual std::shared_ptr<SpriteUI> GetSpriteUI(const std::string& name);

		virtual std::shared_ptr<ButtonUI> CreateButtonUI(const std::string& name);
		virtual std::shared_ptr<ButtonUI> GetButtonUI(const std::string& name);

		virtual std::shared_ptr<Canvas> CreateCanvasUI(const std::string& name, float width, float height);
		virtual std::shared_ptr<Canvas> GetCanvasUI(const std::string& name);


		// 버튼 진입 확인 코드
		virtual void CheckCollidedButton(float mouseX, float mouseY, bool isClicked);
		// UISelected 확인 코드
		virtual std::shared_ptr<UIBase> CheckPickedUI(float mouseX, float mouseY, bool isClicked);

		virtual void SetScaleAllCanvas(SimpleMath::Vector2 scale);
		virtual void SetSelectMode(bool selectMode);
		virtual bool GetSelectMode() { return selectMode; }
		virtual std::shared_ptr<UIBase> GetSelectedUI() { return selectedUI; }

		void Render(float tick) override;
		virtual void Release();

	private:
		// 현재 어떤 버튼에서 충돌이 일어났는가.
		std::shared_ptr<ButtonUI> FindCollidedButton(float mouseX, float mouseY);
		std::shared_ptr<UIBase> FindPickedUI(float mouseX, float mouseY);
	};

}
