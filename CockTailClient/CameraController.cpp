#include "pch.h"
#include "CameraController.h"
#include "SceneManager.h"


ClientSpace::CameraController::CameraController(std::weak_ptr<GameObject> _object)
	: ComponentBase(_object)
	, offset(0.f, 50.f, -50.f)
{

}

ClientSpace::CameraController::~CameraController()
{

}

void ClientSpace::CameraController::Awake()
{
	player = SceneManager::GetInstance()->FindObjectName(L"Player");
	playerTransform = player->GetComponent<Transform>();
	transform = GetGameObject()->GetComponent<Transform>();
}

void ClientSpace::CameraController::Start()
{

}

void ClientSpace::CameraController::PreUpdate(float tick)
{

}

void ClientSpace::CameraController::Update(float tick)
{
	
}

void ClientSpace::CameraController::LateUpdate(float tick)
{
	Vector3 pos = playerTransform->GetWorldPosition();

	Vector3 desiredPosition = pos + offset;

	Vector3 smoothedPosition = Vector3::Lerp(transform->GetWorldPosition(), desiredPosition, smoothSpeed);

	transform->SetPosition(smoothedPosition);
}
