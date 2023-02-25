#include "GraphicsPch.h"
#include "DepthStencilResource.h"

namespace GraphicsEngineSpace
{
	DepthStencilResource::DepthStencilResource()
		: depthStencilBuffer(nullptr)
		, depthStencilView(nullptr)
		, depthStencilState(nullptr)
		, screenViewport(nullptr)
	{
	}

	DepthStencilResource::~DepthStencilResource()
	{
	}

	void DepthStencilResource::Finalize()
	{
		SafeReset(screenViewport)
	}

	// 뷰를 보내봤더니 r-value로 취급 되었기 때문에 장치들을 받아서 설정해주는 것으로 해보자..
		// 정확하게는 CreateRenderTarget 부분.
	void DepthStencilResource::CreateDepthStencil(ComPtr<ID3D11Device> _device, ComPtr<ID3D11DeviceContext> _immediateDC, int _clientWidth, int _clientHeight)
	{
		// 여기 있는 매개 변수가 제대로 생성되었는지 확인.
		assert(_device);
		assert(_immediateDC);

		// 깊이 스텐실 버퍼(텍스쳐) 생성
		D3D11_TEXTURE2D_DESC _depthStencilDesc;
		_depthStencilDesc.Width = _clientWidth;
		_depthStencilDesc.Height = _clientHeight;
		_depthStencilDesc.MipLevels = 1;
		_depthStencilDesc.ArraySize = 1;
		// 깊이 뷰에 사용할 Format과 Shader Resource에서 사용할 Format이 다르기 때문에 (해석이 다르기 때문에)
			// TYPELESS로 설정해줍니다.
			// 일단 현재는 깊이를 사용하는 부분이 main Depth와 Shadow 밖에 없기에 하드 코딩이지만
			// 이후에 필요가 생기면 멤버 변수로 밖으로 빼봅시다..
		_depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		_depthStencilDesc.SampleDesc.Count = 1;
		_depthStencilDesc.SampleDesc.Quality = 0;

		_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		// Shadow Map을위한 Texture를 뽑아내기 위해 바인드 플래그를 추가합니다.
		_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		_depthStencilDesc.CPUAccessFlags = 0;
		_depthStencilDesc.MiscFlags = 0;

		HR(_device->CreateTexture2D(&_depthStencilDesc, 0, depthStencilBuffer.GetAddressOf()));

		// 깊이 스텐실 뷰 생성
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, depthStencilView.GetAddressOf()));

		// State 생성
		D3D11_DEPTH_STENCIL_DESC descDSS = {};
		ZeroMemory(&descDSS, sizeof(D3D11_DEPTH_STENCIL_DESC));

		descDSS.DepthEnable = true;
		descDSS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		descDSS.StencilEnable = false;
		descDSS.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		descDSS.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		descDSS.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device->CreateDepthStencilState(&descDSS, depthStencilState.GetAddressOf()));

		// shader Resource 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = _depthStencilDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		HR(_device->CreateShaderResourceView(depthStencilBuffer.Get(), &srvDesc, shaderResource.GetAddressOf()));

		SafeReset(screenViewport);

		screenViewport = std::make_shared<D3D11_VIEWPORT>();

		// Screen Viewport 세팅
		screenViewport->TopLeftX = 0.0f;
		screenViewport->TopLeftY = 0.0f;
		screenViewport->Width = static_cast<float>(_clientWidth);
		screenViewport->Height = static_cast<float>(_clientHeight);
		screenViewport->MinDepth = 0.0f;
		screenViewport->MaxDepth = 1.0f;
	}

	void DepthStencilResource::CreateDepthStencilCube(ComPtr<ID3D11Device> _device,
		ComPtr<ID3D11DeviceContext> _immediateDC, int _clientWidth, int _clientHeight, int cubeMapCnt)
	{
		assert(_device);
		assert(_immediateDC);

		// 깊이 스텐실 버퍼를 큐브맵 형식으로 만들어 본다..
		D3D11_TEXTURE2D_DESC _depthStencilCubeDesc;
		_depthStencilCubeDesc.Width = _clientWidth;
		_depthStencilCubeDesc.Height = _clientHeight;
		_depthStencilCubeDesc.MipLevels = 1;

		if (cubeMapCnt != 0)
			_depthStencilCubeDesc.ArraySize = 6 * cubeMapCnt;
		else
			_depthStencilCubeDesc.ArraySize = 6;
		// 깊이 뷰에 사용할 Format과 Shader Resource에서 사용할 Format이 다르기 때문에 (해석이 다르기 때문에)
			// TYPELESS로 설정해줍니다.
			// 일단 현재는 깊이를 사용하는 부분이 main Depth와 Shadow 밖에 없기에 하드 코딩이지만
			// 이후에 필요가 생기면 멤버 변수로 밖으로 빼봅시다..
		_depthStencilCubeDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		_depthStencilCubeDesc.SampleDesc.Count = 1;
		_depthStencilCubeDesc.SampleDesc.Quality = 0;

		_depthStencilCubeDesc.Usage = D3D11_USAGE_DEFAULT;
		// Shadow Map을위한 Texture를 뽑아내기 위해 바인드 플래그를 추가합니다.
		_depthStencilCubeDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		_depthStencilCubeDesc.CPUAccessFlags = 0;
		// 해당 플래그를 Texture Cube로 만들어봅니다..
		_depthStencilCubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		HR(_device->CreateTexture2D(&_depthStencilCubeDesc, 0, depthStencilBuffer.GetAddressOf()));

		// 깊이 스텐실 뷰 생성
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		// 이부분이 큐브맵이기 때문에 Texture2DArray로 만들어줍니다.
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		// 큐브맵 생성이기 때문에 Texture2D Array의 해당 내용을 모두 입력해 줍니다.
			// 그렇지 않으면 오류가 발생합니다.
		dsvDesc.Texture2DArray.MipSlice = 0;
		dsvDesc.Texture2DArray.ArraySize = 6;
		if (cubeMapCnt != 0)
		{
			for (int i = 0; i < cubeMapCnt; i++)
			{

				dsvDesc.Texture2DArray.FirstArraySlice = i * 6;
				ComPtr<ID3D11DepthStencilView> tmpDepthStencil;

				HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, tmpDepthStencil.GetAddressOf()));

				depthStencilArray.push_back(tmpDepthStencil);
			}
		}
		else
		{
			dsvDesc.Texture2DArray.FirstArraySlice = 0;		

			HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, depthStencilView.GetAddressOf()));
		}

		// State 생성
		D3D11_DEPTH_STENCIL_DESC descDSS = {};
		ZeroMemory(&descDSS, sizeof(D3D11_DEPTH_STENCIL_DESC));

		descDSS.DepthEnable = true;
		descDSS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		descDSS.StencilEnable = false;
		descDSS.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		descDSS.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		descDSS.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device->CreateDepthStencilState(&descDSS, depthStencilState.GetAddressOf()));

		// shader Resource 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		if (cubeMapCnt != 0)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			// 큐브맵 생성이기 때문에 Texture2D Array의 해당 내용을 모두 입력해 줍니다.
			srvDesc.TextureCubeArray.First2DArrayFace = 0;
			srvDesc.TextureCubeArray.NumCubes = cubeMapCnt;
			srvDesc.TextureCubeArray.MipLevels = _depthStencilCubeDesc.MipLevels;
			srvDesc.TextureCubeArray.MostDetailedMip = 0;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.Texture2D.MipLevels = _depthStencilCubeDesc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
		}
		HR(_device->CreateShaderResourceView(depthStencilBuffer.Get(), &srvDesc, shaderResource.GetAddressOf()));

		SafeReset(screenViewport);

		screenViewport = std::make_shared<D3D11_VIEWPORT>();

		// Screen Viewport 세팅
		screenViewport->TopLeftX = 0.0f;
		screenViewport->TopLeftY = 0.0f;
		screenViewport->Width = static_cast<float>(_clientWidth);
		screenViewport->Height = static_cast<float>(_clientHeight);
		screenViewport->MinDepth = 0.0f;
		screenViewport->MaxDepth = 1.0f;
	}

	void DepthStencilResource::CreateDepthStencilArray(ComPtr<ID3D11Device> _device,
		ComPtr<ID3D11DeviceContext> _immediateDC, int _clientWidth, int _clientHeight, int arrayCnt)
	{
		// 2D Texture를 Array로 관리하면서 Spot Light가 바뀔 때 마다
		// Shader Resource View Array에서 꺼내 쓰는 방식으로..?

		// Light Pass에는 리소스가 하나만 들어가도록 해주면 될 것 같다.
		// 여기 있는 매개 변수가 제대로 생성되었는지 확인.
		assert(_device);
		assert(_immediateDC);

		// 깊이 스텐실 버퍼(텍스쳐) 생성
		D3D11_TEXTURE2D_DESC _depthStencilDesc;
		_depthStencilDesc.Width = _clientWidth;
		_depthStencilDesc.Height = _clientHeight;
		_depthStencilDesc.MipLevels = 1;

		// 해당 부분을 spotLight 개수에 맞게 변경해줍니다.
		if (arrayCnt != 0)
			_depthStencilDesc.ArraySize = arrayCnt;
		else
			_depthStencilDesc.ArraySize = 1;
		// 깊이 뷰에 사용할 Format과 Shader Resource에서 사용할 Format이 다르기 때문에 (해석이 다르기 때문에)
			// TYPELESS로 설정해줍니다.
			// 일단 현재는 깊이를 사용하는 부분이 main Depth와 Shadow 밖에 없기에 하드 코딩이지만
			// 이후에 필요가 생기면 멤버 변수로 밖으로 빼봅시다..
		_depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		_depthStencilDesc.SampleDesc.Count = 1;
		_depthStencilDesc.SampleDesc.Quality = 0;

		_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		// Shadow Map을위한 Texture를 뽑아내기 위해 바인드 플래그를 추가합니다.
		_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		_depthStencilDesc.CPUAccessFlags = 0;
		_depthStencilDesc.MiscFlags = 0;

		HR(_device->CreateTexture2D(&_depthStencilDesc, 0, depthStencilBuffer.GetAddressOf()));
		
		// 깊이 스텐실 뷰 생성
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		// 큐브맵이 아니지만 Array로 만들어봅시다..
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsvDesc.Texture2DArray.MipSlice = 0;
		// Array Size는 Array를 이루는 Texture 하나의 개수..
		dsvDesc.Texture2DArray.ArraySize = 1;

		if(arrayCnt != 0)
		{
			for (int i = 0; i < arrayCnt; i++)
			{
				dsvDesc.Texture2DArray.FirstArraySlice = i;
				ComPtr<ID3D11DepthStencilView> tmpDepthStencil;

				HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, tmpDepthStencil.GetAddressOf()));

				depthStencilArray.push_back(tmpDepthStencil);
			}
		}
		else
		{
			// 없을 때는 하나만 생성해줍니다.
			dsvDesc.Texture2DArray.FirstArraySlice = 0;

			HR(_device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, depthStencilView.GetAddressOf()));
		}

		// State 생성
		D3D11_DEPTH_STENCIL_DESC descDSS = {};
		ZeroMemory(&descDSS, sizeof(D3D11_DEPTH_STENCIL_DESC));

		descDSS.DepthEnable = true;
		descDSS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSS.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		descDSS.StencilEnable = false;
		descDSS.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		descDSS.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		descDSS.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSS.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device->CreateDepthStencilState(&descDSS, depthStencilState.GetAddressOf()));

		// shader Resource 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		// Array로 생성해봅시다.
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		// MultiSampling은 하지 않는다고 가정합니다.
		srvDesc.Texture2DArray.MipLevels = _depthStencilDesc.MipLevels;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		if(arrayCnt != 0)
			srvDesc.Texture2DArray.ArraySize = arrayCnt;
		else
			srvDesc.Texture2DArray.ArraySize = 1;

		HR(_device->CreateShaderResourceView(depthStencilBuffer.Get(), &srvDesc, shaderResource.GetAddressOf()));

		SafeReset(screenViewport);

		screenViewport = std::make_shared<D3D11_VIEWPORT>();

		// Screen Viewport 세팅
		screenViewport->TopLeftX = 0.0f;
		screenViewport->TopLeftY = 0.0f;
		screenViewport->Width = static_cast<float>(_clientWidth);
		screenViewport->Height = static_cast<float>(_clientHeight);
		screenViewport->MinDepth = 0.0f;
		screenViewport->MaxDepth = 1.0f;
	}

	// 기존의 렌더 타겟을 지워주는 함수.
	void DepthStencilResource::DeleteImmediateDepthStencil()
	{
		// 사실 기능상으로는 Finalize와 다를 바가 없다..
			// 이후에 내부에 멤버 변수가 많아지면 또 달라질 수도..
			// 여기서는 화면이 변경될 때 바꿔주는 각종 렌더 타겟들만 지워준다.
		if (depthStencilView)
			depthStencilView = nullptr;
		if (depthStencilBuffer)
			depthStencilBuffer = nullptr;
		if (depthStencilState)
			depthStencilState = nullptr;

		if(shaderResource)
			shaderResource = nullptr;

		// 기존 Array 제거.
		depthStencilArray.clear();
	}

}
