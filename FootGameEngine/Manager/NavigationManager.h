#pragma once

namespace GameEngineSpace
{
	static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
	static const int NAVMESHSET_VERSION = 1;

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
	 * \brief Navigation을 적용하는 클래스
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

	public:
		NavigationManager() = default;

		~NavigationManager() = default;

		// 이동, 복사 생성자 제거
		NavigationManager(const NavigationManager& _other) = delete;
		NavigationManager& operator= (const NavigationManager& _other) = delete;
		NavigationManager(NavigationManager&& _other) = delete;
		NavigationManager& operator= (NavigationManager&& _other) = delete;

		static std::shared_ptr<NavigationManager> GetInstance();

		void Release();

		void MakeNavMesh(std::string navMeshName);
	};
}

