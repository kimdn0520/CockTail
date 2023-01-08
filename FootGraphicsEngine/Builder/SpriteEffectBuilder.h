#pragma once
#include "Interface/IBuilder.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;

	/**
	 * \brief Effect Object들을 만들어주는 EffectBuilder
	 *
	 * 2023.01.05(목) Dev.BlackFoot
	 */
	class SpriteEffectBuilder : public IBuilder
	{
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		std::shared_ptr<ResourceManager> resourceManager;

	public:
		SpriteEffectBuilder()
			: D3DDevice(nullptr)
			, D3DDeviceContext(nullptr)
			, resourceManager(nullptr)
		{
			
		}
		virtual ~SpriteEffectBuilder() override {}

		virtual std::shared_ptr<IDXObject> BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName) override;
		virtual std::shared_ptr<MeshResources> BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath) override;
		virtual void InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC) override;

	private:
		void BuildDefaultEffect(std::shared_ptr<IDXObject> DXObject, std::shared_ptr<MeshResources> tmpResource);
		void BuildBinaryEffect(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData);
	};

}
