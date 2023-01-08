#pragma once
#include "ColliderBase.h"

namespace GameEngineSpace
{
	class BoxCollider;

	/**
	 * \brief 구 충돌을 판정하는 콜라이더 컴포넌트
	 * 따로 바운딩 볼륨은 사용하지 않고.. 중심과 거리로 판별한다.
	 *
	 * 2022.08.03 DevBlackFoot.
	 */
	class SphereCollider : public ColliderBase
	{
	private:
		// 해당 오브젝트의 로컬 상에서의 중앙값
		Vector3 localCenter;
		// 월드에서의 중앙값
		Vector3 worldCenter;
		// 반지름
		float radius;

	public:
		SphereCollider(std::weak_ptr<GameObject> gameObj);
		virtual ~SphereCollider();

	private:
		// 각 경우의 충돌 체크
		bool CheckForSphere(std::shared_ptr<SphereCollider> other);
		bool CheckForBox(std::shared_ptr<BoxCollider> other);

	public:
		void Release() override;

		// Getter & Setter
		const Vector3& GetLocalCenter() { return localCenter; }
		const Vector3& GetWorldCenter() { return worldCenter; }
		const float& GetRadius() { return radius; }

		void SetLocalCenter(const Vector3& newCenter) { localCenter = newCenter; }
		void SetRadius(const float& _radius) { radius = _radius; }

		// 충돌 체크
		bool CheckCollision(std::shared_ptr<ColliderBase> other) override;

		// 콜라이더 렌더
		void ColliderRender() override;

		void LateUpdate(float tick) override;

		friend class PhysicsManager;

		friend class BoxCollider;
		friend class RayCollider;
	};

}
