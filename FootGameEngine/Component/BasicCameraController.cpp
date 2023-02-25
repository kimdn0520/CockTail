#include "GamePch.h"
#include "BasicCameraController.h"

#include "Timer.h"
#include "InputManager.h"
#include "NavigationManager.h"

#include "Object/GameObject.h"
#include "Camera.h"
#include "Transform.h"
//#include "UIManager.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{

	BasicCameraController::BasicCameraController(std::weak_ptr<GameObject> gameObj)
		: ComponentBase(gameObj, ComponentType::MONOBEHAVIOR), moveSpeed(10.0f)
	{
		transform = GetGameObject()->GetComponent<Transform>();
	}

	BasicCameraController::~BasicCameraController()
	{
	}

	void BasicCameraController::Release()
	{
		camera.reset();
		__super::Release();
	}

	void BasicCameraController::Awake()
	{
		// 카메라를 등록
		camera = GetGameObject()->GetComponent<Camera>();
		/*UIManager::GetInstance()->CreateUIText("CameraUpGE", "CameraLook : ", Vector{ 1.0f, 1.0f, 1.0f }, Vector{ 100.f, 200.f, 1.0f }, 0.f, Vector{ 1.f, 1.f, 1.f });
		UIManager::GetInstance()->CreateUIText("CameraLookGE", "CameraLook : ", Vector{1.0f, 1.0f, 1.0f}, Vector{100.f, 250.f, 1.0f}, 0.f, Vector{1.f, 1.f, 1.f});
		UIManager::GetInstance()->CreateUIText("CameraRightGE", "CameraLook : ", Vector{ 1.0f, 1.0f, 1.0f }, Vector{ 100.f, 300.f, 1.0f }, 0.f, Vector{ 1.f, 1.f, 1.f });*/

	}

	void BasicCameraController::PreUpdate(float tick)
	{
		
	}

	void BasicCameraController::LateUpdate(float tick)
	{
		Vector3 rot = transform->GetWorldRotation();

		float delta = tick;

		camera->UpdateViewMatrix();

		if (InputManager::GetInstance()->GetInputState('W', KeyState::STAY)) // 앞으로
		{
			// 레퍼런스 타입 연산만 가능해서 벡터를 변수로 뽑아줘야한다..!
			Vector3 look = transform->GetLook();
			transform->SetPosition(transform->GetWorldPosition() + (look * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('S', KeyState::STAY)) // s
		{
			Vector3 look = transform->GetLook();	
			transform->SetPosition(transform->GetWorldPosition() - (look * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('A', KeyState::STAY)) // a
		{
			Vector3 right = transform->GetRight();	
			transform->SetPosition(transform->GetWorldPosition() - (right * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('D', KeyState::STAY)) // d
		{
			Vector3 right = transform->GetRight();	
			transform->SetPosition(transform->GetWorldPosition() + (right * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('Q', KeyState::STAY)) // q
		{
			Vector3 up = transform->GetUp();
			transform->SetPosition(transform->GetWorldPosition() - (up * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('E', KeyState::STAY)) // e
		{
			Vector3 up = transform->GetUp();
			transform->SetPosition(transform->GetWorldPosition() + (up * moveSpeed * delta));
		}

		if (InputManager::GetInstance()->GetInputState('1', KeyState::DOWN))
		{
			camera->UpdateProjMatrix(ProjType::PERSPECTIVE);
		}

		if (InputManager::GetInstance()->GetInputState('2', KeyState::DOWN))
		{
			camera->UpdateProjMatrix(ProjType::ORTHOGRAPHIC);
		}

		if (InputManager::GetInstance()->GetInputState(VK_RBUTTON, KeyState::DOWN))
		{
			prevMousePos = InputManager::GetInstance()->GetMousePos();
		}	
		else if (InputManager::GetInstance()->GetInputState(VK_RBUTTON, KeyState::STAY))
		{
			float dx = XMConvertToRadians(0.25f * static_cast<float>(InputManager::GetInstance()->GetMousePos().x - prevMousePos.x));
			float dy = XMConvertToRadians(0.25f * static_cast<float>(InputManager::GetInstance()->GetMousePos().y - prevMousePos.y));

			float dxDegree = XMConvertToDegrees(dy);
			float dyDegree = XMConvertToDegrees(dx);

			transform->Rotate(Vector3(dxDegree, dyDegree, 0));

			prevMousePos = InputManager::GetInstance()->GetMousePos();
		}
	}
}
