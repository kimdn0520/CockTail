#include "pch.h"
#include "PlayerControllerMK2.h"

ClientSpace::PlayerControllerMK2::PlayerControllerMK2(std::weak_ptr<GameObject> _object)
	: ComponentBase(_object)
	, status(PlayerStatusMK2::IDLE)
	, moveSpeed(10.0f)
	, rotateSpeed(30.0f)
	, dashTime(2.0f)
	, dashSpeed(2.0f)
	, isGrounded(false)
	, isDash(false)
	, isStop(false)
	, bStop(true)
	, rayDistance(0.3f)
{
	transform = GetGameObject()->GetComponent<Transform>();
	meshRenderer = GetGameObject()->GetComponent<MeshRenderer>();
	rigid = GetGameObject()->GetComponent<Rigidbody>();
	input = InputManager::GetInstance();
	collider = GetGameObject()->GetComponent<Collider>();
	direction = Vector3::Zero;
	dashDirection = Vector3::Zero;
	prevDirection = Vector3::Zero;
}

ClientSpace::PlayerControllerMK2::~PlayerControllerMK2()
{

}

void ClientSpace::PlayerControllerMK2::Awake()
{
	rigid->SetMaxLinearVelocity(40.0f);
}

void ClientSpace::PlayerControllerMK2::Start()
{
	meshRenderer->PlayAnim("tailani_tailtest", true);
}

void ClientSpace::PlayerControllerMK2::PreUpdate(float tick)
{
	RaycastUpdate();

	if (status != PlayerStatusMK2::IDLE && status != PlayerStatusMK2::MOVE)
		return;

	InputUpdate();
	RotateUpdate(tick);
}

void ClientSpace::PlayerControllerMK2::Update(float tick)
{
	if (InputManager::GetInstance()->GetInputState(VK_SPACE, KeyState::DOWN))
	{
 		rigid->SetLinearVelocity({ 4.0f, 0.0f, 0.0f });
	}
	MoveUpdate();
	StopCharacter(tick);
}

void ClientSpace::PlayerControllerMK2::LateUpdate(float tick)
{

}

void ClientSpace::PlayerControllerMK2::StopCharacter(float delta)
{
	if (isStop == true && bStop == false)
	{
		rigid->SetLinearVelocity({ 0,0,0 });
		status = PlayerStatusMK2::IDLE;
		isStop = false;
		bStop = true;
	}

	else if (isStop == false && bStop == true)
	{
		rigid->SetLinearVelocity({ 0,-9.8f * delta,0 });
	}
}

void ClientSpace::PlayerControllerMK2::RaycastUpdate()
{
	const std::vector<std::string> filterLayer = { "Player" };
	Vector3 worldPos = transform->GetWorldPosition();
	auto ret = PhysicsManager::GetInstance()->Raycast({ worldPos.x, worldPos.y, worldPos.z }, { 0, -1.0f, 0 }, rayDistance, filterLayer, MoolyEngine::PhysicsType::INVALID);

	isGrounded = !ret.objectName.empty() && ret.isHit;

	if (isGrounded)
		bStop = false;
}

void ClientSpace::PlayerControllerMK2::InputUpdate()
{
	direction = Vector3::Zero;

	if (isGrounded == false)
		return;

	if (InputManager::GetInstance()->GetInputState(VK_UP, KeyState::STAY))
	{
		direction.z += 1.0f;
	}

	if (InputManager::GetInstance()->GetInputState(VK_DOWN, KeyState::STAY))
	{
		direction.z -= 1.0f;
	}

	if (InputManager::GetInstance()->GetInputState(VK_LEFT, KeyState::STAY))
	{
		direction.x -= 1.0f;
	}

	if (InputManager::GetInstance()->GetInputState(VK_RIGHT, KeyState::STAY))
	{
		direction.x += 1.0f;
	}

	direction.Normalize();

	if (direction != Vector3::Zero)
	{
		status = PlayerStatusMK2::MOVE;
	}
}

void ClientSpace::PlayerControllerMK2::MoveUpdate()
{
	if (isGrounded == true)
		rigid->SetLinearVelocity(direction * moveSpeed);

	else
		isStop = true;
}

void ClientSpace::PlayerControllerMK2::RotateUpdate(float deltaTime)
{
	if (isGrounded == false)
		return;

	if (direction == Vector3::Zero)
	{
		status = PlayerStatusMK2::IDLE;
		return;
	}

	if (status != PlayerStatusMK2::IDLE && status != PlayerStatusMK2::MOVE)
		return;

	rigid->SetMaxLinearVelocity(moveSpeed);
	Vector3 _result;
	_result = Vector3::Lerp(transform->GetLook(), direction, deltaTime * rotateSpeed);

	transform->SetLook(_result);

	status = PlayerStatusMK2::MOVE;
}
