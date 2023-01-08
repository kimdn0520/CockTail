#pragma once
#include "ColliderBase.h"

namespace GameEngineSpace
{
	/**
	 * \brief 네비게이션 메시에 작동하는 컴포넌트
	 *	..
	 *
	 * 2023.01.08(일) Dev.Newbie
	 */
	class NavMeshAgent : public ComponentBase
	{
	private:
		// Agent Size
		float radius;
		float height;
		float baseOffset;		// 변환 피벗점을 기준으로 충돌 원통의 오프셋입니다.

		// Steering
		float speed;			// 최대 이동 속도
		float angularSpeed;		// 최대 각속도
		float acceleration;		// 최대 가속도
		float stopDistance;		// 목표에 가까워졌을때 중지 거리
		bool autoBreaking;		// 활성화되면 에이전트가 대상에 도달할 때 속도가 느려짐

	public:
		NavMeshAgent(std::weak_ptr<GameObject> _gameObject);
		virtual ~NavMeshAgent();

		void Release() override;
	};
}
