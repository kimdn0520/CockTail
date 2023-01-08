#include "GraphicsPch.h"
#include "RasterizerState.h"


namespace GraphicsEngineSpace
{
	// 스태틱 변수 초기화
	ComPtr<ID3D11RasterizerState> RasterizerState::wireFrameRS = nullptr;
	ComPtr<ID3D11RasterizerState> RasterizerState::solidRS = nullptr;

	bool RasterizerState::InitAllRS(ComPtr<ID3D11Device> pDevice)
	{
		// 여긴 별개의 작업이기에 고민의 여지가 별로 없다.
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_NONE;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.DepthClipEnable = true;

		HR(pDevice->CreateRasterizerState(&rsDesc, wireFrameRS.GetAddressOf()));

		rsDesc.FillMode = D3D11_FILL_SOLID;
		HR(pDevice->CreateRasterizerState(&rsDesc, solidRS.GetAddressOf()));

		return true;
	}

	void RasterizerState::DestroyAll()
	{
		if(wireFrameRS)
			wireFrameRS->Release();

		if(solidRS)
			solidRS->Release();
	}
}
