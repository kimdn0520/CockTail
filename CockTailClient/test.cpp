#include "pch.h"
#include "test.h"
#include "InputManager.h"
#include "Component/CapsuleCollider.h"

ClientSpace::test::test(std::weak_ptr<GameObject> obj)
	: ComponentBase(obj)
	, moveSpeed(0.1)
	, gravity(Vector3(0,0,0))
	, direction(Vector3(0,0,0))
{
	input = InputManager::GetInstance();
	transform = GetGameObject()->GetComponent<Transform>();
	transform->SetRotation({ 0,0,0,0 });

	rigid = GetGameObject()->AddComponent<Rigidbody>();
	collider = GetGameObject()->AddComponent<CapsuleCollider>();
	collider->SetCenter({ 0.f,1.f,0.f });
	collider->InitRenderObj();

	rigid->InitRigidBody();
	rigid->SetPhysAxisLock(false, false, false, true, true, true);
}

ClientSpace::test::~test()
{
}

void ClientSpace::test::Awake()
{
	rigid->SetKinematic(true);
}

void ClientSpace::test::Start()
{

}

void ClientSpace::test::Update(float tick)
{
	InputKey();
}

void ClientSpace::test::LateUpdate(float tick)
{
	UpdateMove(tick);
}

void ClientSpace::test::DebugIMGUIRender(float tick)
{

}

void ClientSpace::test::InputKey()
{
	direction.x = 0.f;
	direction.z = 0.f;

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
}

void ClientSpace::test::UpdateMove(float tick)
{
	if (direction.x == 0 && direction.z == 0)
		return;

	Move();
}

void ClientSpace::test::Move()
{
	Vector3 velocity = direction * moveSpeed;

	Vector3 down = { 0.f, -1.f, 0.f };

	gravity = Vector3::Down * abs(rigid->GetLinearVelocity().y);

	Vector3 pos = transform->GetWorldPosition() + velocity;
	MoolyEngine::Vector3 targetPos = MoolyEngine::Vector3({ pos.x , pos.y, pos.z });
	rigid->MoveKinematic(targetPos);
}
