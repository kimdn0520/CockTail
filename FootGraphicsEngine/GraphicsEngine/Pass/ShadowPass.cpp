#include "GraphicsPch.h"
#include "ShadowPass.h"

#include "GraphicsCore/DX11GraphicsCore.h"
#include "GraphicsCore/RenderTargetTexture.h"
#include "GraphicsCore/DepthStencilResource.h"
#include "Interface/IDXObject.h"
#include "Manager/BufferManager.h"

#include "Object/Shader/VertexShader.h"
#include "Object/Shader/GeometryShader.h"
#include "Interface/IDXObject.h"
#include "Resources/ObjectResources.h"

#include "Manager/ShaderManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SamplerManager.h"
#include "Object/PBRObj.h"
#include "Object/Buffer/BufferBase.h"

#include "Util/VertexStruct.h"

namespace GraphicsEngineSpace
{
	void ShadowPass::Start(const std::shared_ptr<RenderTargetTexture>& mainRTV,
		const std::shared_ptr<DepthStencilResource>& mainDSV)
	{
		auto shaderManager = ShaderManager::GetInstance();
		// dirlight 쉐도우를 위한 것.
		shadowStaticVS = std::dynamic_pointer_cast<VertexShader>(shaderManager->GetShader("ShadowStaticVS"));
		shadowSkinnedVS = std::dynamic_pointer_cast<VertexShader>(shaderManager->GetShader("ShadowSkinnedVS"));

		// point light 쉐도우를 위한 것.
		geoShadowStaticVS = std::dynamic_pointer_cast<VertexShader>(shaderManager->GetShader("GeoShadowStaticVS"));
		geoShadowSkinnedVS = std::dynamic_pointer_cast<VertexShader>(shaderManager->GetShader("GeoShadowSkinnedVS"));

		shadowGS = std::dynamic_pointer_cast<GeometryShader>(shaderManager->GetShader("ShadowGS"));

		this->mainDSV = mainDSV;

		dirShadowDSV = std::make_shared<DepthStencilResource>();
		pointShadowDSV = std::make_shared<DepthStencilResource>();
		spotShadowDSV = std::make_shared<DepthStencilResource>();

		graphicsCore = DX11GraphicsCore::GetInstance();
	}

	void ShadowPass::Release()
	{
		SafeReset(shadowStaticVS)
			SafeReset(shadowSkinnedVS)
	}

	void ShadowPass::OnResize(int width, int height)
	{
		dirShadowDSV->DeleteImmediateDepthStencil();
		dirShadowDSV->CreateDepthStencil(D3DDevice, D3DDeviceContext, 4096, 4096);
	}

	// 씬이 바뀔 때 마다 DSV를 초기화 시켜 줍니다.
	void ShadowPass::ResetIsSetPointLightCnt()
	{
		// Scene이 바뀔 때 기존의 View를 지워준다.
			// => 이게 반드시 보장 되어야함.
		pointShadowDSV->DeleteImmediateDepthStencil();

		isSetPointLightCnt = false;
	}

	void ShadowPass::ResetIsSetSpotLightCnt()
	{
		spotShadowDSV->DeleteImmediateDepthStencil();

		isSetSpotLightCnt = false;
	}

	void ShadowPass::SetPointLightArray(int pointLightCnt)
	{
		pointShadowDSV->CreateDepthStencilCube(D3DDevice, D3DDeviceContext, 1024, 1024, pointLightCnt);

		isSetPointLightCnt = true;
	}

	void ShadowPass::SetSpotLightArray(int spotLightCnt)
	{
		spotShadowDSV->CreateDepthStencilArray(D3DDevice, D3DDeviceContext, 1024, 1024, spotLightCnt);

		isSetSpotLightCnt = true;
	}

