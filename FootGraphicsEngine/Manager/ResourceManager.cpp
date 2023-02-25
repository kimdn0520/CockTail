#include "GraphicsPch.h"
#include "ResourceManager.h"
#include "BuilderManager.h"
#include "Object/PBRObj.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<ResourceManager> ResourceManager::instance = nullptr;

	std::shared_ptr<ResourceManager> ResourceManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<ResourceManager>();

		return instance;
	}

	std::shared_ptr<Mesh> ResourceManager::LoadMesh(std::string name)
	{
		// 내부에 저장된 기본 메시를 찾는지 확인한다.
		auto findIt = basicMeshes.find(name);

		if (findIt != basicMeshes.end())
			return basicMeshes[name];
		
		// 외부에서 불러올 메시(FBX에서 불러온 메시..)
		FBXBinaryData::MeshData binaryMesh;

		std::ifstream ifs(meshPath + name + ".mesh", std::ios_base::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
		buffer.push(boost::iostreams::zlib_decompressor());
		buffer.push(ifs);
		boost::archive::binary_iarchive iaMesh(buffer);
		iaMesh >> binaryMesh;

		if (binaryMesh.isSkinned)
		{
			// 버텍스 정보 입력
			std::vector<VertexStruct::PBRSkinnedVertex> vertices;

			// 버텍스 정보 입력
			for (auto& vertex : binaryMesh.meshVertexList)
			{
				auto tempVertex = std::make_unique<VertexStruct::PBRSkinnedVertex>();

				tempVertex->Pos.x = vertex.position.x;
				tempVertex->Pos.y = vertex.position.y;
				tempVertex->Pos.z = vertex.position.z;

				tempVertex->Tex.x = vertex.uv.x;
				tempVertex->Tex.y = vertex.uv.y;

				tempVertex->Normal.x = vertex.normal.x;
				tempVertex->Normal.y = vertex.normal.y;
				tempVertex->Normal.z = vertex.normal.z;

				tempVertex->TangentU.x = vertex.tangent.x;
				tempVertex->TangentU.y = vertex.tangent.y;
				tempVertex->TangentU.z = vertex.tangent.z;

				tempVertex->Color.x = vertex.color.x;
				tempVertex->Color.y = vertex.color.y;
				tempVertex->Color.z = vertex.color.z;
				tempVertex->Color.w = vertex.color.w;

				tempVertex->Weight.x = vertex.weights[0];
				tempVertex->Weight.y = vertex.weights[1];
				tempVertex->Weight.z = vertex.weights[2];
				tempVertex->Weight.w = vertex.weights[3];

				tempVertex->Weights.x = vertex.weights[4];
				tempVertex->Weights.y = vertex.weights[5];
				tempVertex->Weights.z = vertex.weights[6];
				tempVertex->Weights.w = vertex.weights[7];

				tempVertex->BoneIndex.x = vertex.boneIndices[0];
				tempVertex->BoneIndex.y = vertex.boneIndices[1];
				tempVertex->BoneIndex.z = vertex.boneIndices[2];
				tempVertex->BoneIndex.w = vertex.boneIndices[3];

				tempVertex->BoneIndices.x = vertex.boneIndices[4];
				tempVertex->BoneIndices.y = vertex.boneIndices[5];
				tempVertex->BoneIndices.z = vertex.boneIndices[6];
				tempVertex->BoneIndices.w = vertex.boneIndices[7];


				vertices.push_back(*tempVertex);
			}

			std::vector<std::vector<unsigned int>> indices = binaryMesh.indices;

			auto mesh = CreateMesh(
				vertices,
				indices,
				name,
				D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				RasterizerState::GetSolidRS()
			);

			// Mesh에 참조하는 머터리얼 이름들을 넣어준다.
			for (auto& mat : binaryMesh.materials)
			{
				mesh->materials.emplace_back(mat);
			}

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

			mesh->isSkinnedMesh = true;

			return mesh;
		}
		else
		{
			// 버텍스 정보 입력
			std::vector<VertexStruct::PBRStaticVertex> vertices;

			// 버텍스 정보 입력
			for (auto& vertex : binaryMesh.meshVertexList)
			{
				auto tempVertex = std::make_unique<VertexStruct::PBRStaticVertex>();

				tempVertex->Pos.x = vertex.position.x;
				tempVertex->Pos.y = vertex.position.y;
				tempVertex->Pos.z = vertex.position.z;

				tempVertex->Tex.x = vertex.uv.x;
				tempVertex->Tex.y = vertex.uv.y;

				tempVertex->Normal.x = vertex.normal.x;
				tempVertex->Normal.y = vertex.normal.y;
				tempVertex->Normal.z = vertex.normal.z;

				tempVertex->TangentU.x = vertex.tangent.x;
				tempVertex->TangentU.y = vertex.tangent.y;
				tempVertex->TangentU.z = vertex.tangent.z;

				tempVertex->Color.x = vertex.color.x;
				tempVertex->Color.y = vertex.color.y;
				tempVertex->Color.z = vertex.color.z;
				tempVertex->Color.w = vertex.color.w;

				vertices.push_back(*tempVertex);
			}

			std::vector<std::vector<unsigned int>> indices = binaryMesh.indices;

			auto mesh = CreateMesh(
				vertices,
				indices,
				name,
				D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				RasterizerState::GetSolidRS()
			);

			// Mesh에 참조하는 머터리얼 이름들을 넣어준다.
			for (auto& mat : binaryMesh.materials)
			{
				mesh->materials.emplace_back(mat);
			}

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
		
		return nullptr;
	}

	std::shared_ptr<Texture> ResourceManager::LoadTexture(std::wstring path)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->Load(path);

		return texture;
	}

	void ResourceManager::Initialize()
	{
		meshPath = "BinaryFile/Mesh/";
		materialPath = "BinaryFile/Material/";
		AnimationPath = "BinaryFile/Animation/";
		modelPath = "BinaryFile/Model/";
		prefabPath = "BinaryFile/Prefab/";

		LoadCubeMesh();

		LoadSphereMesh();

		LoadAxisMesh();

		LoadGridMesh();

		LoadScreenMesh();

		LoadDebugScreenMesh();

		LoadWireCubeMesh();

		LoadWireSphereMesh();

		LoadCapsuleMesh();

		LoadPlaneMesh();

		LoadPrefab();
	}

	void ResourceManager::Release()
	{
		for (auto& mesh : basicMeshes)
		{
			mesh.second.reset();
		}
	}

	void ResourceManager::LoadCubeMesh()
	{
		float w2 = 0.5f;
		float h2 = 0.5f;
		float d2 = 0.5f;

		std::vector<VertexStruct::PBRStaticVertex> vertices(24);

		// 앞면
		vertices[0] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, -d2), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[1] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, -d2), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[2] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, -d2), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[3] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, -d2), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 뒷면
		vertices[4] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, +d2), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[5] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, +d2), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[6] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, +d2), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[7] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, +d2), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 윗면
		vertices[8] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, -d2), SimpleMath::Vector3(0.0f, 1.0f, 0.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[9] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, +d2), SimpleMath::Vector3(0.0f, 1.0f, 0.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[10] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, +d2), SimpleMath::Vector3(0.0f, 1.0f, 0.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[11] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, -d2), SimpleMath::Vector3(0.0f, 1.0f, 0.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 아랫면
		vertices[12] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, -d2), SimpleMath::Vector3(0.0f, -1.0f, 0.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[13] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, -d2), SimpleMath::Vector3(0.0f, -1.0f, 0.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[14] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, +d2), SimpleMath::Vector3(0.0f, -1.0f, 0.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[15] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, +d2), SimpleMath::Vector3(0.0f, -1.0f, 0.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 왼쪽면
		vertices[16] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, +d2), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[17] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, +d2), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[18] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, -d2), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[19] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, -d2), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 오른쪽면
		vertices[20] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, -d2), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[21] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, -d2), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[22] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, +d2), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[23] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, +d2), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		std::vector<std::vector<unsigned int>> idx(1, std::vector<unsigned int>(36));

		// 앞면
		idx[0][0] = 0; idx[0][1] = 1; idx[0][2] = 2;
		idx[0][3] = 0; idx[0][4] = 2; idx[0][5] = 3;
		// 뒷면
		idx[0][6] = 4; idx[0][7] = 5; idx[0][8] = 6;
		idx[0][9] = 4; idx[0][10] = 6; idx[0][11] = 7;
		// 윗면
		idx[0][12] = 8; idx[0][13] = 9; idx[0][14] = 10;
		idx[0][15] = 8; idx[0][16] = 10; idx[0][17] = 11;
		// 아랫면
		idx[0][18] = 12; idx[0][19] = 13; idx[0][20] = 14;
		idx[0][21] = 12; idx[0][22] = 14; idx[0][23] = 15;
		// 왼쪽면
		idx[0][24] = 16; idx[0][25] = 17; idx[0][26] = 18;
		idx[0][27] = 16; idx[0][28] = 18; idx[0][29] = 19;
		// 오른쪽면
		idx[0][30] = 20; idx[0][31] = 21; idx[0][32] = 22;
		idx[0][33] = 20; idx[0][34] = 22; idx[0][35] = 23;

		auto mesh = CreateMesh(
			vertices,
			idx,
			"CubeMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		basicMeshes.insert(std::make_pair("CubeMesh", mesh));
	}

	void ResourceManager::LoadSphereMesh()
	{
		float radius = 0.5f;	// 구의 반지름
		uint32 stackCount = 20; // 가로 분할
		uint32 sliceCount = 20; // 세로 분할

		std::vector<VertexStruct::PBRStaticVertex> vertices;

		VertexStruct::PBRStaticVertex v;

		// 북극
		v.Pos = SimpleMath::Vector3(0.0f, radius, 0.0f);
		v.Tex = SimpleMath::Vector2(0.5f, 0.0f);
		v.Normal = v.Pos;
		XMVector3Normalize(v.Normal);
		v.Normal.Normalize();
		v.TangentU = SimpleMath::Vector3(1.0f, 0.0f, 1.0f);
		vertices.push_back(v);

		float stackAngle = XM_PI / stackCount;
		float sliceAngle = XM_2PI / sliceCount;

		float deltaU = 1.f / static_cast<float>(sliceCount);
		float deltaV = 1.f / static_cast<float>(stackCount);

		// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
		for (uint32 y = 1; y <= stackCount - 1; ++y)
		{
			float phi = y * stackAngle;

			// 고리에 위치한 정점
			for (uint32 x = 0; x <= sliceCount; ++x)
			{
				float theta = x * sliceAngle;

				v.Pos.x = radius * sinf(phi) * cosf(theta);
				v.Pos.y = radius * cosf(phi);
				v.Pos.z = radius * sinf(phi) * sinf(theta);

				v.Tex = SimpleMath::Vector2(deltaU * x, deltaV * y);

				v.Normal = v.Pos;
				v.Normal.Normalize();

				v.TangentU.x = -radius * sinf(phi) * sinf(theta);
				v.TangentU.y = 0.0f;
				v.TangentU.z = radius * sinf(phi) * cosf(theta);
				v.TangentU.Normalize();

				vertices.push_back(v);
			}
		}

		// 남극
		v.Pos = SimpleMath::Vector3(0.0f, -radius, 0.0f);
		v.Tex = SimpleMath::Vector2(0.5f, 1.0f);
		v.Normal = v.Pos;
		v.Normal.Normalize();
		v.TangentU = SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
		vertices.push_back(v);

		std::vector<std::vector<unsigned int>> idx(1);

		// 북극 인덱스
		for (uint32 i = 0; i <= sliceCount; ++i)
		{
			//  [0]
			//   |  \
			//  [i+1]-[i+2]
			idx[0].push_back(0);
			idx[0].push_back(i + 2);
			idx[0].push_back(i + 1);
		}

		// 몸통 인덱스
		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 y = 0; y < stackCount - 2; ++y)
		{
			for (uint32 x = 0; x < sliceCount; ++x)
			{
				//  [y, x]-[y, x+1]
				//  |		/
				//  [y+1, x]
				idx[0].push_back(1 + (y)*ringVertexCount + (x));
				idx[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				//		 [y, x+1]
				//		 /	  |
				//  [y+1, x]-[y+1, x+1]
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				idx[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x + 1));
			}
		}

		// 남극 인덱스
		uint32 bottomIndex = static_cast<uint32>(vertices.size()) - 1;
		uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
		for (uint32 i = 0; i < sliceCount; ++i)
		{
			//  [last+i]-[last+i+1]
			//  |      /
			//  [bottom]
			idx[0].push_back(bottomIndex);
			idx[0].push_back(lastRingStartIndex + i);
			idx[0].push_back(lastRingStartIndex + i + 1);
		}

		auto mesh = CreateMesh(
			vertices,
			idx,
			"SphereMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		basicMeshes.insert(std::make_pair("SphereMesh", mesh));
	}

	void ResourceManager::LoadAxisMesh()
	{
		std::vector<VertexStruct::ColorVertex> vertices =
		{
			// x축 => 빨간색
			{SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::Red)},
			{SimpleMath::Vector3(50.0f, 0.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::Red)},

			// y축 초록색
			{SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::GreenYellow)},
			{SimpleMath::Vector3(0.0f, 50.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::GreenYellow)},

			// z축 파란색
			{SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::Blue)},
			{SimpleMath::Vector3(0.0f, 0.0f, 50.0f), SimpleMath::Vector4((const float*)&Colors::Blue)},
		};

		// 인덱스 버퍼 생성
		std::vector<std::vector<unsigned int>> indices(1);
		indices[0].emplace_back(0);
		indices[0].emplace_back(1);
		indices[0].emplace_back(2);
		indices[0].emplace_back(3);
		indices[0].emplace_back(4);
		indices[0].emplace_back(5);

		auto mesh = CreateMesh(
			vertices,
			indices,
			"AxisMesh",
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			RasterizerState::GetWireFrameRS()
		);

		basicMeshes.insert(std::make_pair("AxisMesh", mesh));
	}

	void ResourceManager::LoadGridMesh()
	{
		std::vector<VertexStruct::ColorVertex> vertices(84);

		float z_val = -10.0f;
		// for문을 돌면서 버텍스 생성.
		// 세로 버텍스
		for (int i = 0; i < 42; i += 2)
		{
			vertices[i] = { SimpleMath::Vector3(-10.0f, 0.0f, z_val), SimpleMath::Vector4((const float*)&Colors::White) };
			vertices[i + 1] = { SimpleMath::Vector3(10.0f, 0.0f, z_val), SimpleMath::Vector4((const float*)&Colors::White) };
			z_val++;
		}
		float x_val = -10.0f;
		for (int i = 42; i < 84; i += 2)
		{
			vertices[i] = { SimpleMath::Vector3(x_val, 0.0f, -10.0f), SimpleMath::Vector4((const float*)&Colors::White) };
			vertices[i + 1] = { SimpleMath::Vector3(x_val, 0.0f, 10.0f), SimpleMath::Vector4((const float*)&Colors::White) };
			x_val++;
		}

		// 인덱스 버퍼 생성
		// 총 라인의 개수 40개 => 인덱스의 개수 40 * 2개
		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(84));

		for (int i = 0; i < 84; i++)
		{
			indices[0][i] = i;
		}

		auto mesh = CreateMesh(
			vertices,
			indices,
			"GridMesh",
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			RasterizerState::GetWireFrameRS()
		);

		basicMeshes.insert(std::make_pair("GridMesh", mesh));
	}

	void ResourceManager::LoadScreenMesh()
	{
		// Screen Mesh 생성
		std::vector<VertexStruct::TexVertex> vertices(6);

		// 좌 하단
		vertices[0].Pos = SimpleMath::Vector3(-1.f, -1.f, 0.f);
		vertices[0].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1].Pos = SimpleMath::Vector3(-1.f, 1.f, 0.f);
		vertices[1].Tex = SimpleMath::Vector2(0.f, 0.f);

		// 우 상단
		vertices[2].Pos = SimpleMath::Vector3(1.f, 1.f, 0.f);
		vertices[2].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3].Pos = SimpleMath::Vector3(-1.f, -1.f, 0.f);
		vertices[3].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 우 상단
		vertices[4].Pos = SimpleMath::Vector3(1.f, 1.f, 0.f);
		vertices[4].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 우 하단
		vertices[5].Pos = SimpleMath::Vector3(1.f, -1.f, 0.f);
		vertices[5].Tex = SimpleMath::Vector2(1.f, 1.f);

		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(6));

		for (int i = 0; i < 6; i++)
			indices[0][i] = i;

		auto mesh = CreateMesh(
			vertices,
			indices,
			"ScreenMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		basicMeshes.insert(std::make_pair("ScreenMesh", mesh));
	}

	void ResourceManager::LoadDebugScreenMesh()
	{
		// Debug Screen Mesh 생성
		std::vector<VertexStruct::PBRStaticVertex> vertices(6);

		// 좌 하단
		vertices[0].Pos = SimpleMath::Vector3(-1.f, -0.6f, 0.f);
		vertices[0].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1].Pos = SimpleMath::Vector3(-1.f, 1.f, 0.f);
		vertices[1].Tex = SimpleMath::Vector2(0.f, 0.f);

		// 우 상단
		vertices[2].Pos = SimpleMath::Vector3(1.f, 1.f, 0.f);
		vertices[2].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3].Pos = SimpleMath::Vector3(-1.f, -0.6f, 0.f);
		vertices[3].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 우 상단
		vertices[4].Pos = SimpleMath::Vector3(1.f, 1.f, 0.f);
		vertices[4].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 우 하단
		vertices[5].Pos = SimpleMath::Vector3(1.f, -0.6f, 0.f);
		vertices[5].Tex = SimpleMath::Vector2(1.f, 1.f);

		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(6));

		for (int i = 0; i < 6; i++)
			indices[0][i] = i;

		auto mesh = CreateMesh(
			vertices,
			indices,
			"DebugScreenMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		basicMeshes.insert(std::make_pair("DebugScreenMesh", mesh));
	}

	void ResourceManager::LoadWireCubeMesh()
	{
		float w2 = 0.5f;
		float h2 = 0.5f;
		float d2 = 0.5f;

		std::vector<VertexStruct::PosVertex> vertices(24);

		// 앞면
		vertices[0]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, -d2));
		vertices[1]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, -d2));
		vertices[2]  = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, -d2));
		vertices[3]  = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, -d2));
		// 뒷면
		vertices[4]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, +d2));
		vertices[5]  = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, +d2));
		vertices[6]  = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, +d2));
		vertices[7]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, +d2));
		// 윗면
		vertices[8]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, -d2));
		vertices[9]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, +d2));
		vertices[10] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, +d2));
		vertices[11] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, -d2));
		// 아랫면
		vertices[12] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, -d2));
		vertices[13] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, -d2));
		vertices[14] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, +d2));
		vertices[15] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, +d2));
		// 왼쪽면
		vertices[16] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, +d2));
		vertices[17] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, +d2));
		vertices[18] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, -d2));
		vertices[19] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, -d2));
		// 오른쪽면						
		vertices[20] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, -d2));
		vertices[21] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, -d2));
		vertices[22] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, +d2));
		vertices[23] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, +d2));

		std::vector<std::vector<unsigned int>> idx(1, std::vector<unsigned int>(36));

		// 앞면
		idx[0][0] = 0; idx[0][1] = 1; idx[0][2] = 2;
		idx[0][3] = 0; idx[0][4] = 2; idx[0][5] = 3;
		// 뒷면
		idx[0][6] = 4; idx[0][7] = 5; idx[0][8] = 6;
		idx[0][9] = 4; idx[0][10] = 6; idx[0][11] = 7;
		// 윗면
		idx[0][12] = 8; idx[0][13] = 9; idx[0][14] = 10;
		idx[0][15] = 8; idx[0][16] = 10; idx[0][17] = 11;
		// 아랫면
		idx[0][18] = 12; idx[0][19] = 13; idx[0][20] = 14;
		idx[0][21] = 12; idx[0][22] = 14; idx[0][23] = 15;
		// 왼쪽면
		idx[0][24] = 16; idx[0][25] = 17; idx[0][26] = 18;
		idx[0][27] = 16; idx[0][28] = 18; idx[0][29] = 19;
		// 오른쪽면
		idx[0][30] = 20; idx[0][31] = 21; idx[0][32] = 22;
		idx[0][33] = 20; idx[0][34] = 22; idx[0][35] = 23;

		auto wireCubemesh = CreateMesh(
			vertices,
			idx,
			"WireCubeMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetWireFrameRS()
		);

		basicMeshes.insert(std::make_pair("WireCubeMesh", wireCubemesh));

		auto skyMesh = CreateMesh(
			vertices,
			idx,
			"SkyBoxMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		basicMeshes.insert(std::make_pair("SkyBoxMesh", skyMesh));
	}

	void ResourceManager::LoadWireSphereMesh()
	{
		float radius = 1.0f;	// 구의 반지름
		uint32 stackCount = 20; // 가로 분할
		uint32 sliceCount = 20; // 세로 분할

		std::vector<VertexStruct::PosVertex> vertices;

		VertexStruct::PosVertex v;

		// 북극
		v.Pos = SimpleMath::Vector3(0.0f, radius, 0.0f);
		vertices.push_back(v);

		float stackAngle = XM_PI / stackCount;
		float sliceAngle = XM_2PI / sliceCount;

		float deltaU = 1.f / static_cast<float>(sliceCount);
		float deltaV = 1.f / static_cast<float>(stackCount);

		// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
		for (uint32 y = 1; y <= stackCount - 1; ++y)
		{
			float phi = y * stackAngle;

			// 고리에 위치한 정점
			for (uint32 x = 0; x <= sliceCount; ++x)
			{
				float theta = x * sliceAngle;

				v.Pos.x = radius * sinf(phi) * cosf(theta);
				v.Pos.y = radius * cosf(phi);
				v.Pos.z = radius * sinf(phi) * sinf(theta);

				vertices.push_back(v);
			}
		}

		// 남극
		v.Pos = SimpleMath::Vector3(0.0f, -radius, 0.0f);
		vertices.push_back(v);

		std::vector<std::vector<unsigned int>> idx(1);

		// 북극 인덱스
		for (uint32 i = 0; i <= sliceCount; ++i)
		{
			//  [0]
			//   |  \
			//  [i+1]-[i+2]
			idx[0].push_back(0);
			idx[0].push_back(i + 2);
			idx[0].push_back(i + 1);
		}

		// 몸통 인덱스
		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 y = 0; y < stackCount - 2; ++y)
		{
			for (uint32 x = 0; x < sliceCount; ++x)
			{
				//  [y, x]-[y, x+1]
				//  |		/
				//  [y+1, x]
				idx[0].push_back(1 + (y)*ringVertexCount + (x));
				idx[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				//		 [y, x+1]
				//		 /	  |
				//  [y+1, x]-[y+1, x+1]
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				idx[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x + 1));
			}
		}

		// 남극 인덱스
		uint32 bottomIndex = static_cast<uint32>(vertices.size()) - 1;
		uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
		for (uint32 i = 0; i < sliceCount; ++i)
		{
			//  [last+i]-[last+i+1]
			//  |      /
			//  [bottom]
			idx[0].push_back(bottomIndex);
			idx[0].push_back(lastRingStartIndex + i);
			idx[0].push_back(lastRingStartIndex + i + 1);
		}

		auto mesh = CreateMesh(
			vertices,
			idx,
			"WireSphereMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetWireFrameRS()
		);

		basicMeshes.insert(std::make_pair("WireSphereMesh", mesh));
	}

	void ResourceManager::LoadCapsuleMesh()
	{
		float radius = 0.5f;	// 구의 반지름
		float height = 0.5f;	// 캡슐의 높이 => 해당 높이 + 지름 => 캡슐 전체의 높이
		uint32 stackCount = 20; // 가로 분할
		uint32 sliceCount = 20; // 세로 분할

		std::vector<VertexStruct::PosVertex> vertices;

		VertexStruct::PosVertex v;

		// 북극
		v.Pos = SimpleMath::Vector3(0.0f, radius + height, 0.0f);
		vertices.push_back(v);

		float stackAngle = XM_PI / stackCount;
		float sliceAngle = XM_2PI / sliceCount;

		float deltaU = 1.f / static_cast<float>(sliceCount);
		float deltaV = 1.f / static_cast<float>(stackCount);

		// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
		for (uint32 y = 1; y <= stackCount - 1; ++y)
		{
			float phi = y * stackAngle;

			// 고리에 위치한 정점
			for (uint32 x = 0; x <= sliceCount; ++x)
			{
				float theta = x * sliceAngle;
				
				v.Pos.x = radius * sinf(phi) * cosf(theta);
				if(y < 10)
					v.Pos.y = radius * cosf(phi) + height;
				else
					v.Pos.y = radius * cosf(phi) - height;
				v.Pos.z = radius * sinf(phi) * sinf(theta);

				vertices.push_back(v);
			}
		}

		// 남극
		v.Pos = SimpleMath::Vector3(0.0f, -(radius + height), 0.0f);
		vertices.push_back(v);

		std::vector<std::vector<unsigned int>> idx(1);

		// 북극 인덱스
		for (uint32 i = 0; i <= sliceCount; ++i)
		{
			//  [0]
			//   |  \
			//  [i+1]-[i+2]
			idx[0].push_back(0);
			idx[0].push_back(i + 2);
			idx[0].push_back(i + 1);
		}

		// 몸통 인덱스
		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 y = 0; y < stackCount - 2; ++y)
		{
			for (uint32 x = 0; x < sliceCount; ++x)
			{
				//  [y, x]-[y, x+1]
				//  |		/
				//  [y+1, x]
				idx[0].push_back(1 + (y)*ringVertexCount + (x));
				idx[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				//		 [y, x+1]
				//		 /	  |
				//  [y+1, x]-[y+1, x+1]
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				idx[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x + 1));
			}
		}

		// 남극 인덱스
		uint32 bottomIndex = static_cast<uint32>(vertices.size()) - 1;
		uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
		for (uint32 i = 0; i < sliceCount; ++i)
		{
			//  [last+i]-[last+i+1]
			//  |      /
			//  [bottom]
			idx[0].push_back(bottomIndex);
			idx[0].push_back(lastRingStartIndex + i);
			idx[0].push_back(lastRingStartIndex + i + 1);
		}

		auto mesh = CreateMesh(
			vertices,
			idx,
			"WireCapsuleMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetWireFrameRS()
		);

		basicMeshes.insert(std::make_pair("WireCapsuleMesh", mesh));
	}

	void ResourceManager::LoadPlaneMesh()
	{
		// Plane Mesh 생성
		std::vector<VertexStruct::PBRStaticVertex> vertices(6);

		// 좌 하단
		vertices[0].Pos = SimpleMath::Vector3(-5.f, 0.f, -5.f);
		vertices[0].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[0].Tex = SimpleMath::Vector2(0.f, 1.f);
		vertices[0].Color = SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);

		// 좌 상단
		vertices[1].Pos = SimpleMath::Vector3(-5.f, 0.f, 5.f);
		vertices[1].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[1].Tex = SimpleMath::Vector2(0.f, 0.f);
		vertices[1].Color = SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);

		// 우 상단
		vertices[2].Pos = SimpleMath::Vector3(5.f, 0.f, 5.f);
		vertices[2].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[2].Tex = SimpleMath::Vector2(1.f, 0.f);
		vertices[2].Color = SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);

		// 좌 하단
		vertices[3].Pos = SimpleMath::Vector3(-5.f, 0.f, -5.f);
		vertices[3].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[3].Tex = SimpleMath::Vector2(0.f, 1.f);
		vertices[3].Color = SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);

		// 우 상단
		vertices[4].Pos = SimpleMath::Vector3(5.f, 0.f, 5.f);
		vertices[4].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[4].Tex = SimpleMath::Vector2(1.f, 0.f);
		vertices[4].Color = SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);

		// 우 하단
		vertices[5].Pos = SimpleMath::Vector3(5.f, 0.f, -5.f);
		vertices[5].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[5].Tex = SimpleMath::Vector2(1.f, 1.f);
		vertices[5].Color = SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);

		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(6));

		for (int i = 0; i < 6; i++)
			indices[0][i] = i;

		auto mesh = CreateMesh(
			vertices,
			indices,
			"PlaneMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		basicMeshes.insert(std::make_pair("PlaneMesh", mesh));
	}

	void ResourceManager::LoadPrefab()
	{
		
	}

	void ResourceManager::MakeMesh(std::string name, std::vector<FBXBinaryData::VertexData> vertex, std::vector<std::vector<unsigned int>> indices)
	{
		// 내부에 저장된 기본 메시를 찾는지 확인한다.
		auto findIt = basicMeshes.find(name);

		// 있으면 만들지마!
		if (findIt != basicMeshes.end())
			return;

		std::vector<VertexStruct::PBRStaticVertex> vertices(vertex.size());

		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i].Pos = SimpleMath::Vector3{ vertex[i].position.x, vertex[i].position.y, vertex[i].position.z };
			vertices[i].Color = SimpleMath::Vector4{ vertex[i].color.x, vertex[i].color.y, vertex[i].color.z, vertex[i].color.w };
		}

		auto mesh = CreateMesh(
			vertices,
			indices,
			name,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);

		basicMeshes.insert(std::make_pair(name, mesh));
	}

	std::shared_ptr<YAMLBinaryData::Scene> ResourceManager::LoadUnityScene(std::string path)
	{
		std::ifstream ifs(path, std::ios_base::binary);
		boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
		buffer.push(boost::iostreams::zlib_decompressor());
		buffer.push(ifs);
		boost::archive::binary_iarchive ia(buffer);

		std::shared_ptr<YAMLBinaryData::Scene> sceneData;

		ia >> sceneData;

		return sceneData;
	}
}