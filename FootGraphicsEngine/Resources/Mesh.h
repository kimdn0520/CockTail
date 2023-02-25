#pragma once
#include "GraphicsCore/DX11GraphicsCore.h"
#include "Resources/Resource.h"

namespace GraphicsEngineSpace
{
	class Mesh : public Resource
	{
	private:
		ComPtr<ID3D11RasterizerState> rasterState;

		D3D11_PRIMITIVE_TOPOLOGY topology;

	public:
		ComPtr<ID3D11Buffer> vertexBuffer;

		std::vector<ComPtr<ID3D11Buffer>> indexBuffers;

		std::vector<unsigned int> indexBuffersSize;

		unsigned int stride;

		std::vector<std::string> materials;

		SimpleMath::Matrix nodeTM = SimpleMath::Matrix::Identity;

		bool isSkinnedMesh = false;

	public:
		Mesh()
			: Resource(RESOURCE_TYPE::MESH)
			, stride(0)
		{};
		~Mesh() {};

	public:
		template <typename T>
		void CreateVertexBuffer(const std::vector<T>& vertices);

		inline void CreateIndexBuffer(const std::vector<unsigned int>& indexBuffer);

		const ComPtr<ID3D11RasterizerState>& GetRasterState() { return rasterState; }

		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() { return topology; }

		void SetStride(unsigned int stride) { this->stride = stride; }

		void SetRenderState(ComPtr<ID3D11RasterizerState> rasterState) { this->rasterState = rasterState; }

		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { this->topology = topology; }
	};

	template<typename T>
	inline void Mesh::CreateVertexBuffer(const std::vector<T>& vertices)
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = sizeof(T) * vertices.size();
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexInitData;
		vertexInitData.pSysMem = vertices.data();

		GraphicsEngineSpace::DX11GraphicsCore::GetInstance()->GetDevice()->
			CreateBuffer(&vertexBufferDesc, &vertexInitData, vertexBuffer.GetAddressOf());
	}

	inline void GraphicsEngineSpace::Mesh::CreateIndexBuffer(const std::vector<unsigned int>& indices)
	{
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * (unsigned int)indices.size();
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexInitData;
		indexInitData.pSysMem = indices.data();

		indexBuffersSize.emplace_back(indices.size());

		ComPtr<ID3D11Buffer> indexBuffer;

		GraphicsEngineSpace::DX11GraphicsCore::GetInstance()->GetDevice()->
			CreateBuffer(&indexBufferDesc, &indexInitData, indexBuffer.GetAddressOf());

		indexBuffers.emplace_back(indexBuffer);
	}
}
