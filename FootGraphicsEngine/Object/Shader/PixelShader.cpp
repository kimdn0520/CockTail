#include "GraphicsPch.h"
#include "PixelShader.h"

namespace GraphicsEngineSpace
{
	PixelShader::PixelShader(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext, ComPtr<ID3DBlob> shaderBlob)
		: ShaderBase(ShaderType::PIXEL)
		, deviceContext(_deviceContext)
		, shader(nullptr)
	{
		if(CreateShader(_device, shaderBlob) != true)
		{
			assert(0);
		}
	}

	PixelShader::~PixelShader()
	{
	}

	bool PixelShader::SetUpShader()
	{
		deviceContext->PSSetShader(shader.Get(), nullptr, 0);

		return true;
	}

	Shader* PixelShader::GetShader()
	{
		return reinterpret_cast<Shader*>(shader.Get());
	}

	bool PixelShader::CreateShader(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob)
	{
		HRESULT hr = _device->CreatePixelShader
		(
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			nullptr,
			shader.GetAddressOf()
		);

		return SUCCEEDED(hr);
	}
}
