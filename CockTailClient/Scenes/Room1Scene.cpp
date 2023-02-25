#include "pch.h"
#include "Room1Scene.h"
#include "PlayerRellyPointTrigger.h"
#include "EnemySpawner.h"

#include "Component/Camera.h"
#include "Component/BasicCameraController.h"
#include "Component/BoxCollider.h"
#include "Component/SphereCollider.h"
#include "Component/CapsuleCollider.h"
#include "Component/PosColorRenderer.h"
#include "Component/MeshRenderer.h"
#include "Component/Light.h"
#include "Component/SpriteEffect.h"
#include "CameraController.h"

#include "Scripts/MoveController.h"
#include "Scripts/PlayerController.h"
#include "PlayerAttackController.h"

#include "EnemyController.h"
#include "EnemyNavAgent.h"
#include "EnemyHitTrigger.h"

#include "test.h"

//UI
#include "InGameUIController.h"
#include "SettingUIController.h"

namespace ClientSpace
{
	Room1Scene::Room1Scene(tstring sceneName)
		: SceneBase(sceneName)
	{
	}

	Room1Scene::~Room1Scene()
	{
	}

	void Room1Scene::BuildScene()
	{
		// 阿辆 坷宏璃飘甸 积己
		SceneManager::GetInstance()->LoadSceneData("Room1Scene");

		// NavMesh 积己 
		NavigationManager::GetInstance()->MakeNavMesh("Room1NavMesh");

		SetScene();
		auto rellyPoint = SceneManager::GetInstance()->FindObjectName(L"RellyPoint");
		rellyPoint->AddComponent<PlayerRellyPointTrigger>();

		// player 积己
		std::shared_ptr<GameObject> player = CreateEmpty();
		player->AddComponent<PlayerController>();
		player->GetComponent<Transform>()->SetPosition(playerEntryPos);
		player->GetComponent<Transform>()->SetRotation(playerEntryRot);	
		Vector3 target = rellyPoint->GetComponent<Transform>()->GetWorldPosition();
		player->AddComponent<PlayerController>()->SetTarget(target);
		player->AddComponent<PlayerAttackController>();

		// ObjectPool 积己
		objectPool = make_shared<ObjectPool>();

		objectPool->RegisterGameObject("dog", 5, [&]()
		{
			std::shared_ptr<GameObject> dog = CreateEmpty();
			dog->AddComponent<MeshRenderer>()->Init("dog.model");
			dog->AddComponent<EnemyController>();
			dog->AddComponent<EnemyController>()->SetObjectPool(objectPool);
			dog->SetGameObjectEnable(false);
			return dog;
		});

		enemySpawn->GetComponent<EnemySpawner>()->TestObjectPool(objectPool);

		// DirectionalLight Test
		std::shared_ptr<GameObject> directionalLight = CreateEmpty();
		directionalLight->AddComponent<Light>();
		directionalLight->GetComponent<Light>()->ChangeType(LightType::Directional);
		directionalLight->GetComponent<Light>()->SetColor(SimpleMath::Vector3{ Colors::White });
		directionalLight->GetComponent<Light>()->SetDirection(SimpleMath::Vector3{ 0.5f, -0.5f, 0.5f });
		directionalLight->GetComponent<Light>()->SetPower(1.0f);


	/*	std::shared_ptr<GameObject> dummy = CreateEmpty();
		dummy->AddComponent<MeshRenderer>()->Init("dog.model");
		dummy->GetComponent<Transform>()->SetPosition({ 20.f, 5.f, -45.f });
		dummy->AddComponent<EnemyController>();*/

	/*	std::shared_ptr<GameObject> testmob = CreateEmpty();
		testmob->AddComponent<MeshRenderer>()->Init("red.model");
		testmob->GetComponent<Transform>()->SetPosition({ 20.f, 5.f, -50.f });
		testmob->AddComponent<EnemyController>();*/

		//std::shared_ptr<GameObject> red = CreateEmpty();
		//red->AddComponent<MeshRenderer>()->Init("red.model");
		//red->GetComponent<Transform>()->SetPosition({ 30.f, 5.f, -35.f });
		//red->AddComponent<NavMeshAgent>();
		//red->AddComponent<Rigidbody>();
		//red->AddComponent<CapsuleCollider>();
		//red->GetComponent<CapsuleCollider>()->SetCenter({ 0.f, 1.0f, 0.f });
		//red->GetComponent<CapsuleCollider>()->InitRenderObj();
		//red->GetComponent<Rigidbody>()->InitRigidBody();
		//red->GetComponent<Rigidbody>()->SetPhysAxisLock(false, false, false, true, true, true);
		//red->AddComponent<MoveController>();
		//red->GetComponent<MoveController>()->SetTarget(player->GetComponent<Transform>());

		/*std::shared_ptr<GameObject> axis = CreateEmpty();
		axis->AddComponent<PosColorRenderer>()->Init("Axis");*/
		SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->GetGameObject()->AddComponent<CameraController>();

		CreateUI();
	}

	std::shared_ptr<GameObject> Room1Scene::CreateUI()
	{
		std::shared_ptr<GameObject> ui = CreateEmpty();
		ui->AddComponent<InGameUIController>();
		return ui;
	}

	void Room1Scene::SetScene()
	{
		auto scene = SceneManager::GetInstance();

		auto entryPoint = scene->FindObjectName(L"EntryPoint");
		auto nextPortal = scene->FindObjectName(L"Elavtor");
		enemySpawn = scene->FindObjectName(L"EnemySpawn");

		playerEntryPos = entryPoint->GetComponent<Transform>()->GetWorldPosition();
		playerEntryRot = entryPoint->GetComponent<Transform>()->GetWorldRotation();

		enemySpawn->AddComponent<EnemySpawner>();
	}
}