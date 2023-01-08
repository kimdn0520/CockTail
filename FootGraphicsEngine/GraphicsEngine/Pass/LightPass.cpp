#include "GraphicsPch.h"
#include "LightPass.h"

#include "GraphicsCore/DX11GraphicsCore.h"
#include "GraphicsCore/RenderTargetTexture.h"
#include "GraphicsCore/RenderTargetDepth.h"
#include "Manager/BufferManager.h"

#include "Object/Shader/VertexShader.h"
#include "Object/Shader/PixelShader.h"

#include "Manager/ShaderManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SamplerManager.h"
#include "Object/Buffer/BufferBase.h"

#include "Util/VertexStruct.h"
#include "Manager/ImGUIManager.h"

namespace GraphicsEngineSpace
{
	void LightPass::Start(const std::shared_ptr<RenderTargetDepth>& mainRTV)
	{
		quadVS = std::dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader("QuadVS"));

		PBRLightPS = std::dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader("PBRLightPS"));

		lightingRTV = mainRTV;

		graphicsCore = DX11GraphicsCore::GetInstance();
	}

	void LightPass::Release()
	{
		lightingRTV->Finalize();

		SafeReset(PBRLightPS);
		SafeReset(lightingRTV);
	}


	void LightPass::OnResize(int width, int height)
	{
		// 기존의 것을 지워주고 다시 생성한다.
	}

	void LightPass::RenderStart()
	{
		graphicsCore->ResetView(lightingRTV->GetRenderTargetView(), nullptr, Colors::Gray);

		D3DDeviceContext->OMSetRenderTargets(1, lightingRTV->GetRenderTargetView().GetAddressOf(), nullptr);
	}

	void LightPass::Render(const std::vector<std::shared_ptr<RenderTargetTexture>>& renderTargets)
	{
		RenderStart();

		quadVS->SetUpShader();
		PBRLightPS->SetUpShader();

		// cb 생성..
		cbLight cbLightPass;
		cbLightPass.DirLights[0] = directionalLight;
		cbLightPass.EyePosW = eyePos;
		cbLightPass.DirLightCnt = DIRECTION_LIGHT_COUNT;
		cbLightPass.PointLightCnt = POINT_LIGHT_COUNT;
		cbLightPass.SpotLightCnt = SPOT_LIGHT_COUNT;

		BufferManager::GetInstance()->GetBuffer("LightCB")->SetUpBuffer(0, &cbLightPass, ShaderType::PIXEL);

		// Albedo
		D3DDeviceContext->PSSetShaderResources(0, 1, renderTargets[0]->GetShaderResourceView().GetAddressOf());
		//DMRAO
		D3DDeviceContext->PSSetShaderResources(1, 1, renderTargets[1]->GetShaderResourceView().GetAddressOf());
		//Normal
		D3DDeviceContext->PSSetShaderResources(2, 1, renderTargets[2]->GetShaderResourceView().GetAddressOf());
		//Position
		D3DDeviceContext->PSSetShaderResources(3, 1, renderTargets[3]->GetShaderResourceView().GetAddressOf());
		//Emissive
		D3DDeviceContext->PSSetShaderResources(4, 1, renderTargets[4]->GetShaderResourceView().GetAddressOf());

		D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetLinearClamp().GetAddressOf());

		// ScreenMesh를 불러와서 화면에 그려준다.
		auto screenMesh = ResourceManager::GetInstance()->basicMeshes["ScreenMesh"];
		D3DDeviceContext->RSSetState(screenMesh->GetRasterState().Get());

		UINT stride = sizeof(VertexStruct::PBRStaticVertex);
		UINT offset = 0;

		D3DDeviceContext->IASetPrimitiveTopology(screenMesh->GetPrimitiveTopology());
		D3DDeviceContext->IASetVertexBuffers(0, 1, screenMesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(screenMesh->indexBuffers[0].Get(), DXGI_FORMAT_R32_UINT, 0);

		D3DDeviceContext->DrawIndexed(screenMesh->indexBuffersSize[0], 0, 0);

		EndRender();
	}

	void LightPass::EndRender()
	{
		ID3D11ShaderResourceView* nullSRV[5] = { nullptr };

		D3DDeviceContext->PSSetShaderResources(0, 5, nullSRV);

		pointLights.clear();
		spotLights.clear();
	}

	void LightPass::SetDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj)
	{
		directionalLight.color = color;
		directionalLight.direction = direction;
		directionalLight.power = power;
		directionalLight.lightViewProj = lightViewProj;
	}

	void LightPass::SetPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, SimpleMath::Matrix lightViewProj)
	{
		PBR::PointLight pointLight;

		pointLight.color = color;
		pointLight.position = position;
		pointLight.power = power;
		pointLight.range = range;
		pointLight.lightViewProj = lightViewProj;

		pointLights.emplace_back(pointLight);
	}

	void LightPass::SetSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, float halfAngle, float range, SimpleMath::Matrix lightViewProj)
	{
		PBR::SpotLight spotLight;

		spotLight.color = color;
		spotLight.direction = direction;
		spotLight.power = power;
		spotLight.halfAngle = halfAngle;
		spotLight.range = range;
		spotLight.lightViewProj = lightViewProj;

		spotLights.emplace_back(spotLight);
	}
}