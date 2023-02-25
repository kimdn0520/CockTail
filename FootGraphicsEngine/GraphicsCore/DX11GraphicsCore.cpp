#include "GraphicsPch.h"
#include "DX11GraphicsCore.h"
#include "RasterizerState.h"
#include "RenderTargetTexture.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<DX11GraphicsCore> DX11GraphicsCore::instance = nullptr;

	DX11GraphicsCore::DX11GraphicsCore()
		: D3DDevice(nullptr)
		, D3DImmediateContext(nullptr)
		, swapChain(nullptr)
		, _4xMSAAQuality(0)
		, enable4xMSAA(false)
		, screenWidth(0)
		, screenHeight(0)
	{

	}

	DX11GraphicsCore::~DX11GraphicsCore()
	{
		if (D3DImmediateContext)
			D3DImmediateContext->ClearState();
	}

	std::shared_ptr<DX11GraphicsCore> DX11GraphicsCore::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<DX11GraphicsCore>();

		return instance;
	}

	bool DX11GraphicsCore::Initialize(HWND _hWnd, int _clientWidth, int _clientHeight)
	{
		// _hWnd는 가져온다..
		// Device와 DC를 만들어봅시다..
		UINT _createDeviceFlags = 0;

		// 디버그면..
#if defined(DEBUG) || defined(_DEBUG)
		_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL _featureLevel;
		HRESULT hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0,
			_createDeviceFlags, 0, 0, D3D11_SDK_VERSION,
			D3DDevice.GetAddressOf(), &_featureLevel, D3DImmediateContext.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}

		// 버전 확인.
		// 버전 확인. 여기서 다른게 나오면 배열을 하나 생성해서 추가해야함.
		if (_featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(_hWnd, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		// 4X MSAA 품질 수준 점검
		HR(D3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMSAAQuality));
		assert(_4xMSAAQuality > 0);

		// SwapChain 생성.

		// DXGI_SWAP_CHAIN_DESC 구조체 생성
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = _clientWidth;
		sd.BufferDesc.Height = _clientHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		if (enable4xMSAA)
		{
			sd.SampleDesc.Count = 4;

			sd.SampleDesc.Quality = _4xMSAAQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = _hWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		IDXGIDevice* _dxgiDevice = 0;
		HR(D3DDevice->QueryInterface(__uuidof(IDXGIDevice),
			(void**)(&_dxgiDevice)));

		IDXGIAdapter* _dxgiAdapter = 0;
		HR(_dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
			(void**)&_dxgiAdapter));

		IDXGIFactory* _dxgiFactory = 0;
		HR(_dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
			(void**)&_dxgiFactory));

		HR(_dxgiFactory->CreateSwapChain(D3DDevice.Get(), &sd, swapChain.GetAddressOf()));

		ReleaseCOM(_dxgiDevice);
		ReleaseCOM(_dxgiAdapter);
		ReleaseCOM(_dxgiFactory);

		// 이후 OnResize를 거친 뒤..
			// 여기서 Device, DC 등이 무슨 일을 하는 지 봐야함.
			// 일단은.. 새로 버퍼를 생성하는 과정에서 swapChain과 Device 등 여기 있는 게 다 사용되긴 하는데...
			// 문제는 여기에 각종 뷰를 어떻게 들고 올 수 있을 것인가.
			// 기본적으로 여기서 뷰와 버퍼들을 생성하게 된다..
				// 생각해보면 여기에 존재하는 View들은 마지막으로 화면에 그려주는 View들이기 때문에 그냥 멤버변수로 가지고 있어도..?
				// 응집성이 떨어질 것 같다.
			// 밖에서 인자로 받아서 onresize를 하는 방법도.. 있을거 같긴한데 의미가 없을 거 같고..

		// 렌더 스테이트 생성
		// 원래 렌더 스테이트를 생성하는 곳에서 생성해주자..
		RasterizerState::InitAllRS(D3DDevice);

		screenWidth = _clientWidth;
		screenHeight = _clientHeight;

		// 여기까지 하면 성공
		return true;
	}

	void DX11GraphicsCore::Finalize()
	{
		// 각종 친구들 해제.
		if (D3DImmediateContext)
			D3DImmediateContext->ClearState();

		SafeReset(instance);
	}

	void DX11GraphicsCore::ResetRenderTargetView(ComPtr<ID3D11RenderTargetView> nowRTV, const FLOAT color[4])
	{
		assert(D3DImmediateContext);
		assert(swapChain);

		if (nowRTV == nullptr)
			return;

		D3DImmediateContext->ClearRenderTargetView(nowRTV.Get(), color);
	}

	void DX11GraphicsCore::ResetDepthStencilView(ComPtr<ID3D11DepthStencilView> nowDSV, UINT clearFlag, float depth, UINT8 stencil)
	{
		assert(D3DImmediateContext);
		assert(swapChain);

		if (nowDSV == nullptr)
			return;

		D3DImmediateContext->ClearDepthStencilView(nowDSV.Get(), clearFlag, depth, stencil);
	}

	void DX11GraphicsCore::ResetRS()
	{
		D3DImmediateContext->RSSetState(0);
	}

	void DX11GraphicsCore::PresentSwapChain()
	{
		assert(swapChain);

		HR(swapChain->Present(1, 0));
	}

	void DX11GraphicsCore::CreateMainRenderTarget(std::shared_ptr<RenderTargetTexture> mainRenderTarget,
		int clientWidth, int clientHeight, DXGI_FORMAT bufferFormat)
	{
		assert(swapChain);
		HRESULT hr;

		// 받아온 RenderTargetTexture에 backbuffer Texture를 붙여준다.
			// 추가로 SRV도 만들어본다.
		HR(swapChain->ResizeBuffers(1, clientWidth, clientHeight, bufferFormat, 0));

		// 임시 BackBuffer Texture
		ID3D11Texture2D* backBuffer;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer));

		if (backBuffer == nullptr)
		{
			::MessageBoxA(nullptr, "BackBuffer Get Failed!", nullptr, MB_OK);
			return;

		}

		hr = D3DDevice->CreateRenderTargetView(backBuffer, 0, mainRenderTarget->renderTarget.GetAddressOf());

		if (FAILED(hr) == true)
		{
			::MessageBoxA(nullptr, "RTV Create Failed!", nullptr, MB_OK);
			return;
		}

		ReleaseCOM(backBuffer);
	}
}
