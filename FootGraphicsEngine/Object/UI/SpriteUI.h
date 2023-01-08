#pragma once

#include "UIBase.h"

namespace GraphicsEngineSpace
{
	class Texture;
	class Canvas;

	// 2D SpriteUI를 화면에 띄우기 위한 클래스
	class SpriteUI : public UIBase
	{
		// 사용할 텍스쳐 정보..리소스 매니저에 생성된 텍스쳐를 사용한다.
		std::shared_ptr<Texture> texture;
		SimpleMath::Vector4 maskColor;

	public:
		SpriteUI();
		~SpriteUI() override;

		virtual void SetTexture(std::shared_ptr<Texture> texture);
		virtual void SetMaskColor(SimpleMath::Vector4 color);
		virtual void SetMaskColor(float r, float g, float b, float a);
		virtual void SetMaskAlpha(float a);
		virtual std::shared_ptr<Texture> GetTexture();

	protected:
		void Render(float tick) override;

		friend Canvas;

	};

}