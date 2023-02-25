#include "GraphicsPch.h"
#include "Renderer.h"

#include <cassert>
#include <d3dcompiler.h>
#include "Interface/IDXObject.h"
#include "GraphicsCore/DX11GraphicsCore.h"
#include "GraphicsCore/DepthStencilResource.h"
#include "GraphicsCore/RenderTargetTexture.h"
#include "GraphicsCore/RasterizerState.h"

#include "Resources/InputLayout.h"
#include "Manager/BuilderManager.h"
#include "Factory/Factory.h"

#include "Manager/ShaderManager.h"
#include "Manager/BufferManager.h"
#include "Manager/SamplerManager.h"
#include "Manager/UIUtilsManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/ImGUIManager.h"

#include "Object/UI/Canvas.h"
#include "Object/UI/TextUI.h"
#include "Object/UI/SpriteUI.h"
#include "Object/UI/ButtonUI.h"

#include "Pass/LightPass.h"
#include "Pass/ShadowPass.h"
#include "Pass/PostProcess.h"
#include "Object/PBRObj.h"

namespace GraphicsEngineSpace
{
	// 각종 오브젝트..

	Renderer::Renderer()
		: hWnd(0)
		, graphicsCore(nullptr)
		, mainRenderTarget(nullptr)
		, mainDepthStencil(nullptr)
		, DMRAORenderTarget(nullptr)
		, normalRenderTarget(nullptr)
		, albedoRenderTarget(nullptr)
		, worldPosRenderTarget(nullptr)
		, emissiveRenderTarget(nullptr)
		, lightPass(nullptr)
		, shadowPass(nullptr)
		, postProcessPass(nullptr)
		, blendState(nullptr)
		, annotation(nullptr)
		, annotationCount(0)
		, spriteBatch(nullptr)
		, deltaTime(0.0f)
		, camPos(Vector3::Zero)
		, minimized(false)
		, maximized(false)
		, clientWidth(0)
		, clientHeight(0)
	{

	}

	Renderer::~Renderer()
	{
	}

	bool Renderer::Initialize(HWND _hWnd, int _clientWidth, int _clientHeight)
	{
		// 일단 멤버 변수 변경
		hWnd = _hWnd;
		clientWidth = _clientWidth;
		clientHeight = _clientHeight;

		graphicsCore = DX11GraphicsCore::GetInstance();
		// 각종 디바이스 등 초기화.
		graphicsCore->Initialize(hWnd, clientWidth, clientHeight);

		mainRenderTarget = std::make_shared<RenderTargetTexture>();
		mainDepthStencil = std::make_shared<DepthStencilResource>();
		DMRAORenderTarget = std::make_shared<RenderTargetTexture>();
		normalRenderTarget = std::make_shared<RenderTargetTexture>();
		albedoRenderTarget = std::make_shared<RenderTargetTexture>();
		worldPosRenderTarget = std::make_shared<RenderTargetTexture>();
		emissiveRenderTarget = std::make_shared<RenderTargetTexture>();

		ComPtr<ID3D11Device> device = graphicsCore->GetDevice();
		ComPtr<ID3D11DeviceContext> deviceContext = graphicsCore->GetImmediateDC();

		// 각종 Effect 등 Static 클래스들 모두 초기화.
		RasterizerState::InitAllRS(device);
		ShaderManager::GetInstance()->Init(device, deviceContext);
		ShaderManager::GetInstance()->CompileAllShader();
		BufferManager::GetInstance()->Init(device, deviceContext);
		SamplerManager::GetInstance()->Init(device);
		// 팩토리 인스턴스 생성 => dll 외부에서 생성하기 전에 내부에서 생성해준다.
		Factory::GetInstance();

		// 빌더 매니저 생성 및 Init => 디바이스를 받기 때문에 렌더러에서 Init을 해주어야한다.
		BuilderManger::GetInstance()->InitBuilderAll(graphicsCore->GetDevice(), graphicsCore->GetImmediateDC());

		lightPass = std::make_unique<LightPass>();
		lightPass->Init();
		lightPass->Start(mainRenderTarget, mainDepthStencil);

		shadowPass = std::make_unique<ShadowPass>();
		shadowPass->Init();
		shadowPass->Start(mainRenderTarget, mainDepthStencil);

		postProcessPass = std::make_unique<PostProcess>();
		postProcessPass->Init();
		postProcessPass->Start(mainRenderTarget, mainDepthStencil);

		OnResize();

		gBuffer.push_back(albedoRenderTarget);
		gBuffer.push_back(DMRAORenderTarget);
		gBuffer.push_back(normalRenderTarget);
		gBuffer.push_back(worldPosRenderTarget);
		gBuffer.push_back(emissiveRenderTarget);

		// BlendState 세팅
		D3D11_BLEND_DESC blendDesc = {};
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		HRESULT hr = graphicsCore->GetDevice()->CreateBlendState(&blendDesc, blendState.GetAddressOf());
		if (FAILED(hr))
			return false;

		// 그래픽스 디버거 얻어오기
		graphicsCore->GetImmediateDC()->QueryInterface<ID3DUserDefinedAnnotation>(annotation.GetAddressOf());

		// 스프라이트 생성
		spriteBatch = std::make_shared<DirectX::SpriteBatch>(deviceContext.Get());

		// imgui 초기화
		ImGUIManager::GetInstance()->InitImplDX11(device.Get(), deviceContext.Get());


		// 여기까지 하면 성공
		return true;
	}

