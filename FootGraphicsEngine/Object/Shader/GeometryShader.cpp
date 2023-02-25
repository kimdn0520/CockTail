#include "GraphicsPch.h"
#include "GeometryShader.h"

namespace GraphicsEngineSpace
{
	GeometryShader::GeometryShader(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext,
		ComPtr<ID3DBlob> shaderBlob)
		: ShaderBase(ShaderType::GEOMETRY)
		, deviceContext(_deviceContext)
		, shader(nullptr)
	{
		if(CreateShader(_device, shaderBlob) != true)
		{
			assert(0);
		}
	}

	GeometryShader::~GeometryShader()
	{
	}

	bool GeometryShader::SetUpShader()
	{
		deviceContext->GSSetShader(shader.Get(), nullptr, 0);

		return true;
	}

	Shader* GeometryShader::GetShader()
	{
		return reinterpret_cast<Shader*>(shader.Get());
	}

	bool GeometryShader::CreateShader(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob)
	{
		HRESULT hr = _device->CreateGeometryShader
		(
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			nullptr,
			shader.GetAddressOf()
		);

		return SUCCEEDED(hr);
	}
}
