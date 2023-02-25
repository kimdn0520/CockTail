#include "GamePch.h"
#include "NavigationManager.h"

#include "Component/Transform.h"
#include "Component/MeshRenderer.h"
#include "Component/Camera.h"
#include "Object/GameObject.h"
#include "SceneBase.h"

#include "SceneManager.h"
#include "GraphicsManager.h"
#include "WindowManager.h"

namespace GameEngineSpace
{
	std::shared_ptr<NavigationManager> NavigationManager::instance = nullptr;

	std::shared_ptr<NavigationManager> NavigationManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<NavigationManager>();

		return instance;
	}

	void NavigationManager::Release()
	{
		//nav.reset();

		if (isNavMesh)
		{
			dtFreeNavMesh(navMesh);
			navMesh = nullptr;
		}

		if (navQuery)
		{
			dtFreeNavMeshQuery(navQuery);
			navQuery = nullptr;
		}

		isNavMesh = false;
	}

	void GameEngineSpace::NavigationManager::MakeNavMesh(std::string navMeshName)
	{
		currentNavMeshName = navMeshName;

		std::string path = "BinaryFile/NavMesh/" + navMeshName + ".bin";

		FILE* fp = fopen(path.c_str(), "rb");

		// Read header.
		NavMeshSetHeader header;
		
		size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
		if (readLen != 1)
		{
			fclose(fp);
		}
		if (header.magic != NAVMESHSET_MAGIC)
		{
			fclose(fp);
		}
		if (header.version != NAVMESHSET_VERSION)
		{
			fclose(fp);
		}

		navMesh = dtAllocNavMesh();

		if (!navMesh)
		{
			fclose(fp);
		}

		dtStatus status = navMesh->init(&header.params);

		if (dtStatusFailed(status))
		{
			fclose(fp);
		}

		// Read tiles.
		for (int i = 0; i < header.numTiles; ++i)
		{
			NavMeshTileHeader tileHeader;
			readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
			if (readLen != 1)
			{
				fclose(fp);
			}

			if (!tileHeader.tileRef || !tileHeader.dataSize)
				break;

			unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
			if (!data) break;
			memset(data, 0, tileHeader.dataSize);
			readLen = fread(data, tileHeader.dataSize, 1, fp);
			if (readLen != 1)
			{
				dtFree(data);
				fclose(fp);
			}

			// addTile : 탐색메시에 타일을 추가합니다.
			navMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
		}

		navQuery = dtAllocNavMeshQuery();

		// Init(모든 쿼리에 사용할 dtNavMesh에 대한 포인터, 최대 검색 노드 수 [Limits: 0 < value <= 65535])
		navQuery->init(navMesh, 2048);

		// default filter
		filter.setAreaCost(POLYAREA_GROUND, 1.0f);
		filter.setAreaCost(POLYAREA_WATER, 1.0f);
		filter.setAreaCost(POLYAREA_ROAD, 1.0f);
		filter.setAreaCost(POLYAREA_GRASS, 1.0f);
		filter.setAreaCost(POLYAREA_JUMP, 1.0f);
		
		fclose(fp);

		isDebugRender == true ? false : true;

		if (navMesh == nullptr && isDebugRender)
			return;

		auto scene = SceneManager::GetInstance()->GetCurrentScene();

		vertexDataList.clear();
		indices.clear();
		indices.resize(1);
		idx_offset = 0;

		FBXBinaryData::Float4 color = { 0.29f, 0.65f, 0.84f, 1.f };

		const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

		const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

		DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

		SimpleMath::Matrix rotMatrix = XMMatrixIdentity();

		rotMatrix *= XMMatrixRotationQuaternion(q);

		for (int i = 0; i < navMesh->getMaxTiles(); ++i)
		{
			const dtMeshTile* tile = navMesh->getTile(i);
			if (!tile->header) continue;

			for (int j = 0; j < tile->header->polyCount; ++j)
			{
				const dtPoly* poly = &tile->polys[j];

				if (poly->getType() == DT_POLYTYPE_OFFMESH_CONNECTION)
					continue;

				FBXBinaryData::Float3 position;

				for (int k = 0; k < poly->vertCount; ++k)
				{
					float* v = &tile->verts[poly->verts[k] * 3];

					DirectX::XMFLOAT3 vert = { v[0], v[2], v[1] };
					DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&vert);
					vec = DirectX::XMVector3Transform(vec, rotMatrix);
					DirectX::XMStoreFloat3(&vert, vec);

					position.x = vec.m128_f32[0];
					position.y = vec.m128_f32[1];
					position.z = vec.m128_f32[2];

					FBXBinaryData::VertexData vertexData(position, color);
					vertexDataList.push_back(vertexData);
				}

				for (int k = 2; k < poly->vertCount; k++)
				{
					indices[0].push_back(0 + idx_offset);
					indices[0].push_back(k - 1 + idx_offset);
					indices[0].push_back(k + idx_offset);
				}

				idx_offset += poly->vertCount;
			}
		}

		isNavMesh = true;

		/*GraphicsManager::GetInstance()->MakeMesh(currentNavMeshName, vertexDataList, indices);

		nav = scene->CreateEmpty();
		nav->AddComponent<MeshRenderer>()->Init(currentNavMeshName);

		nav->SetGameObjectEnable(isDebugRender);*/
	}

	void NavigationManager::DebugRenderNavMesh()
	{
		if(!isNavMesh)
			return;

		/*isDebugRender = !isDebugRender;

		if (nav != nullptr)
			nav->SetGameObjectEnable(isDebugRender);*/

		if (ImGUIManager::GetInstance()->DrawBegin("DrawNavMesh"))
		{
			std::vector<SimpleMath::Vector4> vertexVector4;

			for (auto& v : vertexDataList)
			{
				vertexVector4.push_back({ v.position.x, v.position.y, v.position.z, 1.0f });
			}

			auto camera = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

			XMMATRIX viewProj = XMMatrixMultiply(camera->GetViewMatrix(), camera->GetProjMatrix());

			for (int i = 0; i < indices[0].size(); i += 3)
			{
				Vector4 screenPos = XMVector4Transform(XMLoadFloat4(&vertexVector4[indices[0][i]]), viewProj);
				Vector4 screenPos2 = XMVector4Transform(XMLoadFloat4(&vertexVector4[indices[0][i + 1]]), viewProj);
				Vector4 screenPos3 = XMVector4Transform(XMLoadFloat4(&vertexVector4[indices[0][i + 2]]), viewProj);

				// Divide the x and y values of the screen space positions by their w value
				screenPos = XMVectorDivide(screenPos, XMVectorSplatW(screenPos));
				screenPos2 = XMVectorDivide(screenPos2, XMVectorSplatW(screenPos2));
				screenPos3 = XMVectorDivide(screenPos3, XMVectorSplatW(screenPos3));

				// -1 ~ 1 구간을 0 ~ 1 구간으로 변환
				screenPos += {1.f, 1.f, 1.f, 1.f};
				screenPos2 += {1.f, 1.f, 1.f, 1.f};
				screenPos3 += {1.f, 1.f, 1.f, 1.f};

				// y를 뒤집어줌
				screenPos = screenPos * 0.5f;
				screenPos.y = 1.f - screenPos.y;
				screenPos2 = screenPos2 * 0.5f;
				screenPos2.y = 1.f - screenPos2.y;
				screenPos3 = screenPos3 * 0.5f;
				screenPos3.y = 1.f - screenPos3.y;

				// Multiply the x and y values by the size of the viewport in pixels
				int width, height;
				width = WindowManager::GetInstance()->GetClientWidth();
				height = WindowManager::GetInstance()->GetClientHeight();

				screenPos = XMVectorMultiply(screenPos, XMVectorSet(width, height, 0.0f, 0.0f));
				screenPos2 = XMVectorMultiply(screenPos2, XMVectorSet(width, height, 0.0f, 0.0f));
				screenPos3 = XMVectorMultiply(screenPos3, XMVectorSet(width, height, 0.0f, 0.0f));

				GraphicsEngineSpace::ImGUIManager::GetInstance()->DrawTriangle({ screenPos.x, screenPos.y, screenPos.z }, { screenPos2.x, screenPos2.y, screenPos2.z }, { screenPos3.x, screenPos3.y, screenPos3.z });
			}
		}

		ImGUIManager::GetInstance()->End();
	}
}