	bool Renderer::LateInitialize()
	{
		auto device = graphicsCore->GetDevice();
		ResourceManager::GetInstance()->Initialize();
		UIUtilsManager::GetInstance()->Init(device, spriteBatch, mainDepthStencil->GetDepthStencilState(), blendState);

		return true;
	}

	void Renderer::Finalize()
	{
		Factory::GetInstance()->DeleteFactory();
		ImGUIManager::GetInstance()->ShutDownImplDX11();

		BuilderManger::GetInstance()->DeleteBuildManager();

		RasterizerState::DestroyAll();

		// 각종 매니저 Finalize
		ShaderManager::GetInstance()->Finalize();
		BufferManager::GetInstance()->Finalize();
		SamplerManager::GetInstance()->Release();
		UIUtilsManager::GetInstance()->Finalize();
		ResourceManager::GetInstance()->Release();

		// 각종 COM 포인터를 Release 한다.
		mainRenderTarget->Finalize();

		graphicsCore->Finalize();

		mainRenderTarget->Finalize();
		mainDepthStencil->Finalize();
		DMRAORenderTarget->Finalize();
		normalRenderTarget->Finalize();
		albedoRenderTarget->Finalize();
		worldPosRenderTarget->Finalize();
		emissiveRenderTarget->Finalize();

		lightPass->Release();
		shadowPass->Release();
		postProcessPass->Release();

		// 그리고포인터 변수를 지워준다.
		SafeReset(mainRenderTarget);
		SafeReset(mainDepthStencil);
		SafeReset(DMRAORenderTarget);
		SafeReset(normalRenderTarget);
		SafeReset(albedoRenderTarget);
		SafeReset(worldPosRenderTarget);
		SafeReset(emissiveRenderTarget);

		SafeReset(lightPass);
	}

