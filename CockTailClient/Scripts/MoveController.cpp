#include "pch.h"
#include "MoveController.h"

#include "InputManager.h"

namespace ClientSpace
{
	MoveController::MoveController(std::weak_ptr<GameObject> obj)
		: ComponentBase(obj)
		, moveSpeed(5.0f)
	{
		transform = GetGameObject()->GetComponent<Transform>();
		meshRenderer = GetGameObject()->GetComponent<MeshRenderer>();
		navMeshAgent = GetGameObject()->GetComponent<NavMeshAgent>();
		camera = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		input = InputManager::GetInstance();
	}

	MoveController::~MoveController()
	{}

	void MoveController::Awake()
	{
		// root 설정
		//auto root = std::make_shared<SelectorNode>();

		// root의 자식으로 달 node 설정
		//auto sequence = std::make_shared<SequenceNode>();

		// Condition 노드를 추가하는 방법
		/*sequence->addChild(std::make_shared<ConditionNode>([]() 
		{ 
			OutputDebugString(L"조건 확인 \n");
			return true; 
		}));*/

		// 이렇게도 가능하다.
		/*auto condition = std::make_shared<ConditionNode>([]() 
		{ 
			std::cout << "Checking some condition" << std::endl; 
			return true; 
		});
		sequence->addChild(condition);*/
		
		// Action 노드를 추가하는 방법 (무조건 true를 반환해야함)
		/*sequence->addChild(std::make_shared<ActionNode>([]()
		{
			std::cout << "Checking condition A" << std::endl;
			return true;
		}));*/

		//root->addChild(sequence);

		// Run the tree
		//root->run();
	}

	void MoveController::Start()
	{
		//meshRenderer->PlayAnim("redani_redtest", true); // animtest
		//meshRenderer->PlayAnim("TestPlayer_Run", true);		// run
		//meshRenderer->PlayAnim("Armature|Armature|mixamo.com|Layer0", true);
		//meshRenderer->PlayAnim("TestPlayer_Idle", true);		// idle
	}

	void MoveController::PreUpdate(float tick)
	{
		float delta = tick;

		//if (InputManager::GetInstance()->GetInputState(VK_UP, KeyState::STAY))
		//{
		//	// Vector3::Forward 왜 0, 0, -1 임? ㅋㅋ
		//	transform->TransLate(Vector3(0, 0, 1) * moveSpeed * delta);
		//}

		//if (InputManager::GetInstance()->GetInputState(VK_DOWN, KeyState::STAY))
		//{
		//	// Vector3::Backward 왜 0, 0, 1 임? ㅋㅋ
		//	transform->TransLate(Vector3(0, 0, -1) * moveSpeed * delta);
		//}

		//if (InputManager::GetInstance()->GetInputState(VK_RIGHT, KeyState::STAY)) 
		//{
		//	transform->TransLate(Vector3::Right * moveSpeed * delta);
		//}

		//if (InputManager::GetInstance()->GetInputState(VK_LEFT, KeyState::STAY))
		//{
		//	transform->TransLate(Vector3::Right * -moveSpeed * delta);
		//}

		//transform->Rotate(Vector3(1.f, 0.f, 0.f));

		//transform->SetRotation(targetTransform->GetWorldRotation() + Vector3(1.f, 0.f, 0.f));
	}

	void MoveController::Update(float tick)
	{
		if(targetTransform != nullptr)
			navMeshAgent->SetDestination(targetTransform->GetWorldPosition());
	}
}
