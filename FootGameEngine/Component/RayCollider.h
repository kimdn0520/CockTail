#pragma once

#include "ColliderBase.h"

namespace GameEngineSpace
{
	class BoxCollider;
	class SphereCollider;

	/**
		 * \brief 레이캐스팅 이후 충돌한 물체의 정보를 가지고 있는 레이 캐스트 Hit 클래스..
		 *
		 * 기본적으로는 맞은 물체의 게임오브젝트, 트랜스폼, name, 거리 정도를 가지고 있다.
		 */
	struct RaycastHit
	{
		std::shared_ptr<GameObject> hitGameObject;
		std::shared_ptr<Transform> hitTransform;
		float hitDist;
		tstring hitName;

		RaycastHit()
			: hitGameObject(nullptr)
			, hitTransform(nullptr)
			, hitDist(0.f)
			, hitName(TEXT("HitNone"))
		{
		}
	};

	/**
	 * \brief 레이 캐스팅을 위한 레이콜라이더 컴포넌트.
	 *
	 * 해당 컴포넌트가 붙어있는 게임오브젝트의 Look 벡터와 Center를 이용해서 Origin -> Dir로 향하는 레이를 쏜다.
	 *
	 * 2022.08.08(월) Dev.BlackFoot
	 */
	class RayCollider : public ColliderBase
	{
	private:
		// 위치 벡터와 방향 벡터.
		Vector3 localCenter;
		Vector3 worldCenter;
		Vector3 direction;

		// 가장 처음 충돌한 콜라이더 정보 저장할 공간
		RaycastHit hit;

		// 거리 값 저장할 공간
		std::vector<std::pair<std::shared_ptr<GameObject>, float>> currHitObjVec;

	public:
		RayCollider(std::weak_ptr<GameObject> gameObj);
		virtual ~RayCollider() = default;

		void Release() override;

	private:
		// 각 콜라이더 와의 충돌 여부
		bool CheckForBox(std::shared_ptr<BoxCollider> other);
		bool CheckForSphere(std::shared_ptr<SphereCollider> other);


	public:
		const Vector3& GetLocalCenter() { return localCenter; }
		const Vector3& GetWorldCenter() { return worldCenter; }
		const Vector3& GetDirection() { return direction; }

		const RaycastHit& GetHit() { return hit; }

		// Hit 설정
		void SetHit();
		void ClearHit();

		void SetLocalCenter(const Vector3& newCenter) { localCenter = newCenter; }
		void SetDirection(const Vector3& newDir) { direction = newDir; }
		// 충돌 체크
		bool CheckCollision(std::shared_ptr<ColliderBase> other) override;

		//void ColliderRender() override;

		void LateUpdate(float tick) override;

		friend class PhysicsManager;
		friend class SphereCollider;
		friend class BoxCollider;
	};

}
