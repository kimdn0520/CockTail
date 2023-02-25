#include "pch.h"
#include "PlayerAttackController.h"
#include "Component/BoxCollider.h"
#include "Scripts/PlayerController.h"
#include "EnemyController.h"

ClientSpace::PlayerAttackController::PlayerAttackController(std::weak_ptr<GameObject> _object)
	: Trigger(_object)
	, comboCounter(0)
	, bAttack(false)
	, timer(0)
{
	ray = std::vector{ SimpleMath::Vector3::Zero, SimpleMath::Vector3::Zero };
	auto currentScene = SceneManager::GetInstance()->GetCurrentScene();
	std::shared_ptr<GameObject> box1 = currentScene->CreateEmpty();
	std::shared_ptr<GameObject> box2 = currentScene->CreateEmpty();
	std::shared_ptr<GameObject> box3 = currentScene->CreateEmpty();

	AddColliders(box1, box2, box3);
}

ClientSpace::PlayerAttackController::~PlayerAttackController()
{

}

void ClientSpace::PlayerAttackController::Awake()
{
	//attack_1->OnDisable();
	//attack_2->OnDisable();
	//attack_3->OnDisable();
	player = GetGameObject()->GetComponent<PlayerController>();
	transform = GetGameObject()->GetComponent<Transform>();
	rigid = GetGameObject()->GetComponent<Rigidbody>();
	input = InputManager::GetInstance();
	collider = GetGameObject()->GetComponent<Collider>();
	camera = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();
}

void ClientSpace::PlayerAttackController::Start()
{
	attack_1.collider->AddTriggerEvent(this);
	attack_2.collider->AddTriggerEvent(this);
	attack_3.collider->AddTriggerEvent(this);

	attack_1.object->SetGameObjectEnable(false);
	attack_2.object->SetGameObjectEnable(false);
	attack_3.object->SetGameObjectEnable(false);

	PhysicsManager::GetInstance()->DisableObject(attack_1.collider.get());
	PhysicsManager::GetInstance()->DisableObject(attack_2.collider.get());
	PhysicsManager::GetInstance()->DisableObject(attack_3.collider.get());
}

void ClientSpace::PlayerAttackController::PreUpdate(float tick)
{
	if (InputManager::GetInstance()->GetInputState(VK_LBUTTON, KeyState::DOWN))
	{
		comboCounter++;

		if (comboCounter == 1)
		{
			Vector2 mousePos = InputManager::GetInstance()->GetMousePos();
			ray = camera->ScreenToRayOrigin(mousePos);
			MoolyEngine::Vector3 rayOrigin = { ray[0].x, ray[0].y, ray[0].z };
			MoolyEngine::Vector3 rayDir = { ray[1].x, ray[1].y, ray[1].z };

			const std::vector<std::string> layer = { "Default", "Player", "Enemy", "Wall", "Trigger" };
			auto ret = PhysicsManager::GetInstance()->Raycast(rayOrigin, rayDir, 100000.f, layer, MoolyEngine::PhysicsType::STATIC);
			Vector3 target{ ret.hitPos.vec[0], transform->GetWorldPosition().y ,ret.hitPos.vec[2] };
			Vector3 look = target - transform->GetWorldPosition();

			transform->SetLook(look);
		}
	}
}

void ClientSpace::PlayerAttackController::Update(float tick)
{
	AttackUpdate(tick);
}

void ClientSpace::PlayerAttackController::LateUpdate(float tick)
{

}

void ClientSpace::PlayerAttackController::DebugIMGUIRender(float tick)
{
	/*auto imgui = GraphicsEngineSpace::ImGUIManager::GetInstance();

	if (imgui->Begin("Ray Direction"))
	{
		imgui->Text("Ray Dir - x : " + std::to_string(ray[1].x) + " y : " + std::to_string(ray[1].y) + " z : " + std::to_string(ray[1].z));
	}
	imgui->End();*/
}

void ClientSpace::PlayerAttackController::OnTriggerEnter(std::string name)
{
	std::shared_ptr<GameObject> collider = SceneManager::GetInstance()->FindObject(name);
	if (collider != nullptr)
	{
		if (collider->GetComponent<Collider>()->GetLayer() == "Enemy")
		{
			auto attack = FindEnableAttack();

			float _time = FindEnableAttack().delay;
			std::shared_ptr<EnemyController> enemy = collider->GetComponent<EnemyController>();
			Vector3 pos = transform->GetWorldPosition();
			enemy->SetHP(attack.dmg);
			enemy->SetPower(attack.power);
			enemy->SetDirection(pos);
			enemy->SetTimer(_time);
			enemy->SetOn();
		}
	}
}

void ClientSpace::PlayerAttackController::OnTriggerExit(std::string name)
{

}

void ClientSpace::PlayerAttackController::SetTriggerEvent(bool value)
{

}

