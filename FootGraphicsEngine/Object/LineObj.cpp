#include "GraphicsPch.h"
#include "LineObj.h"

#include "Manager/ShaderManager.h"
#include "Manager/BufferManager.h"
#include "Manager/ResourceManager.h"

#include "Shader/ShaderBase.h"
#include "Buffer/BufferBase.h"

#include "GraphicsEngine/Renderer.h"

namespace GraphicsEngineSpace
{
	LineObj::LineObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, resourceManager(nullptr)
		, world()
		, view()
		, proj()
		, hasColor(false)
		, lineColor(DirectX::Colors::LimeGreen)
	{
	}

	LineObj::~LineObj()
	{
		for (auto objRes : meshResources)
		{
			SafeReset(objRes)
		}
	}

	bool LineObj::Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext)
	{
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		return true;
	}

	void LineObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// 행렬 넣기
		world = _world;
		view = _view;
		proj = _proj;
	}


	void LineObj::Render()
	{
		// 기본적으로 0번 인덱스의 정보만 사용한다 가정..
		assert(D3DDeviceContext);

		std::shared_ptr<Mesh> mesh = meshResources[0]->mesh;

		if (hasColor == true)
		{
			// 가장 기본적인 인풋 레이아웃을 가져옵시다.
			// 내부 함수에 inputlayout 세팅이 있습니다.
			ShaderManager::GetInstance()->GetShader("BasicColorVS")->SetUpShader();
			// pixel 셰이더
			ShaderManager::GetInstance()->GetShader("BasicColorPS")->SetUpShader();

			// 버텍스 버퍼, 인덱스 버퍼 세팅
			UINT stride = sizeof(VertexStruct::ColorVertex);
			UINT offset = 0;
			D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		}
		else
		{
			ShaderManager::GetInstance()->GetShader("LineVS")->SetUpShader();
			// pixel 셰이더
			ShaderManager::GetInstance()->GetShader("LinePS")->SetUpShader();

			// pixel Shader의 Color Buffer를 세팅해준다.
			BufferManager::GetInstance()->GetBuffer("ColorCB")->SetUpBuffer(1, &lineColor, ShaderType::PIXEL);

			// 버텍스 버퍼, 인덱스 버퍼 세팅
			UINT stride = sizeof(VertexStruct::PosVertex);
			UINT offset = 0;
			D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
		}
		// 버퍼 세팅을 할 이유가 없다 -> 들어가는 버퍼가 없기 때문.
		D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

		D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[0].Get(), DXGI_FORMAT_R32_UINT, 0);

		// 상수 버퍼 세팅.
		// 데이터는 constBuffer와 닮은꼴의 구조체를 던져야함.
		cbMatrix cbPerObj;
		cbPerObj.worldViewProj = world * view * proj;
		BufferManager::GetInstance()->GetBuffer("WorldViewProjCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);


		D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

		// 해당 내용 그리기.
		D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[0], 0, 0);
	}

	void LineObj::SetMeshResources(std::shared_ptr<MeshResources> objResources)
	{
		meshResources.push_back(objResources);
	}

	std::string LineObj::GetObjName()
	{
		return meshResources[0]->ObjName;
	}

	std::vector<std::shared_ptr<MeshResources>> LineObj::GetMeshResources()
	{
		return meshResources;
	}
}