	void Renderer::OnResize()
	{
		ComPtr<ID3D11Device> device = graphicsCore->GetDevice();
		ComPtr<ID3D11DeviceContext> deviceContext = graphicsCore->GetImmediateDC();

		// 기존의 것을 지워주고..
		mainRenderTarget->Finalize();
		// 새로 만들어준다.
		graphicsCore->CreateMainRenderTarget(mainRenderTarget, clientWidth, clientHeight, DXGI_FORMAT_R8G8B8A8_UNORM);

		DMRAORenderTarget->Finalize();
		DMRAORenderTarget->Init(device, clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		normalRenderTarget->Finalize();
		normalRenderTarget->Init(device, clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		albedoRenderTarget->Finalize();
		albedoRenderTarget->Init(device, clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		worldPosRenderTarget->Finalize();
		worldPosRenderTarget->Init(device, clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		emissiveRenderTarget->Finalize();
		emissiveRenderTarget->Init(device, clientWidth, clientHeight, graphicsCore->GetMSAAQuality());

		mainDepthStencil->DeleteImmediateDepthStencil();
		mainDepthStencil->CreateDepthStencil(device, deviceContext, clientWidth, clientHeight);

		lightPass->OnResize(clientWidth, clientHeight);
		shadowPass->OnResize(clientWidth, clientHeight);
		postProcessPass->OnResize(clientWidth, clientHeight);

		// 뷰포트 설정.
		deviceContext->RSSetViewports(1, mainDepthStencil->GetViewport().get());

		// core의 Width Height 최신화.
		graphicsCore->SetScreenWidth(clientWidth);
		graphicsCore->SetScreenHeight(clientHeight);
	}

	float Renderer::GetAspectRatio() const
	{
		return static_cast<float>(clientWidth) / clientHeight;
	}

	bool Renderer::IsVaildDevice()
	{
		return (graphicsCore->GetDevice() != nullptr);
	}

	void Renderer::SetClientSize(int _width, int _height)
	{
		clientWidth = _width;
		clientHeight = _height;
	}

	void Renderer::SetWinMinMax(bool _isMinimized, bool _isMaximized)
	{
		minimized = _isMinimized;
		maximized = _isMaximized;
	}

	/*void Renderer::AddRenderObj(std::shared_ptr<IDXObject> obj)
	{
		renderVector.push_back(obj);
	}*/

	/*void Renderer::InitObject()
	{
		for (auto obj : renderVector)
		{
			obj->Init(graphicsCore->GetDevice(), graphicsCore->GetImmediateDC());
		}
	}*/

	void Renderer::InitObject(std::shared_ptr<IDXObject> obj)
	{
		obj->Init(graphicsCore->GetDevice(), graphicsCore->GetImmediateDC());
	}

	/*void Renderer::ClearRenderVector()
	{
		for (auto renderObj : renderVector)
			renderObj.reset();

		renderVector.clear();
	}*/

	std::shared_ptr<Canvas> Renderer::CreateCanvas(const std::string& name, float width, float height)
	{
		std::shared_ptr<Canvas> newCanvas = std::make_shared<Canvas>(width, height);

		if (newCanvas == nullptr)
			return nullptr;

		newCanvas->SetName(name);

		return newCanvas;
	}

	void Renderer::GraphicsDebugBeginEvent(const std::string& name)
	{
		if (annotation == nullptr)
			return;

		std::wstring wstr;

		wstr.assign(name.begin(), name.end());

		annotation->BeginEvent(wstr.c_str());

		annotationCount++;
	}

	void Renderer::GraphicsDebugEndEvent()
	{
		if (annotation == nullptr || annotationCount <= 0)
			return;

		annotation->EndEvent();
		annotationCount--;
	}

	void Renderer::SetCameraPos(float posX, float posY, float posZ)
	{
		camPos.x = posX;
		camPos.y = posY;
		camPos.z = posZ;
	}

	void Renderer::PassDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj)
	{
		lightPass->SetDirectionalLight(color, direction, power, lightViewProj);
		shadowPass->SetLightViewProj(lightViewProj);
	}

	void Renderer::PassPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, bool isShadow, std::vector<SimpleMath::Matrix> lightViewProj)
	{
		lightPass->SetPointLight(color, position, power, range, isShadow, lightViewProj);
	}

	void Renderer::PassSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, SimpleMath::Vector3 direction, float power, float innerSpotAngle, float outerSpotAngle, float range, bool
	                             isShadow, SimpleMath::Matrix lightViewProj)
	{
		lightPass->SetSpotLight(color, position, direction, power, innerSpotAngle, outerSpotAngle, range, isShadow, lightViewProj);
	}

	void Renderer::PassAmbientSkyColor(SimpleMath::Vector4 color)
	{
		lightPass->SetAmbientSkyColor(color);
	}

	void Renderer::ResetShadowPass()
	{
		shadowPass->ResetIsSetPointLightCnt();
		shadowPass->ResetIsSetSpotLightCnt();
	}

	void Renderer::BeginRender()
	{
		graphicsCore->ResetRenderTargetView(mainRenderTarget->GetRenderTargetView(), Colors::Gray);
		graphicsCore->ResetRenderTargetView(DMRAORenderTarget->GetRenderTargetView(), Colors::Black);
		graphicsCore->ResetRenderTargetView(normalRenderTarget->GetRenderTargetView(), Colors::Black);
		graphicsCore->ResetRenderTargetView(albedoRenderTarget->GetRenderTargetView(), Colors::Black);
		// 빛 정보를 확실히 하기 위해 여기서 alpha 값도 0으로 세팅해봅니다..
		graphicsCore->ResetRenderTargetView(worldPosRenderTarget->GetRenderTargetView(), XMVECTORF32{ 0.f, 0.f, 0.f, 0.f });
		graphicsCore->ResetRenderTargetView(emissiveRenderTarget->GetRenderTargetView(), Colors::Black);

		graphicsCore->ResetDepthStencilView(mainDepthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		graphicsCore->GetImmediateDC()->OMSetDepthStencilState(mainDepthStencil->GetDepthStencilState().Get(), 0);

		// Deferred
		ComPtr<ID3D11RenderTargetView> renderTargets[] =
		{
			//mainRenderTarget->GetRenderTargetView(),
			albedoRenderTarget->GetRenderTargetView(),
			DMRAORenderTarget->GetRenderTargetView(),
			normalRenderTarget->GetRenderTargetView(),
			worldPosRenderTarget->GetRenderTargetView(),
			emissiveRenderTarget->GetRenderTargetView()
		};
		graphicsCore->GetImmediateDC()->OMSetRenderTargets(ARRAYSIZE(renderTargets), renderTargets->GetAddressOf(), mainDepthStencil->GetDepthStencilView().Get());


		ImGUIManager::GetInstance()->Frame();
	}

	void Renderer::Render()
	{
		/*GraphicsDebugBeginEvent("Object Render");
		// 각종 렌더.
		for (auto obj : renderVector)
		{
			// Debug 이벤트를 오브젝트 별로 관리
			GraphicsDebugBeginEvent(obj->GetObjName());
			obj->Render();
			GraphicsDebugEndEvent();
		}
		GraphicsDebugEndEvent();*/

		//lightPass->IMGuiRender();
		//postProcessPass->ImGuiRender();

		// lightPass Seting
		GraphicsDebugBeginEvent("Light Pass");
		lightPass->Render(gBuffer, shadowPass->GetPointShadowDSV(), shadowPass->GetSpotShadowDSV());
		GraphicsDebugEndEvent();

		GraphicsDebugBeginEvent("PostProcess Pass");
		postProcessPass->Render(lightPass->GetLightingRTV());
		GraphicsDebugEndEvent();

		graphicsCore->ResetRS();
	}

	void Renderer::DebugRender()
	{
		/*ID3D11RenderTargetView* backbufferRTV = mainRenderTarget->GetRenderTargetView();
		// 다 그리고 백버퍼에 그려주자.
		graphicsCore->GetImmediateDC()->OMSetRenderTargets(1, &backbufferRTV, mainRenderTarget->GetDepthStencilView());*/

		GraphicsDebugBeginEvent("Debug RTV");

		ID3D11ShaderResourceView* null[] = { nullptr };

		auto depthState = mainDepthStencil->GetDepthStencilState();

		auto depthSRV = DMRAORenderTarget->GetShaderResourceView();

		spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, depthState.Get());
		spriteBatch->Draw(depthSRV.Get(), RECT{ 0, 0, static_cast<long>(clientWidth * 0.1f), static_cast<long>(clientHeight * 0.1f) });
		spriteBatch->End();

		auto normalSRV = normalRenderTarget->GetShaderResourceView();

		spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, depthState.Get());
		spriteBatch->Draw(normalSRV.Get(), RECT{ static_cast<long>(clientWidth * 0.1f), 0, static_cast<long>(clientWidth * 0.2f), static_cast<long>(clientHeight * 0.1f) });
		spriteBatch->End();

		auto albedoSRV = albedoRenderTarget->GetShaderResourceView();

		spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, depthState.Get());
		spriteBatch->Draw(albedoSRV.Get(), RECT{ static_cast<long>(clientWidth * 0.2f), 0, static_cast<long>(clientWidth * 0.3f), static_cast<long>(clientHeight * 0.1f) });
		spriteBatch->End();

