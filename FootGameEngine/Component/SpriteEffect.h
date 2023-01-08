#pragma once
#include "RendererBase.h"
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{

	// 2D 스프라이트 이펙트를 뽑아내기 위한 렌더러 컴포넌트 입니다.
	class SpriteEffect : public RendererBase
	{
	public:
		SpriteEffect(std::weak_ptr<GameObject> object);
		virtual ~SpriteEffect() {}


	public:
		// 여기서 초기화 할 때 이름을 넣습니다
		void Init(std::string objName);

		void Render() override;
		// Update
		void PrepareRender(float tick) override;

		void DebugIMGUIRender(float tick) override;
	};

}
