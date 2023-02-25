#include "pch.h"
#include "TutorialScene.h"
#include "NextSceneTrigger.h"
#include "PlayerRellyPointTrigger.h"

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
#include "OnHitCollider.h"

//UI
#include "InGameUIController.h"
#include "SettingUIController.h"

namespace ClientSpace
{
	TutorialScene::TutorialScene(tstring sceneName)
		: SceneBase(sceneName)
	{
		playerEntryPos = Vector3::Zero;
		playerEntryRot = Vector3::Zero;
	}

	TutorialScene::~TutorialScene()
	{
	}

	void TutorialScene::BuildScene()
	{
		// 각종 오브젝트들 생성
		SceneManager::GetInstance()->LoadSceneData("tutorialScene");

		// NavMesh 생성 
		NavigationManager::GetInstance()->MakeNavMesh("TutorialNavMesh");

		SetScene();
		auto rellyPoint = SceneManager::GetInstance()->FindObjectName(L"RellyPoint");
		rellyPoint->AddComponent<PlayerRellyPointTrigger>();

		// DirectionalLight Test
		/*std::shared_ptr<GameObject> directionalLight = CreateEmpty();
		directionalLight->AddComponent<Light>();
		directionalLight->GetComponent<Light>()->ChangeType(LightType::Directional);
		directionalLight->GetComponent<Light>()->SetColor(SimpleMath::Vector3{ Colors::White });
		directionalLight->GetComponent<Light>()->SetDirection(SimpleMath::Vector3{ 0.5f, -0.5f, 0.5f });
		directionalLight->GetComponent<Light>()->SetPower(1.0f);*/

		std::shared_ptr<GameObject> player = CreateEmpty();
		player->SetName(L"Player");
		//player->AddComponent<NavMeshAgent>();
		player->GetComponent<Transform>()->SetPosition(playerEntryPos);
		player->GetComponent<Transform>()->SetRotation(playerEntryRot);
		Vector3 target = rellyPoint->GetComponent<Transform>()->GetWorldPosition();
		player->AddComponent<PlayerController>()->SetTarget(target);
		player->AddComponent<PlayerAttackController>();

		/*std::shared_ptr<GameObject> red = CreateEmpty();
		red->AddComponent<MeshRenderer>()->Init("red.model");
		red->GetComponent<Transform>()->SetPosition({ 5.f, 5.f, -15.f });
		red->AddComponent<NavMeshAgent>();
		red->AddComponent<Rigidbody>();
		red->AddComponent<CapsuleCollider>();
		red->GetComponent<CapsuleCollider>()->SetCenter({ 0.f, 1.0f, 0.f });
		red->GetComponent<CapsuleCollider>()->InitRenderObj();
		red->GetComponent<Rigidbody>()->InitRigidBody();
		red->GetComponent<Rigidbody>()->SetPhysAxisLock(false, false, false, true, true, true);
		red->AddComponent<MoveController>();
		red->GetComponent<MoveController>()->SetTarget(player->GetComponent<Transform>());

		std::shared_ptr<GameObject> axis = CreateEmpty();
		axis->AddComponent<PosColorRenderer>()->Init("Axis");*/

		// Fallow Camera
		auto cameraObj = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->GetGameObject();
		cameraObj->AddComponent<CameraController>();
		CreateUI();
	}
	std::shared_ptr<GameObject> TutorialScene::CreateUI()
	{
		std::shared_ptr<GameObject> ui = CreateEmpty();
		ui->AddComponent<InGameUIController>();
		return ui;
	}

	void TutorialScene::SetScene()
	{
		auto scene = SceneManager::GetInstance();

		auto nextPortal = scene->FindObjectName(L"NextPortal");
		auto playerSpawn = scene->FindObjectName(L"PlayerSpawn");

		playerEntryPos = playerSpawn->GetComponent<Transform>()->GetWorldPosition();
		playerEntryRot = playerSpawn->GetComponent<Transform>()->GetWorldRotation();

		nextPortal->AddComponent<NextSceneTrigger>()->SetNextScene(L"Room1Scene");
	}

}