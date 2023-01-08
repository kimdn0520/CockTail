#include "GraphicsPch.h"
#include "SkyBox.h"

#include "Manager/ShaderManager.h"
#include "Manager/BufferManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SamplerManager.h"

#include "Shader/ShaderBase.h"
#include "Buffer/BufferBase.h"
#include "Resources/Texture.h"
#include "Resources/RenderingMaterial.h"

#include "GraphicsEngine/Renderer.h"

namespace GraphicsEngineSpace
{
	GraphicsEngineSpace::SkyBox::SkyBox()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, resourceManager(nullptr)
		, world()
		, view()
		, proj()
	{
	}

	SkyBox::~SkyBox()
	{
		for (auto objRes : meshResources)
		{
			SafeReset(objRes)
		}
	}

	bool SkyBox::Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext)
	{
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		return true;
	}

	void SkyBox::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// 행렬 넣기
		world = _world;
		view = _view;
		proj = _proj;
	}

	void SkyBox::Render()
	{
		assert(D3DDeviceContext);

		auto mesh = meshResources[0]->mesh;
		auto material = meshResources[0]->material[0];

		auto cube = material->cubeTex;

		if (mesh == nullptr)
			return;

		// SkyBox Render
		// 여기서 SkyBox Render
		// RasterizeState 및 Depth State의 경우 기본적으로 용책 내용과 동일하게 되어있다..
		// 쉐이더 세팅
		ShaderManager::GetInstance()->GetShader("SkyBoxVS")->SetUpShader();
		ShaderManager::GetInstance()->GetShader("SkyBoxPS")->SetUpShader();

		D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

		UINT stride = sizeof(VertexStruct::PosVertex);
		UINT offset = 0;

		D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[0].Get(), DXGI_FORMAT_R32_UINT, 0);

		// 상수 버퍼 세팅
			// view만 받아오면..
		cbMatrix cbMat;
		cbMat.worldViewProj = world * view * proj;

		BufferManager::GetInstance()->GetBuffer("WorldViewProjCB")->SetUpBuffer(0, &cbMat, ShaderType::VERTEX);

		// skyBox 넣어주기..
		D3DDeviceContext->PSSetShaderResources(0, 1, cube->GetMapSRV().GetAddressOf());

		D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetLinearWrap().GetAddressOf());

		D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

		D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[0], 0, 0);
	}

	void SkyBox::SetMeshResources(std::shared_ptr<MeshResources> objResources)
	{
		meshResources.push_back(objResources);
	}

	std::string SkyBox::GetObjName()
	{
		return meshResources[0]->ObjName;
	}

	std::vector<std::shared_ptr<MeshResources>> SkyBox::GetMeshResources()
	{
		return meshResources;
	}
}
