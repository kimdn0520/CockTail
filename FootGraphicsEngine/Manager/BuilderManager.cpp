#include "GraphicsPch.h"
#include "BuilderManager.h"
#include "Interface/IBuilder.h"

// 추가할 빌더들을 include 해준다.
	// TODO : 시간 될 때 묶을 수 있는 것들을 묶어보자
	// 일단 현재는 코드를 옮겨본다.
#include "Builder/LineBuilder.h"
#include "Builder/SkyBoxBuilder.h"
#include "Builder/PBRBuilder.h"
#include "Builder/SpriteEffectBuilder.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<BuilderManger> BuilderManger::instance = nullptr;

	void BuilderManger::InitBuilderAll(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext)
	{
		// 예외처리
		assert(pDevice);
		assert(pDeviceContext);
		assert(RasterizerState::GetSolidRS());
		assert(RasterizerState::GetWireFrameRS());

		// 멤버 변수 초기화.
		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		// 만들어둔 템플릿 함수로 넣어보기
		AddBuilder<LineBuilder>("LineBuilder");
		AddBuilder<SkyBoxBuilder>("SkyBoxBuilder");
		AddBuilder<PBRBuilder>("PBRBuilder");
		AddBuilder<SpriteEffectBuilder>("SpriteEffectBuilder");
	}


	void BuilderManger::DeleteBuildManager()
	{
		// 맵을 순회하면서 모든 정보를 지워준다.
		for (auto _builder : builderMap)
		{
			SafeReset(_builder.second)
		}

		builderMap.clear();

		if (instance != nullptr)
		{
			SafeReset(instance);
		}
	}

	std::shared_ptr<IBuilder> BuilderManger::GetBuilder(std::string _builderName)
	{
		// 키에 맞는 빌더를 찾아서 내보내줌
		return builderMap.at(_builderName);
	}

}
