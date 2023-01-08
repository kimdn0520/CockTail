#include "pch.h"
#include "MoveController.h"

#include "InputManager.h"

namespace ClientSpace
{
	MoveController::MoveController(std::weak_ptr<GameObject> obj)
		: ComponentBase(obj)
		, moveSpeed(5.0f)
	{
		targetTransform = GetGameObject()->GetComponent<Transform>();
		meshRenderer = GetGameObject()->GetComponent<MeshRenderer>();
		input = InputManager::GetInstance();
	}

	MoveController::~MoveController()
	{}

	void MoveController::Awake()
	{
		
	}

	void MoveController::Start()
	{
		meshRenderer->PlayAnim("redani_redtest", true); // animtest
		//meshRenderer->PlayAnim("TestPlayer_Run", true);		// run
		//meshRenderer->PlayAnim("Armature|Armature|mixamo.com|Layer0", true);
		//meshRenderer->PlayAnim("TestPlayer_Idle", true);		// idle
	}

	void MoveController::PreUpdate(float tick)
	{
		float delta = tick;

		if (InputManager::GetInstance()->GetInputState(VK_UP, KeyState::STAY))
		{
			// Vector3::Forward 왜 0, 0, -1 임? ㅋㅋ
			targetTransform->TransLate(Vector3(0, 0, 1) * moveSpeed * delta);
		}

		if (InputManager::GetInstance()->GetInputState(VK_DOWN, KeyState::STAY))
		{
			// Vector3::Backward 왜 0, 0, 1 임? ㅋㅋ
			targetTransform->TransLate(Vector3(0, 0, -1) * moveSpeed * delta);
		}

		if (InputManager::GetInstance()->GetInputState(VK_RIGHT, KeyState::STAY)) 
		{
			targetTransform->TransLate(Vector3::Right * moveSpeed * delta);
		}

		if (InputManager::GetInstance()->GetInputState(VK_LEFT, KeyState::STAY))
		{
			targetTransform->TransLate(Vector3::Right * -moveSpeed * delta);
		}

		//targetTransform->Rotate(Vector3(1.f, 0.f, 0.f));

		//targetTransform->SetRotation(targetTransform->GetWorldRotation() + Vector3(1.f, 0.f, 0.f));
	}
}
