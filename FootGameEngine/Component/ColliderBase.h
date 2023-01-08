#pragma once
#include "Object/ComponentBase.h"

namespace GameEngineSpace
{
	class Transform;

	class PhysicsManager;

	/**
	 * \brief 모든 콜라이더들의 기본이 되는 콜라이더 베이스 클래스
	 *
	 * 기본적으로 피직스매니저 에서 관리한다.
	 *
	 * 2022.08.03(수) Dev.BlackFoot
	 */
	class ColliderBase : public ComponentBase, public std::enable_shared_from_this<ColliderBase>
	{
	protected:
		// 캐싱용 트랜스폼
		std::shared_ptr<Transform> transform;
		// 현재 충돌한 콜라이더들
		std::vector<std::shared_ptr<ColliderBase>> currColliedColList;
		// 이전에 충돌처리가 된 콜라이더들
		std::vector<std::shared_ptr<ColliderBase>> prevColliedColList;

		bool isTrigger;

		Vector3 color;

	public:
		ColliderBase(std::weak_ptr<GameObject> gameObj);
		virtual ~ColliderBase();

		void Awake() override;

		// 실제로 다형성을 이용해서 콜리전을 체크해주는 함수
			// 이 컴포넌트를 상속받는 콜라이더는 모두 콜리전 체크를 만들어 주어야한다.
			// 이후에 다양한 상황에 맞춰서 각 콜라이더 컴포넌트에 추가해주면 된다.
		virtual bool CheckCollision(std::shared_ptr<ColliderBase> other) abstract;

		// 다형성을 이용해서 콜라이더를 렌더하는 함수..
		virtual void ColliderRender() {}

		// 다형적 동작이 필요 없는 함수
		void SetColor(const Vector3& _color) { color = _color; };

		void ClearCurrColliedColList() { currColliedColList.clear(); }
		bool GetIsCollidedThisFrame() { return currColliedColList.size() != 0; }

		friend class PhysicsManager;
	};

}
