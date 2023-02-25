#include "pch.h"
#include "LobbyScene.h"
#include "NextSceneTrigger.h"

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

#include "Scripts/OnHitTrigger.h"
#include "DamageTest.h"
#include "PlayerControllerMK2.h"

#include "Scripts/MoveController.h"
#include "Scripts/PlayerController.h"
#include "PlayerAttackController.h"

#include "EnemyController.h";
//UI
#include "InGameUIController.h"
#include "SettingUIController.h"

namespace ClientSpace
{
	LobbyScene::LobbyScene(tstring sceneName)
		: SceneBase(sceneName)
	{
		playerEntryPos = Vector3::Zero;
		playerEntryRot = Vector3::Zero;
	}

	LobbyScene::~LobbyScene()
	{
	}

	void LobbyScene::BuildScene()
	{
		// 각종 오브젝트들 생성
		SceneManager::GetInstance()->LoadSceneData("lobbyScene");

		SetScene();

		// NavMesh 생성 
		//NavigationManager::GetInstance()->MakeNavMesh("TestNavMesh");

		// DirectionalLight Test
		/*std::shared_ptr<GameObject> directionalLight = CreateEmpty();
		directionalLight->AddComponent<Light>();
		directionalLight->GetComponent<Light>()->ChangeType(LightType::Directional);
		directionalLight->GetComponent<Light>()->SetColor(SimpleMath::Vector3{ Colors::White });
		directionalLight->GetComponent<Light>()->SetDirection(SimpleMath::Vector3{ 0.5f, -0.5f, 0.5f });
		directionalLight->GetComponent<Light>()->SetPower(1.0f);*/

		std::shared_ptr<GameObject> player = CreateEmpty();
		player->AddComponent<PlayerController>()->SetBool(true);
		player->GetComponent<Transform>()->SetPosition(playerEntryPos);
		player->GetComponent<Transform>()->SetRotation(playerEntryRot);

		// Follow Camera
		SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->GetGameObject()->AddComponent<CameraController>();

		//std::shared_ptr<GameObject> dummy = CreateEmpty();
		//dummy->AddComponent<MeshRenderer>()->Init("dog.model");
		//dummy->GetComponent<Transform>()->SetPosition({ 20.f, 5.f, -45.f });
		//dummy->AddComponent<EnemyController>();


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
		CreateUI();
	}
	std::shared_ptr<GameObject> LobbyScene::CreateUI()
	{
		std::shared_ptr<GameObject> ui = CreateEmpty();
		ui->AddComponent<InGameUIController>();
		return ui;
	}

	void LobbyScene::SetScene()
	{
		auto scene = SceneManager::GetInstance();

		auto entryPoint = scene->FindObjectName(L"EntryPoint");
		auto exitPortal = scene->FindObjectName(L"ExitPortal");
		auto interactNPC = scene->FindObjectName(L"InteractNPC");
		auto makeCockTail = scene->FindObjectName(L"MakeCockTail");
		auto swapCharacter = scene->FindObjectName(L"SwapCharacter");

		playerEntryPos = entryPoint->GetComponent<Transform>()->GetWorldPosition();
		playerEntryRot = entryPoint->GetComponent<Transform>()->GetWorldRotation();
		
		exitPortal->AddComponent<NextSceneTrigger>()->SetNextScene(L"TutorialScene");
		makeCockTail->AddComponent<NextSceneTrigger>()->SetNextScene(L"BarOperationScene");
	}

}