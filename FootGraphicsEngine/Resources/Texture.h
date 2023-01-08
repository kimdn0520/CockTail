#pragma once
#include "Resource.h"

namespace GraphicsEngineSpace
{
	class Texture : public Resource
	{
	private:
		ComPtr<ID3D11ShaderResourceView> mapSRV;

		ScratchImage image;

		ComPtr<ID3D11Texture2D> texResource;

		DXGI_FORMAT format;

		unsigned int levels;

		unsigned int width;

		unsigned int height;

	public:
		GRAPHICSENGINE_DECLSPEC Texture()
			: Resource(RESOURCE_TYPE::TEXTURE)
			, mapSRV(nullptr)
			, image(ScratchImage())
			, texResource(nullptr)
		{};

		~Texture() {};

	public:
		ComPtr<ID3D11ShaderResourceView> GetMapSRV() { return mapSRV; }

		virtual void Load(const std::wstring& path) override;

		void Release();
	};
}