#pragma once

#include <functional>
#include "UIBase.h"
#include "Resources/Texture.h"

namespace GraphicsEngineSpace
{
	class Canvas;

	enum class ButtonState
	{
		DEFAULT,	// 기본 상태
		DOWN,		// 누른 그 시점
		UP,			// 뗀 그 시점
		PRESS,		// 누르고 있는 상태
		HOVER,		// 올려두고 있는 상태
		ENTER,		// 가장 처음 진입한 상태(hover 직전, 한 번만 나와야하는 이벤트)
		DISABLE		// 모든 선택에 영향을 받지 않는 상태 (bool 값으로 하지 않고 ButtonState로 설정)
	};

	// 기본적으로 Sprite UI와 유사하지만, 각종 이벤트들을 functor로 가지고 있는 버튼 UI
	class ButtonUI : public UIBase
	{
		std::shared_ptr<Texture> defaultTexture;
		SimpleMath::Vector4 defaultMaskColor;
		std::shared_ptr<Texture> hoverTexture;
		SimpleMath::Vector4 hoverMaskColor;
		std::shared_ptr<Texture> pressTexture;
		SimpleMath::Vector4 pressMaskColor;
		ButtonState state;

		// 아무 값도 반환하지 않고, 매개 변수도 필요 없는 함수 포인터
		std::function<void()> clickEvent;	// 버튼을 눌렸을 때
		std::function<void()> pressEvent;	// 버튼을 누르고 있을 때
		std::function<void()> enterEvent;	// 버튼에 진입했을 때
		std::function<void()> hoverEvent;	// 버튼을 누르지 않고 위에 띄워 둘 때
		std::function<void()> outEvent;		// 버튼에서 벗어났을 때 (hover에서 버튼 영역 밖으로 나갈 때)

	public:
		ButtonUI();
		~ButtonUI() override;

		virtual void SetDefaultTexture(std::shared_ptr<Texture> texture);
		virtual void SetDefaultMaskColor(SimpleMath::Vector4 color) { defaultMaskColor = color; }
		virtual void SetDefaultMaskColor(float r, float g, float b, float a);
		virtual void SetHoverTexture(std::shared_ptr<Texture> texture);
		virtual void SetHoverMaskColor(SimpleMath::Vector4 color) { hoverMaskColor = color; }
		virtual void SetHoverMaskColor(float r, float g, float b, float a);
		virtual void SetPressTexture(std::shared_ptr<Texture> texture);
		virtual void SetPressMaskColor(SimpleMath::Vector4 color) { pressMaskColor = color; }
		virtual void SetPressMaskColor(float r, float g, float b, float a);
		virtual void SetButtonState(ButtonState state);

		virtual void SetButtonEnable(bool enable);

		virtual void SetClickEvent(std::function<void()> clickEvent);
		virtual void SetPressEvent(std::function<void()> pressEvent);
		virtual void SetEnterEvent(std::function<void()> enterEvent);
		virtual void SetHoverEvent(std::function<void()> hoverEvent);
		virtual void SetOutEvent(std::function<void()> outEvent);

		virtual ButtonState GetButtonState();

	protected:
		void Render(std::shared_ptr<IRenderer> renderer, float tick) override;

		friend Canvas;

	};
}