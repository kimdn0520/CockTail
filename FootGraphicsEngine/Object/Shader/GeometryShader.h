#pragma once

#include "ShaderBase.h"

// 지오메트리 쉐이더와 관련된 요소들이 모여있는 클래스
namespace GraphicsEngineSpace
{

	class GeometryShader : public ShaderBase
	{
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3D11GeometryShader> shader;

	public:
		GeometryShader(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext, ComPtr<ID3DBlob> shaderBlob);
		~GeometryShader() override;

		bool SetUpShader() override;

		Shader* GetShader() override;

	private:
		bool CreateShader(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob);
	};

}
