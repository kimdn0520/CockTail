#pragma once
#include "ShaderBase.h"

// 버텍스 쉐이더와 관련된 요소들이 모여있는 클래스
namespace GraphicsEngineSpace
{
	class VertexShader : public ShaderBase
	{
		// 버텍스에는 inputLayout 필요.
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3D11VertexShader> shader;
		ComPtr<ID3D11InputLayout> layout;

	public:
		VertexShader(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext, ComPtr<ID3DBlob> shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize);
		~VertexShader() override;

		bool SetUpShader() override;

		Shader* GetShader() override;

	private:
		// inputLayoutClass 에서 가져다 쓰기
		bool CreateLayout(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob, D3D11_INPUT_ELEMENT_DESC* desc, UINT elementsSize);
		bool CreateShader(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob);
	};
}