#pragma once
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	class ColliderBase;

	/**
	 * \brief CometEngine의 PhysicsManager를 사용한 physicsManager
	 *
	 * 충돌, 물리와 관련된 처리를 수행함.
	 * 현재는 간단한 충돌 처리만 관리.
	 *
	 * 2022.08.03(수)
	 */
	class PhysicsManager final
	{
	private:
		// 인스턴스로 사용해줄 피직스 매니저..
		static std::shared_ptr<PhysicsManager> instance;

	public :
		PhysicsManager() = default;
		~PhysicsManager() = default;
		PhysicsManager(const PhysicsManager& other) = delete;
		PhysicsManager(const PhysicsManager&& other) = delete;
		PhysicsManager& operator= (const PhysicsManager& other) = delete;
		PhysicsManager& operator= (const PhysicsManager&& other) = delete;

		static std::shared_ptr<PhysicsManager> GetInstance();

		void Init();

		// 벡터를 레퍼런스로..
		void Update(float deltaTime, std::vector<std::shared_ptr<ColliderBase>>& colliderList);

		void Release();

	private:
		// 충돌 체크.
		void CheckCollisionAll(std::vector<std::shared_ptr<ColliderBase>>& colliderList);

		// 콜라이더들을 리셋
		void ResetColliders(std::vector<std::shared_ptr<ColliderBase>>& colliderList);

		// 정보를 날려준다.
			// -> onTrigger, ExitTrigger
		void BroadCastTriggerEvents(std::vector<std::shared_ptr<ColliderBase>>& colliderList);

	};

}
