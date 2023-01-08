#pragma once
#include "ColliderBase.h"

namespace GraphicsEngineSpace
{
	class IDXObject;
}

namespace GameEngineSpace
{

	class SphereCollider;

	/**
	 * \brief 분리축 이론을 이용, OBB로 박스 충돌을 검사하는 콜라이더 클래스
	 *
	 * ref by CometEngine
	 *
	 * 2022.08.04(목) Dev.BlackFoot
	 */
	class BoxCollider : public ColliderBase
	{
	private:
		// 기본적으로 현재 꼭지점의 월드 위치를 알기 위해서 로테이션 행렬이 필요하다.
			// 회전된 직육면체의 충돌을 검사하기 위함.
		Matrix objWorldRotTM;
		// 로컬과 월드의 센터
		Vector3 localCenter;
		Vector3 worldCenter;

		// 분리축들의 벡터 => 검사를 진행할 축들의 노말 벡터.
		std::vector<Vector3> worldNormals;

		// 박스 하나가 가지고 있는 사이즈 == radius
		Vector3 size;
		Vector3 tmpSize;

		// 각 축이 가지고 있는 절반의 크기 => 즉 센터에서 얼마만큼 거리로 떨어져있는가..
		std::vector<float> extents;

		std::shared_ptr<GraphicsEngineSpace::IDXObject> colliderRenderObj;

	public:
		BoxCollider(std::weak_ptr<GameObject> gameObj);
		virtual ~BoxCollider();

		void Release() override;

	private:
		// 우선 박스간의 충돌만...
		bool CheckForBox(std::shared_ptr<BoxCollider> other);

		// 이후에 코드 참고해서 구 충돌도..
		bool CheckForSphere(std::shared_ptr<SphereCollider> other);

	public:
		// Getter Setter
		const Vector3& GetLocalCenter() { return localCenter; }
		const Vector3& GetWorldCenter() { return worldCenter; }
		const Vector3& GetSize() { return size; }

		void SetLocalCenter(const Vector3& newCenter) { localCenter = newCenter; }
		void SetSize(const Vector3& newSize);

		// 충돌 체크
		bool CheckCollision(std::shared_ptr<ColliderBase> other) override;

		void InitRenderObj(std::string _objName);
		// 콜라이더 렌더
		void ColliderRender() override;

		void LateUpdate(float tick) override;

		friend class PhysicsManager;

		friend class SphereCollider;
		friend class RayCollider;
	};

}