void ClientSpace::PlayerAttackController::AddColliders(std::shared_ptr<GameObject> _box1, std::shared_ptr<GameObject> _box2, std::shared_ptr<GameObject> _box3)
{
	attack_1.object = _box1;
	attack_2.object = _box2;
	attack_3.object = _box3;

	SetBox(attack_1);
	ReSizeBox(attack_1, { 1.0f, 1.0f, 1.5f });
	RePositionBox(attack_1, { 0.0f, 1.0f, 1.0f });
	//ReCenter(attack_1, { 0.5f, 1.0f, 1.0f });
	attack_1.dmg = 1;
	attack_1.power = 1.0f;
	attack_1.delay = 1.0f;
	attack_1.num = attack_1.object->GetObjectID();
	attack_1.collider = attack_1.object->GetComponent<Collider>();

	SetBox(attack_2);
	ReSizeBox(attack_2, { 1.0f, 1.0f, 1.5f });
	RePositionBox(attack_2, { 0.0f, 1.0f, 1.0f });
	//ReCenter(attack_2, { -0.5f, 1.0f, 1.0f });
	attack_2.dmg = 2;
	attack_2.power = 2.0f;
	attack_2.delay = 1.0f;
	attack_2.num = attack_2.object->GetObjectID();
	attack_2.collider = attack_2.object->GetComponent<Collider>();


	SetBox(attack_3);
	ReSizeBox(attack_3, { 1.5f, 1.0f, 1.5f });
	RePositionBox(attack_3, { 0.0f, 1.0f, 1.0f });
	//ReCenter(attack_2, { 0.0f, 1.0f, 1.0f });
	attack_3.dmg = 5;
	attack_3.power = 5.0f;
	attack_3.delay = 1.0f;
	attack_3.num = attack_3.object->GetObjectID();
	attack_3.collider = attack_3.object->GetComponent<Collider>();


}

void ClientSpace::PlayerAttackController::SetBox(AttackBoxObject _object)
{
	_object.object->AddComponent<BoxCollider>();
	_object.object->GetComponent<BoxCollider>()->InitRenderObj();
	_object.object->GetComponent<BoxCollider>()->SetTrigger(true);
	this->GetGameObject()->SetChild(_object.object);
}

void ClientSpace::PlayerAttackController::ReSizeBox(AttackBoxObject _object, Vector3 _size)
{
	_object.size = _size;
	_object.object->GetComponent<BoxCollider>()->SetSize(_size);
}

void ClientSpace::PlayerAttackController::RePositionBox(AttackBoxObject _object, Vector3 _pos)
{
	_object.position = _pos;
	_object.object->GetComponent<Transform>()->SetPosition(_pos, GameEngineSpace::Space::LOCAL);
}

void ClientSpace::PlayerAttackController::ReCenter(AttackBoxObject _object, Vector3 _center)
{
	_object.center = _center;
	_object.object->GetComponent<BoxCollider>()->SetCenter(_center);
}

void ClientSpace::PlayerAttackController::ReDelay(AttackBoxObject _object, float _delay)
{
	_object.delay = _delay;
}

std::shared_ptr<GameEngineSpace::GameObject> ClientSpace::PlayerAttackController::GetAttackBox(int num)
{
	if (num == 1)
		return attack_1.object;

	else if (num == 2)
		return attack_2.object;

	else if (num == 3)
		return attack_3.object;

	else
		return NULL;

	//return attackVec[num];
}

void ClientSpace::PlayerAttackController::SetAttakDelayTime(int num, float value)
{
	if (num == 1)
		ReDelay(attack_1, value);

	else if (num == 2)
		ReDelay(attack_2, value);

	else if (num == 3)
		ReDelay(attack_3, value);

	else
		return;
}

void ClientSpace::PlayerAttackController::AttackUpdate(float tick)
{
	if (comboCounter == 1)
	{
		if (bAttack == false)
		{
			bAttack = true;
			attack_1.object->SetGameObjectEnable(true);
			PhysicsManager::GetInstance()->EnableObject(attack_1.collider.get());
			timer = 0.0f;
		}
	}

	if (attack_1.object->GetIsEnable())
	{
		if (attack_1.delay > timer)
			timer += tick;

		else
		{
			attack_1.object->SetGameObjectEnable(false);
			PhysicsManager::GetInstance()->DisableObject(attack_1.collider.get());
			timer = 0.0f;

			if (comboCounter < 2)
			{
				bAttack = false;
				comboCounter = 0;
			}

			else if (comboCounter >= 2)
			{
				bAttack = true;
				attack_2.object->SetGameObjectEnable(true);
				PhysicsManager::GetInstance()->EnableObject(attack_2.collider.get());
			}
		}
	}

	if (attack_2.object->GetIsEnable())
	{
		if (attack_2.delay > timer)
			timer += tick;

		else
		{
			attack_2.object->SetGameObjectEnable(false);
			PhysicsManager::GetInstance()->DisableObject(attack_2.collider.get());
			timer = 0.0f;

			if (comboCounter < 3)
			{
				bAttack = false;
				comboCounter = 0;
			}

			else if (comboCounter >= 3)
			{
				bAttack = true;
				attack_3.object->SetGameObjectEnable(true);
				PhysicsManager::GetInstance()->EnableObject(attack_3.collider.get());
			}
		}
	}

	if (attack_3.object->GetIsEnable())
	{
		if (attack_3.delay > timer)
			timer += tick;

		else
		{
			attack_3.object->SetGameObjectEnable(false);
			PhysicsManager::GetInstance()->DisableObject(attack_3.collider.get());
			timer = 0.0f;
			comboCounter = 0;
			bAttack = false;
		}
	}
}

ClientSpace::AttackBoxObject ClientSpace::PlayerAttackController::FindEnableAttack()
{
	if (attack_1.object->GetIsEnable())
		return attack_1;

	else if (attack_2.object->GetIsEnable())
		return attack_2;

	else if (attack_3.object->GetIsEnable())
		return attack_3;

	else
		return AttackBoxObject();
}
