#include "pch.h"
#include "DemoScene.h"

#include "Component/Camera.h"
#include "Component/BasicCameraController.h"
#include "Component/BoxCollider.h"
#include "Component/PosColorRenderer.h"
#include "Component/MeshRenderer.h"
#include "Component/Light.h"
#include "Component/SpriteEffect.h"
#include "Scripts/MoveController.h"


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
		SceneManager::GetInstance()->LoadSceneData("SampleScene");

		// DirectionalLight Test
		std::shared_ptr<GameObject> directionalLight = CreateEmpty();
		directionalLight->AddComponent<Light>();
		directionalLight->GetComponent<Light>()->ChangeType(LightType::Directional);
		directionalLight->GetComponent<Light>()->SetColor(SimpleMath::Vector3{ Colors::White });
		directionalLight->GetComponent<Light>()->SetDirection(SimpleMath::Vector3{ 0.5f, -0.5f, 0.5f });
		directionalLight->GetComponent<Light>()->SetPower(1.0f);

		std::shared_ptr<GameObject> player = CreateEmpty();
		player->AddComponent<MeshRenderer>()->Init("redani.model");

		/*player->AddComponent<BoxCollider>()->SetSize({1.f, 2.f, 1.f});
		player->GetComponent<BoxCollider>()->InitRenderObj("WireCube");*/

		/*std::shared_ptr<GameObject> camera = CreateEmpty();
		this->SetMainCamera(camera->AddComponent<Camera>());
		camera->GetComponent<Transform>()->SetPosition(Vector3{ 0.f, 10.f, 10.f });
		camera->GetComponent<Transform>()->SetRotation(Vector3{ -45.f, 0.f, 45.f });
		camera->GetComponent<Transform>()->LookAt(player->GetComponent<Transform>()->GetWorldPosition());*/

		//player->GetComponent<Transform>()->SetScale(Vector3{60.0f, 60.0f, 60.0f});
		player->AddComponent<MoveController>();

		std::shared_ptr<GameObject> effect = CreateEmpty();
		effect->AddComponent<SpriteEffect>()->Init("EffectMesh.model");
		effect->GetComponent<Transform>()->SetPosition({5.0f, 10.f, 0.f});
		effect->GetComponent<Transform>()->SetScale({ 2.0f, 1.f, 2.f });

		std::shared_ptr<GameObject> cock = CreateEmpty();
		cock->AddComponent<MeshRenderer>()->Init("cock.model");
		cock->GetComponent<Transform>()->SetPosition({0.f, 10.f, 0.f});

		std::shared_ptr<GameObject> dog = CreateEmpty();
		dog->AddComponent<MeshRenderer>()->Init("dog.model");
		dog->GetComponent<Transform>()->SetPosition({ 10.f, 10.f, 0.f });

		std::shared_ptr<GameObject> red = CreateEmpty();
		red->AddComponent<MeshRenderer>()->Init("red.model");
		red->GetComponent<Transform>()->SetPosition({ -10.f, 10.f, 0.f });

		std::shared_ptr<GameObject> toad = CreateEmpty();
		toad->AddComponent<MeshRenderer>()->Init("toad.model");
		toad->GetComponent<Transform>()->SetPosition({ 0.f, 10.f, 10.f });

		/*std::shared_ptr<GameObject> grid = CreateEmpty();
		grid->AddComponent<PosColorRenderer>()->Init("Grid");*/

		std::shared_ptr<GameObject> axis = CreateEmpty();
		axis->AddComponent<PosColorRenderer>()->Init("Axis");

		/*std::shared_ptr<GameObject> box = CreateEmpty();
		box->AddComponent<PosColorRenderer>()->Init("WireCube");
		box->GetComponent<Transform>()->SetPosition({0.f, 5.0f, 0.f});*/

		/*std::shared_ptr<GameObject> normalLoby = CreateEmpty();
		normalLoby->AddComponent<MeshRenderer>()->Init("Loby");*/ 
		//normalLoby->GetComponent<Transform>()->SetScale(Vector3{ 60.0f, 60.0f, 60.0f });


		/*std::shared_ptr<GameObject> genji = CreateEmpty();
		genji->AddComponent<LegacyRenderer>()->Init("Resources/Model/genji_max.ASE", 4, 6,L"Resources/Texture/000000002405.dds", 7, L"Resources/Texture/000000002406_b.dds");
		genji->GetComponent<Transform>()->SetPosition(Vector3{5.f, 0.f, 0.f});*/
	}
}