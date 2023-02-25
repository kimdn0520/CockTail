#include "GraphicsPch.h"
#include "RasterizerState.h"


namespace GraphicsEngineSpace
{
	// 스태틱 변수 초기화
	ComPtr<ID3D11RasterizerState> RasterizerState::wireFrameRS = nullptr;
	ComPtr<ID3D11RasterizerState> RasterizerState::solidRS = nullptr;
	ComPtr<ID3D11RasterizerState> RasterizerState::pointDepthRS = nullptr;
	ComPtr<ID3D11RasterizerState> RasterizerState::spotDepthRS = nullptr;

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

		// 쉐도우를 위한 depth RS
		D3D11_RASTERIZER_DESC depthRsDesc;
		ZeroMemory(&depthRsDesc, sizeof(D3D11_RASTERIZER_DESC));
		depthRsDesc.FillMode = D3D11_FILL_SOLID;
		depthRsDesc.CullMode = D3D11_CULL_NONE;
		depthRsDesc.FrontCounterClockwise = false;
		depthRsDesc.DepthClipEnable = true;
		// 장면 특성 의존 값들 => 현재는 일단 용책을 바탕으로 작업해본다.
		// 고정된 편향치
		depthRsDesc.DepthBias = 1000;
		// 허용되는 최대 깊이 편향치. 피터 팬 결함 방지용
			// 해당 계수가 너무 작으면 그림자 여드름이 두드러 진다.
		depthRsDesc.DepthBiasClamp = 0.0f;
		// 다각형의 기울기 편향치.
		depthRsDesc.SlopeScaledDepthBias = 1.0f;

		HR(pDevice->CreateRasterizerState(&depthRsDesc, pointDepthRS.GetAddressOf()));

		// 고정된 편향치
		depthRsDesc.DepthBias = 100000;
		// 허용되는 최대 깊이 편향치. 피터 팬 결함 방지용
			// 해당 계수가 너무 작으면 그림자 여드름이 두드러 진다.
		depthRsDesc.DepthBiasClamp = 0.0f;
		// 다각형의 기울기 편향치.
		depthRsDesc.SlopeScaledDepthBias = 1.0f;
		HR(pDevice->CreateRasterizerState(&depthRsDesc, spotDepthRS.GetAddressOf()));


		return true;
	}

	void RasterizerState::DestroyAll()
	{
		
	}
}
