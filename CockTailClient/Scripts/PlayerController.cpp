#include "pch.h"
#include "PlayerController.h"
#include "Component/BoxCollider.h"
#include "Component/CapsuleCollider.h"
#include "EnemyHitTrigger.h"
#include "PlayerAttackController.h"

#include "InputManager.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace ClientSpace
{

	PlayerController::PlayerController(std::weak_ptr<GameObject> _object)
		: ComponentBase(_object, ComponentType::MONOBEHAVIOR)
		, bMoveToTarget(false)
		, moveSpeed(10.0f)
		, rotateSpeed(30.0f)
		, dashTime(0.3f)
		, dashSpeed(30.0f)
		, isGrounded(false)
		, isDash(false)
		, isStop(false)
		, bStop(false)
		, rayDistance(0.3f)
		, maxSlopeAngle(60.f)
		, slopeAngle(0.f)
		, gravity(Vector3(0.f, 0.f, 0.f))
	{
		input = InputManager::GetInstance();

		transform = GetGameObject()->GetComponent<Transform>();
		//transform->SetPosition({ 10.f, 10.f, -40.f });
		transform->SetRotation({ 0.0f, 0.0f, 0.0f });

		meshRenderer = GetGameObject()->AddComponent<MeshRenderer>();
		meshRenderer->Init("tailani.model");

		rigid = GetGameObject()->AddComponent<Rigidbody>();
		collider = GetGameObject()->AddComponent<CapsuleCollider>();
		collider->SetLayer("Player");
		collider->SetCenter({ 0.f,1.f,0.f });
		collider->InitRenderObj();

		rigid->InitRigidBody();
		rigid->SetPhysAxisLock(false, false, false, true, true, true);

		direction = Vector3::Zero;
		dashDirection = Vector3::Zero;
		prevDirection = Vector3::Zero;

		GetGameObject()->AddComponent<EnemyHitTrigger>();
		GetGameObject()->AddComponent<PlayerAttackController>();

		GetGameObject()->SetName(L"Player");

		/*blocker = SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		blocker->AddComponent<Rigidbody>();
		blocker->AddComponent<CapsuleCollider>()->InitRenderObj();

		float radius = GetGameObject()->GetComponent<CapsuleCollider>()->GetRadius();
		float height = GetGameObject()->GetComponent<CapsuleCollider>()->GetHeight();
		std::shared_ptr<CapsuleCollider> blockCollider = blocker->GetComponent<CapsuleCollider>();
		blockCollider->SetRadius(radius);
		blockCollider->SetHeight(height);

		blocker->GetComponent<Rigidbody>()->InitRigidBody();
		blockCollider->SetCenter(collider->GetCenter());
		blockCollider->SetLayer("CharacterBlocker");


		blocker->GetComponent<Transform>()->SetPosition(Vector3::Zero, GameEngineSpace::Space::LOCAL);

		GetGameObject()->SetChild(blocker);*/
	}

	PlayerController::~PlayerController()
	{

	}

	void PlayerController::Awake()
	{
		rigid->SetMaxLinearVelocity(40.0f);
		/*	rigid->SetKinematic(true);*/
			//blocker->GetComponent<Rigidbody>()->SetKinematic(true);

		status = PlayerStatus::IDLE;

		//////////////////////////////////////////////////////////////////////////////
		//							AnimationState
		//////////////////////////////////////////////////////////////////////////////
		renderObj = meshRenderer->GetRenderObj();

		// State 이름, 클립 이름, 재생 속도, Loop 여부
		renderObj->AddAnimationState("Idle", "tailani_idle.hammer", 1, true);
		renderObj->AddAnimationState("Run", "tailani_run.hammer", 1, true);

		renderObj->AddFloat("Move", 0.f);
		//renderObj->AddTrigger("RunTrigger");
		//renderObj->AddTrigger("IdleTrigger");
		
		// Transition Duration : 블렌딩할때 쓰인다.현재애니메이션 클립과 다음 애니메이션 클립을 섞어(블렌딩) 부드럽게 이어주는 역할을 한다.
		//	                     0.2초로 설정하면 0.2초 동안 뛰는 애니메이션클립과 점프하는 애니메이션 클립을 섞어서 부드럽게 이어준다.
		// bool has Exit Time : 활성화 시 -> 종료 시점이 존재하게 되어 전이의 조건을 만족해도 다음 상태로 전이하지 않게 된다.
		//                      비활성화 시->전이 조건을 만족하는 즉시 전이가 시작된다.
		//			예) Run->Jump 에서 Has Exit Time 이 활성화 되어있고 exit time이 0.5(50 %) 인 경우 전이 조건을 만족해도
		//				Run 애니메이션 클립이 최소 절반은 재생되어야 전이가 이루어진다.
		// float Exit Time : (Has Exit Time 이 활성화시에 설정할 수 있다.) 종료 시점
		// Trasition을 추가할 State 이름(Add 하고 써야함), 연결할 state 이름, 연결된 state 이름, has Exit Time, ExitTime, Transition Duration  
		renderObj->SettingAnimationStateTransition("Idle", "Idle", "Run", false, 0.f, 0.2f);
		renderObj->SettingAnimationStateTransition("Run", "Run", "Idle", false, 0.f, 0.2f);

		// Transition의 조건을 추가하는 함수(조건을 만족해야 다음 애니메이션으로 넘어간다)
		// 오버로딩 함수 4개 이다.
		// int 에는 Greater(초과), Less(미만), Equals(같은), NotEqual(같지 않은) / float 에는 greater, less 2개만 있다!
		renderObj->AddCondition("Idle->Run", "Move", "Greater", 0.5f);
		renderObj->AddCondition("Run->Idle", "Move", "Less", 0.5f);
		//renderObj->AddCondition("Idle->Run", "RunTrigger", "None");
		//renderObj->AddCondition("Run->Idle", "IdleTrigger", "None");
		//renderObj->AddCondition("Idle->Run", "TestInt", "Equlas", 1);
		//renderObj->AddCondition("Idle->Run", "TestBool", "None", true);

		// Entry 애니메이션 State 설정
		renderObj->EntryPlayAnimationState("Idle");
	}

	void PlayerController::Start()
	{
		//meshRenderer->PlayAnim("tailani_tailtest", true);
		rigid->SetMass(0.001f);
	}

	void PlayerController::FixedUpdate(float tick)
	{
		switch (status)
		{
		case PlayerStatus::IDLE:
			UpdateIdle();
			break;
		case PlayerStatus::MOVE:
			UpdateMove(tick);
			break;
		case PlayerStatus::DASH:
			UpdateDash(tick);
			break;
		}
	}

	void PlayerController::PreUpdate(float tick)
	{
		/*switch (status)
		{
			case PlayerStatus::IDLE:
				UpdateIdle();
				break;
			case PlayerStatus::MOVE:
				UpdateMove(tick);
				break;
			case PlayerStatus::DASH:
				UpdateDash(tick);
				break;
		}*/
	}

	void PlayerController::Update(float tick)
	{
		if (bMoveToTarget == false)
		{
			MoveToTarget(moveTarget);
			return;
		}

		InputKey();
	}

	void PlayerController::LateUpdate(float tick)
	{
	}

	void PlayerController::DebugIMGUIRender(float tick)
	{
		Vector3 _result;
		if (direction == Vector3::Zero)
		{
			_result = Vector3::Zero;
		}

		else
		{
			Vector3 _direct = direction;
			_direct.Normalize();
			_result = Vector3::Lerp(transform->GetLook(), _direct, tick);
		}

		auto imgui = GraphicsEngineSpace::ImGUIManager::GetInstance();

		/*if (imgui->Begin("Result"))
		{
			imgui->Text("Result " + std::to_string(_result.x) + " " + std::to_string(_result.y) + " " + std::to_string(_result.z));
		}
		imgui->End();*/

		if (imgui->CollapsingHeader("Status"))
		{
			switch (status)
			{
				case PlayerStatus::IDLE:
					imgui->Text("Status : IDLE");
					break;

				case PlayerStatus::MOVE:
					imgui->Text("Status : MOVE");
					break;

				case PlayerStatus::DASH:
					imgui->Text("Status : DASH");
					break;

				case PlayerStatus::ATTACK:
					imgui->Text("Status : ATTACK");
					break;
			}
		}
		imgui->Spacing();
	}

	void PlayerController::InputKey()
	{
		direction.x = 0.f;
		direction.z = 0.f;
		
		if (InputManager::GetInstance()->GetInputState('W', KeyState::STAY))
		{
			direction.z += 1.0f;
		}

		if (InputManager::GetInstance()->GetInputState('S', KeyState::STAY))
		{
			direction.z -= 1.0f;
		}

		if (InputManager::GetInstance()->GetInputState('A', KeyState::STAY))
		{
			direction.x -= 1.0f;
		}

		if (InputManager::GetInstance()->GetInputState('D', KeyState::STAY))
		{
			direction.x += 1.0f;
		}

		direction.Normalize();

		if (InputManager::GetInstance()->GetInputState(VK_SPACE, KeyState::DOWN))
		{
			Vector3 _look = transform->GetLook();
			rigid->AddForce({ _look.x * 100, 0.0f, _look.z * 100 });
			status = PlayerStatus::DASH;
		}
	}

	void PlayerController::UpdateIdle()
	{
		// Status Check
		if (direction.x != 0.f || direction.z != 0.f)
			status = PlayerStatus::MOVE;

		//renderObj->SetTrigger("IdleTrigger");
		renderObj->SetFloat("Move", 0.f);

		if (IsGround() && IsSlope())
		{
			rigid->SetGravity(false);
		}
		else if (IsGround() && !IsSlope())
		{
			rigid->SetGravity(true);
		}
	}

	void PlayerController::UpdateMove(float tick)
	{
		// Status Check
		if (direction.x == 0.f && direction.z == 0.f)
			status = PlayerStatus::IDLE;

		renderObj->SetFloat("Move", 1.f);
		Turn(tick);

		//renderObj->SetTrigger("RunTrigger");

		if (isBorder() == false)
			Move();
	}

	void PlayerController::UpdateDash(float tick)
	{
		dashTime -= tick;

		Vector3 velocity = transform->GetLook();

		if (dashTime <= 0)
		{
			rigid->SetLinearVelocity({ 0.0f, 0.0f, 0.0f });

			dashTime = 0.3f;

			if (direction.x != 0.f || direction.z != 0.f)
				status = PlayerStatus::MOVE;
			else
				status = PlayerStatus::IDLE;
			return;
		}

		if (IsGround() && IsSlope())
		{
			// 경사면에 투영된 벡터를 구함
			velocity = ProjectOnPlane(direction, { hitSlope.normal.vec[0], hitSlope.normal.vec[1], hitSlope.normal.vec[2] });

			velocity.Normalize();

			// 경사면이라면 gravity는 0
			gravity = Vector3::Zero;
			// gravity 도 꺼준다.
			rigid->SetGravity(false);

			rigid->SetLinearVelocity(velocity * dashSpeed);
		}
		else if (IsGround() && !IsSlope())
		{
			Vector3 down = { 0.f, -1.f, 0.f };

			gravity = Vector3::Down * abs(rigid->GetLinearVelocity().y);

			rigid->SetGravity(true);

			rigid->SetLinearVelocity(velocity * dashSpeed + gravity);
		}
	}

	void PlayerController::Move()
	{
		Vector3 velocity = direction;

		if (IsGround() && IsSlope())
		{
			// 경사면에 투영된 벡터를 구함
			velocity = ProjectOnPlane(direction, { hitSlope.normal.vec[0], hitSlope.normal.vec[1], hitSlope.normal.vec[2] });

			velocity.Normalize();

			// 경사면이라면 gravity는 0
			gravity = Vector3::Zero;

			// gravity 도 꺼준다.
			rigid->SetGravity(false);

			rigid->SetLinearVelocity(velocity * moveSpeed + gravity);
			/*	Vector3 pos = transform->GetWorldPosition() + velocity;
				MoolyEngine::Vector3 targetPos = MoolyEngine::Vector3({ pos.x , pos.y, pos.z });
				rigid->MoveKinematic(targetPos);*/
		}

		else if (IsGround() && !IsSlope())
		{
			Vector3 down = { 0.f, -1.f, 0.f };

			gravity = Vector3::Down * abs(rigid->GetLinearVelocity().y);

			rigid->SetGravity(true);

			rigid->SetLinearVelocity(velocity * moveSpeed + gravity);
			/*	Vector3 pos = transform->GetWorldPosition() + velocity;
				MoolyEngine::Vector3 targetPos = MoolyEngine::Vector3({ pos.x , pos.y, pos.z });
				rigid->MoveKinematic(targetPos);*/
		}
	}

	void PlayerController::Turn(float tick)
	{
		Vector3 _result;
		_result = Vector3::Lerp(transform->GetLook(), direction, tick * rotateSpeed);

		transform->SetLook(_result);
	}

	bool PlayerController::IsSlope()
	{
		const std::vector<std::string> filterLayer = { "Default", "Player", "Enemy", "Wall", "Trigger" };
		Vector3 worldPos = transform->GetWorldPosition();

		// 캐릭터의 worldPos 에서 아래로 2.f 길이만큼의 ray 를 쏜다.
		hitSlope = PhysicsManager::GetInstance()->Raycast({ worldPos.x, worldPos.y, worldPos.z }, { 0, -1.0f, 0 }, 2.f, filterLayer, MoolyEngine::PhysicsType::INVALID);

		if (hitSlope.isHit)
		{
			SimpleMath::Vector3 normal = { hitSlope.normal.vec[0], hitSlope.normal.vec[1], hitSlope.normal.vec[2] };

			slopeAngle = XMVectorGetX(XMVector3AngleBetweenVectors(Vector3::Up, normal));

			slopeAngle = XMConvertToDegrees(slopeAngle);

			return slopeAngle != 0.f && slopeAngle < maxSlopeAngle;
		}

		return false;
	}

	bool PlayerController::isBorder()
	{
		const std::vector<std::string> filterLayer = { "Default", "Player", "Enemy", "Plane", "Trigger" };
		Vector3 worldPos = transform->GetWorldPosition();
		Vector3 forward = transform->GetLook();

		auto _result = PhysicsManager::GetInstance()->Raycast({ worldPos.x, worldPos.y + 1.0f, worldPos.z }, { forward.x, forward.y, forward.z }, 1.f, filterLayer, MoolyEngine::PhysicsType::INVALID);

		return _result.isHit;
	}

	bool PlayerController::IsGround()
	{
		Vector3 worldPos = transform->GetWorldPosition();
		Vector3 scale = { 1.0f, 0.6f, 1.0f };

		const std::vector<std::string> filterLayer = { "Default", "Player", "Enemy", "Wall", "Trigger" };

		return PhysicsManager::GetInstance()->CheckBox({ worldPos.x, worldPos.y, worldPos.z }, { scale.x, scale.y, scale.z }, filterLayer);
	}

	/*void PlayerController::DashInputUpdate()
	{
		if (InputManager::GetInstance()->GetInputState(VK_SPACE, KeyState::DOWN))
		{
			isDash = true;
			status = PlayerStatus::DASH;
			currentTimer = 0.0f;
			dashDirection = transform->GetLook();
			dashDirection *= moveSpeed * dashSpeed;
		}
	}*/

	void PlayerController::MoveToTarget(Vector3 target)
	{
		direction = target - transform->GetWorldPosition();
		direction.y = 0.0f;
		direction.Normalize();
		renderObj->SetFloat("Move", 1.f);
	}

	/*void PlayerController::DashUpdate(float deltaTime)
	{
		if (isDash == false)
			return;

		if (status == PlayerStatus::DASH)
		{
			if (currentTimer >= dashTime)
			{
				isDash = false;
				status = PlayerStatus::IDLE;
				rigid->SetMaxLinearVelocity(0);
				return;
			}

			currentTimer += deltaTime;
			rigid->SetMaxLinearVelocity(dashSpeed * moveSpeed);
			rigid->AddForce({ dashDirection.x , dashDirection.y, dashDirection.z });
		}
	}*/

	SimpleMath::Vector3 PlayerController::ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal)
	{
		float dot = vector.Dot(planeNormal);

		Vector3 scaledNormal = planeNormal * dot;

		return vector - scaledNormal;
	}
}