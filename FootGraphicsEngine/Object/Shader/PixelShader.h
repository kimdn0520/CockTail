#pragma once

#include "ShaderBase.h"

// 픽셀 쉐이더와 관련된 요소들이 모여있는 클래스
namespace GraphicsEngineSpace
{
	class PixelShader : public ShaderBase
	{
		// 픽셀 셰이더 필요.
		ComPtr<ID3D11DeviceContext> deviceContext;
		ComPtr<ID3D11PixelShader> shader;
		
	public:
		PixelShader(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _deviceContext, ComPtr<ID3DBlob> shaderBlob);
		~PixelShader() override;

		bool SetUpShader() override;

		// 포인터 주소값만을 전달하는 용도이기 때문에 원시포인터를 사용.
		Shader* GetShader() override;

	private:
		bool CreateShader(ComPtr<ID3D11Device> _device, ComPtr<ID3DBlob> shaderBlob);
	};
}