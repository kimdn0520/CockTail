#pragma once

// 텍스트 UI를 화면에 띄우기 위해..

#include <string>
#include "UIBase.h"

namespace GraphicsEngineSpace
{
	class Canvas;

	class TextUI : public UIBase
	{
		std::wstring text;
		std::string font;
		// 알파값까지 한 color
		SimpleMath::Vector4 color;
		float fontSize;

	public:
		TextUI();
		~TextUI() override;
		virtual void SetText(const std::string& text);
		virtual void SetText(const std::wstring& text);
		virtual void SetColor(const SimpleMath::Vector4& color);
		virtual void SetFont(const std::string& fontName);
		virtual void SetFontSize(float fontSize);

		virtual std::string GetText();

	protected:
		void Render(float tick) override;

		friend Canvas;
	};
}
