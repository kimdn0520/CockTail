#include "GraphicsPch.h"
#include "Texture.h"
#include "GraphicsCore/DX11GraphicsCore.h"

namespace GraphicsEngineSpace
{
	void Texture::Load(const std::wstring& path)
	{
		std::wstring ext = fs::path(path).extension();

		if (ext == L".dds" || ext == L".DDS")
			::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
		else if (ext == L".tga" || ext == L".TGA")
			::LoadFromTGAFile(path.c_str(), nullptr, image);
		else if (ext == L".hdr" || ext == L".HDR")
			::LoadFromHDRFile(path.c_str(), nullptr, image);
		else // png, jpg, jpeg, bmp
			::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);

		HRESULT hr = ::CreateTexture(DX11GraphicsCore::GetInstance()->GetDevice().Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			reinterpret_cast<ID3D11Resource**>(texResource.GetAddressOf()));

		if (FAILED(hr))
			MessageBox(0, TEXT("Texture Failed."), 0, 0);

		bool isCubeMap = image.GetMetadata().IsCubemap();

		D3D11_TEXTURE2D_DESC textureDesc;

		texResource->GetDesc(&textureDesc);

		width = textureDesc.Width;

		height = textureDesc.Height;

		levels = textureDesc.MipLevels;

		format = textureDesc.Format;

		// 레벨 부여
		//if (_levels <= 0)
			//_levels = Texture::NumMipmapLevels(_width, _height);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (!isCubeMap)
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		else
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

		hr = DX11GraphicsCore::GetInstance()->GetDevice()->CreateShaderResourceView(texResource.Get(), &srvDesc, mapSRV.GetAddressOf());

		if (FAILED(hr))
			MessageBox(0, TEXT("Texture Failed."), 0, 0);

		// Mip Level이 만약 0이면 .. Mip Map 형성해주자.
		/*if (textureDesc.MipLevels == 0)
			Graphics_Interface::Get()->GetDeviceClass()->GetDeviceContext()->GenerateMips(this->mapSRV.Get());*/
	}

	void Texture::Release()
	{
		texResource.ReleaseAndGetAddressOf();

		mapSRV.ReleaseAndGetAddressOf();
	}
}