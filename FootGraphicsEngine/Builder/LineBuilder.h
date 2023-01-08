#pragma once
#include "Interface/IBuilder.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	* \brief PosColor 오브젝트들을 만들어주는 PosColor Builder
	*
	* 2022.07.13(수) Dev.BlackFoot
	*/
	class LineBuilder : public IBuilder
	{
		// 내부적으로 Device를 가지고 있어야 한다.
			// 멤버 함수가 여러개가 되면서 여러 부분에서 사용하는 각종 멤버 변수가 필요해졌다.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// 사용해줄 리소스 매니저.
		std::shared_ptr<ResourceManager> resourceManager;
		

	public:
		LineBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{}
		~LineBuilder() override {}

		std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		//std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName, std::string textureName, std::wstring path) override;
		std::shared_ptr<MeshResources> BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath) override;
		void InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC) override;

	private:
		// 하드코딩으로 넣어줄 그리드, 축
		std::shared_ptr<MeshResources> BuildGridResources(std::shared_ptr<MeshResources> _objRes);
		std::shared_ptr<MeshResources> BuildAxisResources(std::shared_ptr<MeshResources> _objRes);

	};

}
