#pragma once
#include "Interface/IBuilder.h"

#include "FBXBinaryLayout.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief PBR 쉐이더를 이용해서 빛 연산을 처리하는 PBR object를 위한 빌더 입니다.
	 */
	class PBRBuilder : public IBuilder
	{
		// 내부적으로 Device를 가지고 있어야 한다.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		std::shared_ptr<ResourceManager> resourceManager;

	public:
		PBRBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{
			
		}
		~PBRBuilder() override;

		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		virtual std::shared_ptr<MeshResources> BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath) override;

		virtual void InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC) override;

	private:
		// 자체 포맷에서 가져온 정적 mesh의 Resources 정보를 추가해줍니다.
		void BuildStaticMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData);
		void BuildSkinnedMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData);

	};

}