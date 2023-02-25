#pragma once
#include "Collider.h"
#include "PhysicsManager.h"

namespace GameEngineSpace
{
	class Camera;
	/**
	 * \brief 네비게이션 메시에 작동하는 컴포넌트
	 * Recast & Detour Library
	 * 이렇게 하는지는 모르겠는데 Recast & Detour Tool에서 NavMesh를 뽑게되면 오른손 좌표계라서 다 뒤집혀있어서
	 * start, end 를 Change해주고 경로를 구한다음에 해당 WayPoint도 Change 해줘서 값을 구했다.
	 * 원래는 NavMesh 받은 vertex들을 미리 뒤집어주면 되지 않을까 생각해서 적용해보았으나 vertex만 바꿔서는 적용이 되지않았다.
	 * 해당 타일들의 여러 값들을 더 바꿔줘야 하는것 같은데 아무리 찾아봐도 해결할 수 없었다.
	 * 2023.01.17(일) Dev.Newbie
	 */
	class NavMeshAgent : public ComponentBase
	{
	private:	
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Rigidbody> rigidbody;

		static const int MAX_POLYS = 256;
		static const int MAX_SMOOTH = 2048;
		static const int MAX_AGENTS = 100;

		// 쓸지말지 모르는 변수들..
		// Agent Size
		float radius;
		float height;
		float baseOffset;		// 변환 피벗점을 기준으로 충돌 원통의 오프셋입니다.

		// Steering
		float speed;			// 최대 이동 속도
		float rotateSpeed;
		float angularSpeed;		// 최대 각속도
		float acceleration;		// 최대 가속도
		float stopDistance;		// Agent가 목표를 추적하다가 목표 위치에 가까워졌을시 서서히 정지하는 근접 거리
		bool autoBreaking;		// 활성화되면 에이전트가 대상에 도달할 때 속도가 느려짐

		bool isEnabled;			// NavMeshAgent 끼리는 서로를 장애물로 인식함. 그래서 이건 추적을 멈추고 컴포넌트자체를 비활성화 해준다.

		bool isTrace;			// 추적 상태

		SimpleMath::Vector3 destination;

		SimpleMath::Vector3 currentDestination;

		bool isChangeDestination;

		dtPolyRef polys[MAX_POLYS];
		int npolys;
		float polyPickExt[3];	// 각 축의 탐색 거리

		float smoothPath[MAX_SMOOTH * 3];
		int nsmoothPath;

		float straightPath[MAX_POLYS * 3];					// 지나온 직선 경로
		unsigned char straightPathFlags[MAX_POLYS];
		dtPolyRef straightPathPolys[MAX_POLYS];
		int straightPathCount;

		std::vector<SimpleMath::Vector3> wayPoint;
		int wayPointIndex;
	
		SimpleMath::Matrix rotMatrix = XMMatrixIdentity();

		RaycastHit hitSlope;
		float slopeAngle;
		float maxSlopeAngle;
		Vector3 gravity;

	public:
		NavMeshAgent(std::weak_ptr<GameObject> _gameObject);
		virtual ~NavMeshAgent();

	public:
		inline SimpleMath::Vector3 ChangeCoordinate(float x, float y, float z);

		void SetDestination(SimpleMath::Vector3 pos);

		void TrackingStatus(bool _isTrace) { isTrace = _isTrace; }

		void SetSpeed(float _speed) { speed = _speed; }

		std::vector<SimpleMath::Vector3> GetWayPoint() { return wayPoint; }

		int GetWayPointIndex() { return wayPointIndex; }

		void Awake() override;

		void DebugIMGUIRender(float tick) override;

		void Update(float tick) override;

		bool IsSlope();
		
		bool IsGround();

		SimpleMath::Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal);

		void Release() override;
	};
}
