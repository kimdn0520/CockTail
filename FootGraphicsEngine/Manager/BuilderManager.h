#pragma once
#include <map>

#include "Interface/IBuilder.h"
#include "GraphicsCore/RasterizerState.h"

/// Dll 매크로
#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	/**
	 * \brief 외부에서 빌더들을 사용하기 위해 빌더를 관리할 싱글톤 매니저
	 *
	 * 렌더러(그래픽스 엔진) 내부에서 빌더를 사용하는 것이 아닌, 게임 엔진 혹은 클라이언트 위치에서 빌더를 사용해야하기 때문에
	 * 각종 함수들을 사용할 수 있도록 외부로 뽑아내준 클래스이다.
	 * 해당 클래스에서 디바이스, DC, RS 등을 초기화 하고, 사용할 때 뽑아준다.
	 *
	 * 2022.07.12(화) Dev.BlackFoot
	 */
	class BuilderManger
	{
	private:
		// 외부에서 받아줄 인스턴스
		static std::shared_ptr<BuilderManger> instance;

		// 빌더를 관리할 맵
		std::map<std::string, std::shared_ptr<IBuilder>> builderMap;

		// 게임엔진 or 클라이언트에서 빌더를 추가한다고 했을 때.. Device와 DC등을 가져올 수 없기 때문에 그걸 여기서 가지고 있는다.
		// 그게 아니라면 빌더가 추가 될 때마다 이 클래스에 수동으로 빌더를 추가해주는 방법이 있다.(하드 코딩)
			// 현재(22.07.12) Effect 클래스나 InputLayout 클래스가 그렇게 되어있다.
		// 일단은 여기서 DC등을 가지고 있고, 맵에 빌더를 추가할 때 넣어주는 방식으로 한다.
		ComPtr<ID3D11Device> D3DDevice = nullptr;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext = nullptr;
	public:
		// 그래픽스 엔진 안에서 사용할 함수
		// 자주 사용하는 빌더들 한번에 Init
		void InitBuilderAll(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext);
		// 새로운 빌더를 만들어서 추가할 때..
			// T 라는 빌더를 상속 받은 빌더를 추가해준다.
		template <typename T>
		void AddBuilder(std::string _builderName);

		// 그래픽스 엔진 외부에서 사용 가능한 함수들.
		GRAPHICSENGINE_DECLSPEC static std::shared_ptr<BuilderManger> GetInstance()
		{
			if (instance == nullptr)
			{
				instance = std::make_shared<BuilderManger>();
			}

			return instance;
		}
		GRAPHICSENGINE_DECLSPEC void DeleteBuildManager();

		// 특정 빌더를 뽑아내기
		GRAPHICSENGINE_DECLSPEC std::shared_ptr<IBuilder> GetBuilder(std::string _builderName);
	};

	template <typename T>
	void BuilderManger::AddBuilder(std::string _builderName)
	{
		// 예외처리
		assert(D3DDevice);
		assert(D3DDeviceContext);
		assert(RasterizerState::GetSolidRS());
		assert(RasterizerState::GetWireFrameRS());

		// 생성자의 시그니쳐를 일치시킨다.
		std::shared_ptr<IBuilder> _tmpBuilder = std::make_shared<T>();

		// 빌더 초기화
			// 모든 빌더는 Init이 구현 되어있어야한다.
		_tmpBuilder->InitBuilder(D3DDevice, D3DDeviceContext);

		// 해당 빌더를 추가시킨다.
		builderMap.insert({ _builderName, _tmpBuilder });
	}
}
