#include "GraphicsPch.h"
#include "PassBase.h"
#include "GraphicsCore/DX11GraphicsCore.h"

namespace GraphicsEngineSpace
{
	ComPtr<ID3D11Device> PassBase::D3DDevice = nullptr;
	ComPtr<ID3D11DeviceContext> PassBase::D3DDeviceContext = nullptr;

	void PassBase::Init()
	{
		D3DDevice = DX11GraphicsCore::GetInstance()->GetDevice();
		D3DDeviceContext = DX11GraphicsCore::GetInstance()->GetImmediateDC();
	}

	void PassBase::Reset()
	{
		// 내부가 nullptr이면 어차피 release를 진행하지 않는다.
		if(D3DDevice)
			D3DDevice->Release();
		if(D3DDeviceContext)
			D3DDeviceContext->Release();
	}
}
