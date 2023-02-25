#pragma once
#include "FBXBinaryLayout.h"

namespace GameEngineSpace
{
	class GameObject;

	// 필요에 따라 이러한 기반을 지정해야한다.
	enum PolyAreas
	{
		POLYAREA_GROUND,
		POLYAREA_WATER,
		POLYAREA_ROAD,
		POLYAREA_DOOR,
		POLYAREA_GRASS,
		POLYAREA_JUMP
	};

	enum PolyFlags
	{
		POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
		POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
		POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
		POLYFLAGS_JUMP = 0x08,		// Ability to jump.
		POLYFLAGS_DISABLED = 0x10,	// Disabled polygon
		POLYFLAGS_ALL = 0xffff		// All abilities.
	};

	constexpr const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
	constexpr const int NAVMESHSET_VERSION = 1;

	struct NavMeshSetHeader
	{
		int magic;
		int version;
		int numTiles;
		dtNavMeshParams params;
	};

	struct NavMeshTileHeader
	{
		dtTileRef tileRef;
		int dataSize;
	};

	/**
	 * \brief Navigation.bin을 불러와서 적용하는 클래스
	 *
	 * 해당 매니저 클래스를 실제로 게임엔진에서 가지고 있다. 싱글톤으로 구현한다.
	 *
	 * 2023.01.08(일) Dev.Newbie
	 */
	class NavigationManager final
	{
		// 단 하나 존재하는 SceneManager 객체
		static std::shared_ptr<NavigationManager> instance;

	private:
		bool isDebugRender = false;

		//std::shared_ptr<GameObject> nav;

		std::vector<FBXBinaryData::VertexData> vertexDataList;
		std::vector<std::vector<unsigned int>> indices;
		unsigned int idx_offset = 0;

		bool isNavMesh = false;

	public:
		dtNavMesh* navMesh;
		dtNavMeshQuery* navQuery;
		dtQueryFilter filter;				// 쿼리에 적용할 폴리곤 필터

		std::string currentNavMeshName = "";

	public:
		NavigationManager() = default;

		~NavigationManager() = default;

		// 이동, 복사 생성자 제거
		NavigationManager(const NavigationManager& _other) = delete;
		NavigationManager& operator= (const NavigationManager& _other) = delete;
		NavigationManager(NavigationManager&& _other) = delete;
		NavigationManager& operator= (NavigationManager&& _other) = delete;

		static std::shared_ptr<NavigationManager> GetInstance();
	
	public:
		void Release();

		// NavMesh를 불러온다.
		void MakeNavMesh(std::string navMeshName);

		// NavMesh 를 그려주고 싶다. 가능할까..?
		void DebugRenderNavMesh();
	};
}

