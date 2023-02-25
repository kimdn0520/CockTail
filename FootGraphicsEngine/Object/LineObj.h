#pragma once
#include "Interface/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	* \brief Grid, Axis등, 위치와 컬러 값만 가지는 기본적인 Pos ColorObj에 관한 오브젝트 클래스
	*
	* 2022.07.13(수) Dev.BlackFoot
	*/
	class LineObj : public IDXObject
	{
		// 멤버 변수 목록
		// 디바이스를 가지고 있자.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// 각종 정보가 포함된 리소스
		std::vector<std::shared_ptr<MeshResources>> meshResources;

		// 캐싱용 리소스 매니저 변수
		std::shared_ptr<ResourceManager> resourceManager;

		// TM 행렬 => 트랜스 폼..?
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;

		// 버텍스에 Color 정보가 들어있는지에 대한 여부.
		bool hasColor;
		// Line에 들어갈 색 정보를 담아두는 변수.
		SimpleMath::Color lineColor;

		// 안에 들어가는 cb
		__declspec(align(16)) struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
		};

	public:
		// 생성자는 Export
		GRAPHICSENGINE_DECLSPEC LineObj();
		~LineObj() override;

		virtual bool Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext) override;
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;
		virtual void PreRender(float tick) override {}
		virtual void Render() override;
		virtual void SetMeshResources(std::shared_ptr<MeshResources> objResources) override;
		virtual std::string GetObjName() override;
		virtual std::vector<std::shared_ptr<MeshResources>> GetMeshResources() override;

		virtual const SimpleMath::Matrix& GetWorld() override { return world; }
		virtual const SimpleMath::Matrix& GetView() override { return view; }
		virtual const SimpleMath::Matrix& GetProj() override { return proj; }

		// dynamic 캐스팅을 이용하여 color를 세팅해준다.
		void SetHasColor(bool hasColor) { this->hasColor = hasColor; }
	};
}
