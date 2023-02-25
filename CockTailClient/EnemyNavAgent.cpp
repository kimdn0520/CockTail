#include "pch.h"
#include "EnemyNavAgent.h"
#include "Scripts/PlayerController.h"
#include "EnemyController.h"

namespace ClientSpace
{

	EnemyNavAgent::EnemyNavAgent(std::weak_ptr<GameObject> _object)
		: ComponentBase(_object)
		, moveSpeed(6.0f)
		, bSet(true)
	{
		GetGameObject()->AddComponent<NavMeshAgent>();
		transform = GetGameObject()->GetComponent<Transform>();
		meshRenderer = GetGameObject()->GetComponent<MeshRenderer>();
		navMeshAgent = GetGameObject()->GetComponent<NavMeshAgent>();
	}

	EnemyNavAgent::~EnemyNavAgent()
	{

	}

	void EnemyNavAgent::Awake()
	{
	}

	void EnemyNavAgent::Start()
	{
		controller = GetGameObject()->GetComponent<EnemyController>();
		player = SceneManager::GetInstance()->FindObjectName(L"Player");
		targetTransform = player->GetComponent<Transform>();
		playerController = player->GetComponent<PlayerController>();
	}

	void EnemyNavAgent::PreUpdate(float tick)
	{

	}

	void EnemyNavAgent::Update(float tick)
	{
		if (bSet == true)
		{
			if (targetTransform == nullptr)
				return;

			navMeshAgent->TrackingStatus(true);
			navMeshAgent->SetSpeed(moveSpeed);
			Chasetarget(tick);
			controller->SetStatus(EnemyStatus::MOVE);
		}

		else if (bSet == false)
		{
			navMeshAgent->TrackingStatus(false);
			/*	if (timer != 0.f)
				{
					if (timer > currentTimer)
						currentTimer += tick;

					else
					{
						timer = 0.0f;
						currentTimer = 0.0f;
						bSet = true;
					}
				}*/
		}
	}

	void EnemyNavAgent::LateUpdate(float tick)
	{

	}

	void EnemyNavAgent::SetTrackingStatus(bool value)
	{
		navMeshAgent->TrackingStatus(value);
	}

	void EnemyNavAgent::Chasetarget(float tick)
	{
		navMeshAgent->SetDestination(targetTransform->GetWorldPosition());
	}

}
