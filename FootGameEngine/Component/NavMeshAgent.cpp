#include "GamePch.h"
#include "NavMeshAgent.h"

namespace GameEngineSpace
{
	NavMeshAgent::NavMeshAgent(std::weak_ptr<GameObject> _gameObject)
		: ComponentBase(_gameObject, ComponentType::NAVAGENT)
		, radius(0.5f)
		, height(2.0f)
		, baseOffset(1.0f)
		, speed(3.5f)
		, angularSpeed(120.f)
		, acceleration(8.0f)
		, stopDistance(0.f)
		, autoBreaking(true)
	{}

	NavMeshAgent::~NavMeshAgent()
	{}

	void NavMeshAgent::Release()
	{
		__super::Release();
	}
}
