#pragma once
#include "Interface/IDXObject.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief SkyBox 오브젝트입니다.
	 *
	 * View Proj에 따라 움직이려면 따로 오브젝트로 만들어야 하기 때문에 만들었습니다.
	 */
	class SkyBox : public IDXObject
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

		// 안에 들어가는 cb
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
		};

	public:
		GRAPHICSENGINE_DECLSPEC SkyBox();
		~SkyBox() override;

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
	};

}