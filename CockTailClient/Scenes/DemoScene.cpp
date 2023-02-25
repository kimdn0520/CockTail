#include "pch.h"
#include "DemoScene.h"

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
#include "Scripts/MoveController.h"
#include "Scripts/PlayerController.h"
#include "PlayerControllerMK2.h"
#include "PlayerAttackController.h"
#include "DamageTest.h"

//UI
#include "InGameUIController.h"
#include "SettingUIController.h"


namespace ClientSpace
{
	DemoScene::DemoScene(tstring sceneName)
		: SceneBase(sceneName)
	{
	}

	DemoScene::~DemoScene()
	{
	}

	void DemoScene::BuildScene()
	{
		// 각종 오브젝트들 생성
		SceneManager::GetInstance()->LoadSceneData("Prototype");

		// NavMesh 생성 (여기서 하는게 맞나?!)
		NavigationManager::GetInstance()->MakeNavMesh("TestNavMesh");

		// DirectionalLight Test
		std::shared_ptr<GameObject> directionalLight = CreateEmpty();
		directionalLight->AddComponent<Light>();
		directionalLight->GetComponent<Light>()->ChangeType(LightType::Directional);
		directionalLight->GetComponent<Light>()->SetColor(SimpleMath::Vector3{ Colors::White });
		directionalLight->GetComponent<Light>()->SetDirection(SimpleMath::Vector3{ 0.5f, -0.5f, 0.5f });
		directionalLight->GetComponent<Light>()->SetPower(1.0f);

		std::shared_ptr<GameObject> player = CreateEmpty();
		player->SetName(L"Player");
		player->AddComponent<MeshRenderer>()->Init("tailani.model");
		player->GetComponent<Transform>()->SetPosition({ 0.f, 5.f, -10.f });
		player->GetComponent<Transform>()->SetRotation({ 0.0f, 0.0f, 0.0f });
		player->AddComponent<Rigidbody>();
		player->AddComponent<CapsuleCollider>();
		player->GetComponent<CapsuleCollider>()->SetCenter({0.f, 1.f, 0.f});
		player->GetComponent<CapsuleCollider>()->InitRenderObj();
		player->GetComponent<Rigidbody>()->InitRigidBody();
		player->GetComponent<Rigidbody>()->SetPhysAxisLock(false, false, false, true, true, true);

		std::shared_ptr<GameObject> playerAttack_1 = CreateEmpty();
		std::shared_ptr<GameObject> playerAttack_2 = CreateEmpty();
		std::shared_ptr<GameObject> playerAttack_3 = CreateEmpty();

		player->AddComponent<PlayerControllerMK2>();
		player->AddComponent<PlayerAttackController>()->AddColliders(playerAttack_1, playerAttack_2, playerAttack_3);
		player->AddComponent<OnHitTrigger>();

		std::shared_ptr<GameObject> dummy = CreateEmpty();
		dummy->AddComponent<MeshRenderer>()->Init("dog.model");
		dummy->GetComponent<Transform>()->SetPosition({ 5.f, 5.f, -15.f });
		dummy->AddComponent<Rigidbody>();
		std::string enemyLayer = "Enemy";
		dummy->AddComponent<CapsuleCollider>()->SetLayer(enemyLayer);
		dummy->GetComponent<CapsuleCollider>()->SetCenter({ 0.f, 1.f, 0.f });
		dummy->GetComponent<CapsuleCollider>()->InitRenderObj();
		dummy->GetComponent<Rigidbody>()->InitRigidBody();
		dummy->GetComponent<Rigidbody>()->SetPhysAxisLock(false, false, false, true, true, true);
		dummy->AddComponent<DamageTest>();
	
		/*std::shared_ptr<GameObject> camera = CreateEmpty();
		this->SetMainCamera(camera->AddComponent<Camera>());
		camera->GetComponent<Transform>()->SetPosition(Vector3{ 0.f, 10.f, -20.f });
		camera->GetComponent<Transform>()->SetRotation(Vector3{ -45.f, 0.f, 45.f });
		camera->GetComponent<Transform>()->LookAt(player->GetComponent<Transform>()->GetWorldPosition());
		camera->AddComponent<BasicCameraController>();*/
		//player->GetComponent<Transform>()->SetScale(Vector3{60.0f, 60.0f, 60.0f});

		/*std::shared_ptr<GameObject> effect = CreateEmpty();
		effect->AddComponent<GameEngineSpace::SpriteEffect>()->Init("effect.model");
		effect->GetComponent<Transform>()->SetPosition({5.0f, 10.f, 0.f});
		effect->GetComponent<Transform>()->SetScale({ 2.0f, 1.f, 2.f });*/

		/*
		std::shared_ptr<GameObject> cock = CreateEmpty();
		cock->AddComponent<MeshRenderer>()->Init("cock.model");
		cock->GetComponent<Transform>()->SetPosition({ 0.f, 10.f, 0.f });

		std::shared_ptr<GameObject> dog = CreateEmpty();
		dog->AddComponent<MeshRenderer>()->Init("dog.model");
		dog->GetComponent<Transform>()->SetPosition({ 0.f, 1.5f, 10.f });*/

		// 네비매쉬 테스트 잠깐 꺼논다
		//std::shared_ptr<GameObject> red = CreateEmpty();
		//red->AddComponent<MeshRenderer>()->Init("red.model");
		//red->GetComponent<Transform>()->SetPosition({ 5.f, 5.f, -15.f });
		//red->AddComponent<NavMeshAgent>();
		//red->AddComponent<Rigidbody>();
		//std::string _layer = "ENEMY";
		//red->AddComponent<CapsuleCollider>()->SetLayer(_layer);
		//red->GetComponent<CapsuleCollider>()->SetCenter({0.f, 1.0f, 0.f});
		//red->GetComponent<CapsuleCollider>()->InitRenderObj();
		//red->GetComponent<Rigidbody>()->InitRigidBody();
		//red->GetComponent<Rigidbody>()->SetPhysAxisLock(false, false, false, true, true, true);
		//red->AddComponent<MoveController>();
		//red->GetComponent<MoveController>()->SetTarget(player->GetComponent<Transform>());

		/*std::shared_ptr<GameObject> toad = CreateEmpty();
		toad->AddComponent<MeshRenderer>()->Init("toad.model");
		toad->GetComponent<Transform>()->SetPosition({ 0.f, -1.f, -15.f });*/

		/*std::shared_ptr<GameObject> grid = CreateEmpty();
		grid->AddComponent<PosColorRenderer>()->Init("Grid");*/

		/*std::shared_ptr<GameObject> axis = CreateEmpty();
		axis->AddComponent<PosColorRenderer>()->Init("Axis");*/

		/*std::shared_ptr<GameObject> box = CreateEmpty();
		box->AddComponent<PosColorRenderer>()->Init("WireCube");
		box->GetComponent<Transform>()->SetPosition({0.f, 5.0f, 0.f});*/

		/*std::shared_ptr<GameObject> normalLoby = CreateEmpty();
		normalLoby->AddComponent<MeshRenderer>()->Init("Loby");*/
		//normalLoby->GetComponent<Transform>()->SetScale(Vector3{ 60.0f, 60.0f, 60.0f });


		/*std::shared_ptr<GameObject> genji = CreateEmpty();
		genji->AddComponent<LegacyRenderer>()->Init("Resources/Model/genji_max.ASE", 4, 6,L"Resources/Texture/000000002405.dds", 7, L"Resources/Texture/000000002406_b.dds");
		genji->GetComponent<Transform>()->SetPosition(Vector3{5.f, 0.f, 0.f});*/

		/*std::shared_ptr<GameObject> testLight = CreateEmpty();
		testLight->GetComponent<Transform>()->SetPosition({0.f, 20.f, 0.f});
		testLight->AddComponent<Light>()->ChangeType(LightType::Point);
		testLight->GetComponent<Light>()->SetRange(100.f);
		testLight->GetComponent<Light>()->SetColor({1.0f, 1.0f, 1.0f});
		testLight->GetComponent<Light>()->SetPower(2.f);*/

		/*
		std::shared_ptr<GameObject> testLight2 = CreateEmpty();
		testLight2->GetComponent<Transform>()->SetPosition({ 10.f, 20.f, 0.f });
		testLight2->AddComponent<Light>()->ChangeType(LightType::Point);
		testLight2->GetComponent<Light>()->SetRange(50.f);
		testLight2->GetComponent<Light>()->SetColor({ 0.0f, 1.0f, 0.0f });
		testLight2->GetComponent<Light>()->SetPower(2.f);

		std::shared_ptr<GameObject> testLight3 = CreateEmpty();
		testLight3->GetComponent<Transform>()->SetPosition({ 0.f, 20.f, 10.f });
		testLight3->AddComponent<Light>()->ChangeType(LightType::Point);
		testLight3->GetComponent<Light>()->SetRange(150.f);
		testLight3->GetComponent<Light>()->SetColor({ 1.0f, 0.0f, 0.0f });
		testLight3->GetComponent<Light>()->SetPower(2.f);

		std::shared_ptr<GameObject> testLight4 = CreateEmpty();
		testLight4->GetComponent<Transform>()->SetPosition({ -10.f, 20.f, 0.f });
		testLight4->AddComponent<Light>()->ChangeType(LightType::Spot);
		testLight4->GetComponent<Light>()->SetRange(70.f);
		testLight4->GetComponent<Light>()->SetColor({ 0.0f, 0.0f, 1.0f });
		testLight4->GetComponent<Light>()->SetPower(2.f);
		*/
		SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->GetGameObject()->AddComponent<CameraController>();

		CreateUI();
	}
	
	std::shared_ptr<GameObject> DemoScene::CreateUI()
	{
		std::shared_ptr<GameObject> ui = CreateEmpty();
		ui->AddComponent<InGameUIController>();
		return ui;
	}
}