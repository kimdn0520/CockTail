#pragma once

#include "UIBase.h"

namespace GraphicsEngineSpace
{

	class Texture;
	class Canvas;

	// 차는 방향 ~쪽으로 차오른다는 의미.
	enum class FillDir
	{
		UP,		// 위로 참
		DOWN,	// 아래로 참
		LEFT,	// 왼쪽으로 참
		RIGHT	// 오른쪽으로 참
	};

	/// 2D Texture중 Progress Bar를 화면에 띄우기 위한 클래스
	///	기본적으로 BackBar를 그리고 그 위에 ProgressBar를 그린다.
	///	ProgressBar는 Bar가 차는 방향, 몇 % 찼는지 등의 정보를 가지고 있다.
	class ProgressBarUI : public UIBase
	{
		// 사용할 텍스쳐 정보
		std::shared_ptr<Texture> backgroundTexture;
		std::shared_ptr<Texture> barTexture;

		// 각 텍스쳐의 크기 정보 (Texture Wipe)를 위한 정보
			// Sprite Batch Draw의 Source에 들어간다..
			// progressBar 텍스쳐의 특정 부분만을 드러낸다고 봐도 무방하다..
			// 기본적으로 Background는 모두 그려지고 Progress Bar의 texture만 관리해주면 된다.
		RECT barSrcRect;

		FillDir direction;
		float fillPercent;

	public:
		ProgressBarUI();
		~ProgressBarUI() override;

		virtual void SetBackgroundTexture(std::shared_ptr<Texture> texture);
		virtual void SetBarTexture(std::shared_ptr<Texture> texture);

		virtual float GetFillPercent() { return fillPercent; }
		virtual void SetFillPercent(float percent);
		virtual void SetFillDirection(FillDir direction);

		// BarTexture의 left, top, right, bottom을 잡습니다.
		// => 해당 수치에 percent를 곱해서 texture를 자릅니다.
		virtual void SetBarTexRect(long left, long top, long width, long height);

	protected:
		void Render(std::shared_ptr<IRenderer> renderer, float tick) override;

		friend Canvas;

	};
}

