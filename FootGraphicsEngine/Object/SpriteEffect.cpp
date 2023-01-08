#include "GraphicsPch.h"
#include "SpriteEffect.h"

#include "Manager/ShaderManager.h"
#include "Manager/BufferManager.h"
#include "Manager/ResourceManager.h"

#include "Shader/ShaderBase.h"
#include "Buffer/BufferBase.h"

#include "GraphicsEngine/Renderer.h"
#include "Manager/SamplerManager.h"
#include "Resources/RenderingMaterial.h"

namespace GraphicsEngineSpace
{
	SpriteEffect::SpriteEffect()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, resourceManager(nullptr)
		, world()
		, view()
		, proj()
		, texOffset(0.f, 0.f, 0.f, 0.f)
	{
	}

	SpriteEffect::~SpriteEffect()
	{
		for (auto meshRes : meshResources)
		{
			SafeReset(meshRes);
		}
	}

	bool SpriteEffect::Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext)
	{
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		for (auto& objRes : meshResources)
		{
			objRes->mesh = CreateEffectMesh(objRes->ObjName);

			objRes->nodeTM = objRes->mesh->nodeTM;

			if (objRes->mesh->materials.empty())
			{
				std::shared_ptr<RenderingMaterial> material = std::make_shared<RenderingMaterial>();

				// TODO 해당부분 변경 가능하도록 해야함
				auto albedoTex = resourceManager->LoadTexture(L"Resources/Effect/CutEffect.png");
				material->albedoTex = albedoTex;

				objRes->material.emplace_back(material);

				objRes->setMaterial = true;

				objRes->material.emplace_back(material);
			}
		}

		return true;
	}

	void SpriteEffect::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// 행렬 넣기
		world = _world;
		view = _view;
		proj = _proj;
	}

	void SpriteEffect::Render()
	{
		assert(D3DDeviceContext);

		std::shared_ptr<Mesh> mesh = meshResources[0]->mesh;

		ShaderManager::GetInstance()->GetShader("SpriteEffectVS")->SetUpShader();
		ShaderManager::GetInstance()->GetShader("SpriteEffectPS")->SetUpShader();

		UINT stride = sizeof(VertexStruct::TexVertex);
		UINT offset = 0;
		BufferManager::GetInstance()->GetBuffer("TexOffsetCB")->SetUpBuffer(1, &texOffset, ShaderType::VERTEX);

		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("WorldViewProjCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

		D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());
		D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[0].Get(), DXGI_FORMAT_R32_UINT, 0);

		// Sampler와 texture 세팅
		D3DDeviceContext->PSSetShaderResources(0, 1, meshResources[0]->material[0]->albedoTex->GetMapSRV().GetAddressOf());

		D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetLinearClamp().GetAddressOf());

		D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

		D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[0], 0, 0);
	}

	void SpriteEffect::SetMeshResources(std::shared_ptr<MeshResources> objResources)
	{
		meshResources.emplace_back(objResources);
	}

	std::string SpriteEffect::GetObjName()
	{
		return meshResources[0]->ObjName;
	}

	std::vector<std::shared_ptr<MeshResources>> SpriteEffect::GetMeshResources()
	{
		return meshResources;
	}

	void SpriteEffect::SetTexOffset(float u, float v)
	{
		texOffset.x = u;
		texOffset.y = v;
	}

	void SpriteEffect::SetTexOffset(const SimpleMath::Vector2& offset)
	{
		texOffset.x = offset.x;
		texOffset.y = offset.y;
	}

	SimpleMath::Vector2 SpriteEffect::GetTexOffset()
	{
		return SimpleMath::Vector2{texOffset.x, texOffset.y};
	}

	std::shared_ptr<Mesh> SpriteEffect::CreateEffectMesh(std::string name)
	{
		// 외부에서 불러올 메시(FBX에서 불러온 메시..)
		FBXBinaryData::MeshData binaryMesh;

		std::ifstream ifs("BinaryFile/Mesh/" + name + ".mesh", std::ios_base::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
		buffer.push(boost::iostreams::zlib_decompressor());
		buffer.push(ifs);
		boost::archive::binary_iarchive iaMesh(buffer);
		iaMesh >> binaryMesh;

		std::vector<VertexStruct::TexVertex> vertices;

		for (auto& vertex : binaryMesh.meshVertexList)
		{
			auto tempVertex = std::make_unique<VertexStruct::TexVertex>();

			tempVertex->Pos.x = vertex.position.x;
			tempVertex->Pos.y = vertex.position.y;
			tempVertex->Pos.z = vertex.position.z;

			tempVertex->Tex.x = vertex.uv.x;
			tempVertex->Tex.y = vertex.uv.y;

			vertices.push_back(*tempVertex);
		}

		std::vector<std::vector<unsigned int>> indices = binaryMesh.indices;

		auto mesh = resourceManager->CreateMesh(
			vertices,
			indices,
			name,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		// Mesh에 node TM 정보를 넣어준다.
		auto nodeTM = binaryMesh.nodeTM;

		mesh->nodeTM.m[0][0] = nodeTM.m[0][0];
		mesh->nodeTM.m[0][1] = nodeTM.m[0][1];
		mesh->nodeTM.m[0][2] = nodeTM.m[0][2];
		mesh->nodeTM.m[0][3] = nodeTM.m[0][3];

		mesh->nodeTM.m[1][0] = nodeTM.m[1][0];
		mesh->nodeTM.m[1][1] = nodeTM.m[1][1];
		mesh->nodeTM.m[1][2] = nodeTM.m[1][2];
		mesh->nodeTM.m[1][3] = nodeTM.m[1][3];

		mesh->nodeTM.m[2][0] = nodeTM.m[2][0];
		mesh->nodeTM.m[2][1] = nodeTM.m[2][1];
		mesh->nodeTM.m[2][2] = nodeTM.m[2][2];
		mesh->nodeTM.m[2][3] = nodeTM.m[2][3];

		mesh->nodeTM.m[3][0] = nodeTM.m[3][0];
		mesh->nodeTM.m[3][1] = nodeTM.m[3][1];
		mesh->nodeTM.m[3][2] = nodeTM.m[3][2];
		mesh->nodeTM.m[3][3] = nodeTM.m[3][3];

		mesh->SetName(binaryMesh.meshName);

		return mesh;
	}
}