		auto worldPosSRV = worldPosRenderTarget->GetShaderResourceView();

		spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, depthState.Get());
		spriteBatch->Draw(worldPosSRV.Get(), RECT{ static_cast<long>(clientWidth * 0.3f), 0, static_cast<long>(clientWidth * 0.4f), static_cast<long>(clientHeight * 0.1f) });
		spriteBatch->End();

		auto shadowSRV = shadowPass->GetShadowDSV()->GetShaderResource();

		spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, depthState.Get());
		spriteBatch->Draw(shadowSRV.Get(), RECT{ static_cast<long>(clientWidth * 0.4f), 0, static_cast<long>(clientWidth * 0.5f), static_cast<long>(clientHeight * 0.1f) });
		spriteBatch->End();

		graphicsCore->GetImmediateDC()->PSSetShaderResources(0, 1, null);

		GraphicsDebugEndEvent();
	}

	void Renderer::ShadowRenderStart()
	{
		GraphicsDebugBeginEvent("Shadow Render");

		auto pointLights = lightPass->GetPointLights();
		if (shadowPass->GetIsSetPointLightCnt() != true)
			shadowPass->SetPointLightArray(pointLights.size());

		shadowPass->RenderStart();
		//shadowPass->PointRenderStart();
	}

	void Renderer::ShadowRenderStart(int idx, bool isPointLight)
	{
		GraphicsDebugBeginEvent("Shadow Render");

		// pointLight면 true
		if (isPointLight == true)
		{
			auto pointLights = lightPass->GetPointLights();
			if (shadowPass->GetIsSetPointLightCnt() != true)
				shadowPass->SetPointLightArray(pointLights.size());

			//shadowPass->RenderStart();
			shadowPass->PointRenderStart(idx);
			shadowPass->SetLightMatrix(pointLights[idx]);
		}
		else
		{
			// 아니면 false (== SpotLight)
			auto spotLights = lightPass->GetSpotLights();
			if(shadowPass->GetIsSetSpotLightCnt() != true)
				shadowPass->SetSpotLightArray(spotLights.size());

			shadowPass->SpotRenderStart(idx);
			shadowPass->SetLightViewProj(spotLights[idx].lightViewProj);
		}
	}

	void Renderer::ShadowRender(std::shared_ptr<IDXObject> obj, bool isPointLight)
	{
		// BeginRender 전에 해줘야함.
		if(isPointLight == true)
			shadowPass->PointRender(obj);
		else
			shadowPass->Render(obj);
	}

	void Renderer::ShadowRenderEnd()
	{
		shadowPass->EndRender();
		//shadowPass->PointEndRender();
		GraphicsDebugEndEvent();
	}


	void Renderer::EndRender()
	{
		GraphicsDebugBeginEvent("ImGUI");
		ImGUIManager::GetInstance()->Render();
		GraphicsDebugEndEvent();

		graphicsCore->PresentSwapChain();
	}

}

// 렌더러 생성 함수
GraphicsEngineSpace::IRenderer* CreateRenderer()
{
	return new GraphicsEngineSpace::Renderer;
}

void DeleteRenderer(GraphicsEngineSpace::IRenderer* obj)
{
	if (obj != nullptr)
	{
		delete obj;
	}
}
