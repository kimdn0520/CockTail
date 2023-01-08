#include "GamePch.h"
#include "NavigationManager.h"
// Recast Navigation

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
		// TODO : Release »ý°¢! dtmesh free..?
	}

	void GameEngineSpace::NavigationManager::MakeNavMesh(std::string navMeshName)
	{
		std::string path = "BinaryFile/NavMesh/testNavMesh" + navMeshName + ".bin";

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

		dtNavMesh* dtmesh = dtAllocNavMesh();

		if (!dtmesh)
		{
			fclose(fp);
		}
		dtStatus status = dtmesh->init(&header.params);
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

			dtmesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
		}

		fclose(fp);
	}
}