	void ShadowPass::RenderStart()
	{
		// 렌더 타겟 세팅 없이 DSV만 세팅해줘서 SRV를 만들어준다.
			// 사실상 이부분이 Begin Render 전에 들어가야할듯..
			// 렌더 타겟 클리어 => Shadow Render => EndRender
			// 오브젝트 렌더 (Begin, Render, End)
			// Light Render 순으로 진행되어야할 듯.

		D3DDeviceContext->RSSetViewports(1, dirShadowDSV->GetViewport().get());

		graphicsCore->ResetDepthStencilView(dirShadowDSV->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RenderTargetView* renderTargets[1] = { 0 };
		D3DDeviceContext->OMSetRenderTargets(1, renderTargets, dirShadowDSV->GetDepthStencilView().Get());
	}

	void ShadowPass::Render(std::shared_ptr<IDXObject> object)
	{
		// object 단위로 만들어줬기 때문에.. mesh 하나라도 shadow가 없다면
				// 오브젝트 전체가 shadow가 없는 것..
		if (object->GetMeshResources()[0]->isShadow != true)
			return;

		// 오브젝트 별 상수 버퍼를 만들어준다.
		cbMatrix cbPerObj;
		SimpleMath::Matrix world = object->GetWorld();

		std::vector<SimpleMath::Matrix> nowBoneTM;
		if (std::dynamic_pointer_cast<PBRObj>(object) != nullptr)
			nowBoneTM = std::dynamic_pointer_cast<PBRObj>(object)->GetNowBoneTM();

		// 오브젝트를 받아서 돌려준다.
		for (auto& meshRes : object->GetMeshResources())
		{
			UINT stride;
			UINT offset = 0;

			if (meshRes->isSkinnedMesh != true)
			{
				// 정적 메시
				shadowStaticVS->SetUpShader();
				stride = sizeof(VertexStruct::PBRStaticVertex);

				auto newWorld = meshRes->nodeTM * world;
				cbPerObj.worldTransform = newWorld;
			}
			else
			{
				// 스키닝 메시
				shadowSkinnedVS->SetUpShader();
				stride = sizeof(VertexStruct::PBRSkinnedVertex);

				cbBoneMatrix cbBone;

				for (int i = 0; i < nowBoneTM.size(); i++)
				{
					cbBone.boneMatrix[i] = nowBoneTM[i];
				}

				BufferManager::GetInstance()->GetBuffer("BoneMatrixCB")->SetUpBuffer(2, &cbBone, ShaderType::VERTEX);

				cbPerObj.worldTransform = world;

			}

			auto material = meshRes->material;


			for (int matCnt = 0; matCnt < material.size(); matCnt++)
			{
				if(meshRes->mesh->indexBuffersSize.empty())
					break;

				// 상수 버퍼 세팅.
				BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

				BufferManager::GetInstance()->GetBuffer("LightViewProjCB")->SetUpBuffer(1, &lightViewProj, ShaderType::VERTEX);

				D3DDeviceContext->PSSetShader(nullptr, nullptr, 0);
				D3DDeviceContext->RSSetState(RasterizerState::GetPointDepthRS().Get());

				D3DDeviceContext->IASetPrimitiveTopology(meshRes->mesh->GetPrimitiveTopology());
				D3DDeviceContext->IASetVertexBuffers(0, 1, meshRes->mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
				D3DDeviceContext->IASetIndexBuffer(meshRes->mesh->indexBuffers[matCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

				D3DDeviceContext->DrawIndexed(meshRes->mesh->indexBuffersSize[matCnt], 0, 0);
			}
		}
	}

	void ShadowPass::EndRender()
	{
		ID3D11RenderTargetView* renderTargets[1] = { 0 };

		// 사용이 끝난 지오메트리 쉐이더는 nullptr로 설정해줍니다.
		D3DDeviceContext->GSSetShader(nullptr, nullptr, 0);

		// 세팅을 원래대로..
		D3DDeviceContext->OMSetRenderTargets(1, renderTargets, mainDSV->GetDepthStencilView().Get());
		D3DDeviceContext->RSSetViewports(1, mainDSV->GetViewport().get());
	}

	void ShadowPass::PointRenderStart(int idx)
	{
		D3DDeviceContext->RSSetViewports(1, pointShadowDSV->GetViewport().get());

		auto depthStencilArray = pointShadowDSV->GetDepthStencilArray();
		auto tmpDepthStencil = depthStencilArray[idx];

		graphicsCore->ResetDepthStencilView(tmpDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RenderTargetView* renderTargets[1] = { 0 };
		D3DDeviceContext->OMSetRenderTargets(1, renderTargets, tmpDepthStencil.Get());
	}

	void ShadowPass::PointRender(std::shared_ptr<IDXObject> object)
	{
		if (object->GetMeshResources()[0]->isShadow != true)
			return;

		cbMatrix cbPerObj;
		SimpleMath::Matrix world = object->GetWorld();

		std::vector<SimpleMath::Matrix> nowBoneTM;
		if (std::dynamic_pointer_cast<PBRObj>(object) != nullptr)
			nowBoneTM = std::dynamic_pointer_cast<PBRObj>(object)->GetNowBoneTM();

		// 오브젝트를 받아서 돌려준다.
		for (auto& meshRes : object->GetMeshResources())
		{
			UINT stride;
			UINT offset = 0;

			if (meshRes->isSkinnedMesh != true)
			{
				// 정적 메시
				geoShadowStaticVS->SetUpShader();
				stride = sizeof(VertexStruct::PBRStaticVertex);

				auto newWorld = meshRes->nodeTM * world;
				cbPerObj.worldTransform = newWorld;
			}
			else
			{
				// 스키닝 메시
				geoShadowSkinnedVS->SetUpShader();
				stride = sizeof(VertexStruct::PBRSkinnedVertex);

				cbBoneMatrix cbBone;

				for (int i = 0; i < nowBoneTM.size(); i++)
				{
					cbBone.boneMatrix[i] = nowBoneTM[i];
				}

				BufferManager::GetInstance()->GetBuffer("BoneMatrixCB")->SetUpBuffer(2, &cbBone, ShaderType::VERTEX);

				cbPerObj.worldTransform = world;

			}

			auto material = meshRes->material;


			for (int matCnt = 0; matCnt < material.size(); matCnt++)
			{
				if (meshRes->mesh->indexBuffersSize.empty())
					break;

				// 상수 버퍼 세팅.
				BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

				// Geometry에 light Matrix 버퍼를 넣어줍니다.
					// 특정 포인트 Light의 lightViewProj을 얻어와야 합니다.
				BufferManager::GetInstance()->GetBuffer("LightMatrixCB")->SetUpBuffer(1, &lightMatrix, ShaderType::GEOMETRY);

				shadowGS->SetUpShader();

				D3DDeviceContext->PSSetShader(nullptr, nullptr, 0);
				D3DDeviceContext->RSSetState(RasterizerState::GetPointDepthRS().Get());

				D3DDeviceContext->IASetPrimitiveTopology(meshRes->mesh->GetPrimitiveTopology());
				D3DDeviceContext->IASetVertexBuffers(0, 1, meshRes->mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
				D3DDeviceContext->IASetIndexBuffer(meshRes->mesh->indexBuffers[matCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

				D3DDeviceContext->DrawIndexed(meshRes->mesh->indexBuffersSize[matCnt], 0, 0);
			}
		}
	}

	void ShadowPass::SpotRenderStart(int idx)
	{
		D3DDeviceContext->RSSetViewports(1, spotShadowDSV->GetViewport().get());

		auto depthStencilArray = spotShadowDSV->GetDepthStencilArray();
		auto tmpDepthStencil = depthStencilArray[idx];

		graphicsCore->ResetDepthStencilView(tmpDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RenderTargetView* renderTargets[1] = { 0 };
		D3DDeviceContext->OMSetRenderTargets(1, renderTargets, tmpDepthStencil.Get());
	}

	void ShadowPass::SetLightViewProj(SimpleMath::Matrix lightViewProj)
	{
		this->lightViewProj = lightViewProj;
	}

	void ShadowPass::SetLightMatrix(std::vector<SimpleMath::Matrix> lightMatrix)
	{
		for(int i = 0; i < 6; i++)
		{
			this->lightMatrix.lightViewProj[i] = lightMatrix[i];
		}
	}

	void ShadowPass::SetLightMatrix(PBR::PointLight pointLight)
	{
		for (int i = 0; i < 6; i++)
		{
			this->lightMatrix.lightViewProj[i] = pointLight.lightViewProj[i];
		}
	}
}
