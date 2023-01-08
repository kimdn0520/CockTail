#include "GraphicsPch.h"
#include "VertexShader.h"

namespace GraphicsEngineSpace
{
	VertexShader::VertexShader(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext, ComPtr<ID3DBlob> shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize)
		: ShaderBase(ShaderType::VERTEX)
		, deviceContext(_deviceContext)
		, shader(nullptr)
		, layout(nullptr)
	{
		if (CreateShader(_device, shaderBlob) != true)
		{
			// 예외 처리
			assert(0);
		}

		if (CreateLayout(_device, shaderBlob, desc, elementsSize) != true)
		{
			assert(0);
		}

	}

	VertexShader::~VertexShader()
	{
		/*if(shader != nullptr)
			shader->Release();

		if(layout != nullptr)
			layout->Release();*/
	}

	bool VertexShader::SetUpShader()
	{
		deviceContext->VSSetShader(shader.Get(), nullptr, 0);
		// 여기에서 inputLayout까지 세팅해줍니다.
		deviceContext->IASetInputLayout(layout.Get());

		return true;
	}

	Shader* VertexShader::GetShader()
	{
		return reinterpret_cast<Shader*>(shader.Get());
	}

	bool VertexShader::CreateLayout(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize)
	{
		HRESULT hr = _device->CreateInputLayout
		(
			desc,
			elementsSize,
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			layout.GetAddressOf()
		);

		return SUCCEEDED(hr);
	}

	bool VertexShader::CreateShader(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob)
	{
		HRESULT hr = _device->CreateVertexShader
		(
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			nullptr,
			shader.GetAddressOf()
		);

		return SUCCEEDED(hr);
	}
}
