#pragma once
#include "RendererBase.h"
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	// 이 역시 Mesh 렌더러로 나올 수 있다.
		// 현재 내 구조가 특정 쉐이더를 사용하는 특정 빌더를 가져와서 만드는 구조기 때문에 이렇게 하는 것..
	class MeshRenderer : public RendererBase
	{
	public:
		MeshRenderer(std::weak_ptr<GameObject> object);

		virtual ~MeshRenderer() {}

	public:
		// 빌더에 필요한 Init 함수.. 가변인자 템플릿을 사용하자.
		void Init(std::string objName);

		void Render() override;
		void PrepareRender(float tick) override;

		void PlayAnim(std::string animCilpName, bool isLoop);
	};
}