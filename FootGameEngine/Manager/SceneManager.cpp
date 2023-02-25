#include "GamePch.h"
#include "SceneManager.h"

#include "SceneBase.h"
#include "Object/GameObject.h"
#include "Component/Camera.h"
#include "Component/BoxCollider.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "Component/Light.h"
#include "PhysicsManager.h"
#include "Timer.h"

#include "InputManager.h"
#include "UIManager.h"
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "NavigationManager.h"
#include "Component/BasicCameraController.h"

namespace GameEngineSpace
{
	std::shared_ptr<SceneManager> SceneManager::instance = nullptr;

	void SceneManager::LoadSceneData(std::string sceneName)
	{
		auto sceneData = ResourceManager::GetInstance()->LoadUnityScene("BinaryFile/Scene/" + sceneName + ".scene");

		auto scene = SceneManager::GetInstance()->GetCurrentScene();

		GraphicsManager::GetInstance()->PassAmbientSkyColor({sceneData->ambientSkyColor.x,
															 sceneData->ambientSkyColor.y,
															 sceneData->ambientSkyColor.z,
															 sceneData->ambientSkyColor.w });

		for (int i = 0; i < sceneData->gameObjects.size(); i++)
		{
			// 게임오브젝트 생성
			std::shared_ptr<GameObject> gameObject = scene->CreateEmpty();

			std::wstring gameObjectName;

			gameObjectName.assign(sceneData->gameObjects[i].name.begin(), sceneData->gameObjects[i].name.end());

			// 게임오브젝트 이름 세팅
			gameObject->SetName(gameObjectName);

			// 게임오브젝트 태그 세팅
			if(sceneData->gameObjects[i].tag != "Untagged")
				gameObject->SetTag(sceneData->gameObjects[i].tag);

			// 게임오브젝트 fileID 세팅
			gameObject->SetFileID(sceneData->gameObjects[i].gameObjectID);

			if (sceneData->gameObjects[i].transform != nullptr)
			{
				auto tmpTransform = gameObject->GetComponent<Transform>();

				tmpTransform->gameObjectID = sceneData->gameObjects[i].transform->gameObjectID;
				tmpTransform->componentID = sceneData->gameObjects[i].transform->componentID;
				tmpTransform->childIDList = sceneData->gameObjects[i].transform->childIDList;

				tmpTransform->SetPosition(Vector3{
					sceneData->gameObjects[i].transform->localPosition.x,
					sceneData->gameObjects[i].transform->localPosition.y,
					sceneData->gameObjects[i].transform->localPosition.z });

				tmpTransform->SetRotation(Vector3{
					sceneData->gameObjects[i].transform->localRotation.x,
					sceneData->gameObjects[i].transform->localRotation.y,
					sceneData->gameObjects[i].transform->localRotation.z });

				tmpTransform->SetScale(Vector3{
					sceneData->gameObjects[i].transform->localScale.x,
					sceneData->gameObjects[i].transform->localScale.y,
					sceneData->gameObjects[i].transform->localScale.z });
			}

			if (sceneData->gameObjects[i].camera != nullptr)
			{
				scene->SetMainCamera(gameObject->AddComponent<Camera>());

				auto tmpCamera = gameObject->GetComponent<Camera>();

				tmpCamera->gameObjectID = sceneData->gameObjects[i].camera->gameObjectID;
				tmpCamera->componentID = sceneData->gameObjects[i].camera->componentID;

				if (sceneData->gameObjects[i].camera->projectionMatrixMode == 0)
					tmpCamera->UpdateProjMatrix(ProjType::PERSPECTIVE);
				else
					tmpCamera->UpdateProjMatrix(ProjType::ORTHOGRAPHIC);

				tmpCamera->SetNearZ(sceneData->gameObjects[i].camera->nearPlane);

				tmpCamera->SetFarZ(sceneData->gameObjects[i].camera->farPlane);

				//gameObject->AddComponent<BasicCameraController>();

				//gameObject->GetComponent<Transform>()->LookAt(Vector3{ 0.f, 0.f, 0.f });
			}

			if (sceneData->gameObjects[i].light != nullptr)
			{
				/*
				*/
				auto tmpLight = gameObject->AddComponent<Light>();

				if (sceneData->gameObjects[i].light->type == 0)
					tmpLight->ChangeType(LightType::Spot);
				if (sceneData->gameObjects[i].light->type == 1)
					tmpLight->ChangeType(LightType::Directional);
				if (sceneData->gameObjects[i].light->type == 2)
					tmpLight->ChangeType(LightType::Point);

				SimpleMath::Vector3 color = { sceneData->gameObjects[i].light->color.x, sceneData->gameObjects[i].light->color.y, sceneData->gameObjects[i].light->color.z };

				tmpLight->SetColor(color);

				tmpLight->SetPower(sceneData->gameObjects[i].light->intensity);

				tmpLight->SetRange(sceneData->gameObjects[i].light->range);
				tmpLight->SetOuterSpotAngle(sceneData->gameObjects[i].light->outerSpotAngle);
				tmpLight->SetInnerSpotAngle(sceneData->gameObjects[i].light->innerSpotAngle);
				
				if(sceneData->gameObjects[i].light->shadow == 0)
					tmpLight->SetIsShadow(false);
				else
					tmpLight->SetIsShadow(true);
			}

			if (sceneData->gameObjects[i].boxCollider != nullptr)
			{
				auto tmpCollider = gameObject->AddComponent<BoxCollider>();

				tmpCollider->gameObjectID = sceneData->gameObjects[i].boxCollider->gameObjectID;
				tmpCollider->componentID = sceneData->gameObjects[i].boxCollider->componentID;

				// 일단 8 ground, 9 wall
				if(sceneData->gameObjects[i].layer == 8)
					tmpCollider->SetLayer("Plane");
				else if(sceneData->gameObjects[i].layer == 9)
					tmpCollider->SetLayer("Wall");

				tmpCollider->SetTrigger(sceneData->gameObjects[i].boxCollider->isTrigger);

				tmpCollider->SetSize(Vector3{
					sceneData->gameObjects[i].boxCollider->size.x,
					sceneData->gameObjects[i].boxCollider->size.y,
					sceneData->gameObjects[i].boxCollider->size.z });

				tmpCollider->InitRenderObj();
			}

			if (sceneData->gameObjects[i].sphereCollider != nullptr)
			{
				/*gameObject->AddComponent<SphereCollider>();

				gameObject->GetComponent<SphereCollider>()->SetRadius(sceneData->gameObjects[i].sphereCollider->radius);

				gameObject->GetComponent<SphereCollider>()->SetLocalCenter(Vector3{
				sceneData->gameObjects[i].sphereCollider->center.x,
				sceneData->gameObjects[i].sphereCollider->center.y,
				sceneData->gameObjects[i].sphereCollider->center.z });*/
			}

			if (sceneData->gameObjects[i].meshFilter != nullptr)
			{
				if (sceneData->gameObjects[i].meshFilter->meshName == "CubeMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("CubeMesh");
				}
				if (sceneData->gameObjects[i].meshFilter->meshName == "SphereMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("SphereMesh");
				}
				if (sceneData->gameObjects[i].meshFilter->meshName == "PlaneMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("PlaneMesh");
				}
				if (sceneData->gameObjects[i].meshFilter->meshName == "PlaneMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("PlaneMesh");
				}

				auto tmpMeshRenderer = gameObject->GetComponent<MeshRenderer>();

				// 일단 없는건 하지말자 ex) Cylinder..
				if(tmpMeshRenderer == nullptr)
					continue;

				// meshrenderer 는 meshfilter로부터 가져오자..
				tmpMeshRenderer->gameObjectID = sceneData->gameObjects[i].meshFilter->gameObjectID;
				tmpMeshRenderer->componentID = sceneData->gameObjects[i].meshFilter->componentID;
			}

			// gameObject가 Light를 들고있었다면..
			// unity에서는 회전값으로 빛의 방향을 설정하고 있기때문에 이렇게 구해준다.
			if (gameObject->GetComponent<Light>() != nullptr)
			{
				gameObject->GetComponent<Light>()->SetDirection(gameObject->GetComponent<Transform>()->GetLook());
			}
		}

		/// <summary>
		///	부모가 있다면 계층 구조로..
		/// </summary>
		auto gameObjectInScene = scene->GetGameObjectInScene();

		for (int i = 0; i < gameObjectInScene.size(); i++)
		{
			if (!gameObjectInScene[i]->GetComponent<Transform>()->childIDList.empty())
			{
				for (auto& childID : gameObjectInScene[i]->GetComponent<Transform>()->childIDList)
				{
					// 자식이 있는지 찾는다.
					auto it = find_if(gameObjectInScene.begin(), gameObjectInScene.end(), [&id = childID](const std::shared_ptr<GameObject>& obj)->bool {return (obj->GetComponent<Transform>()->componentID == id); });

					// 자식을 찾았다면
					if (it != gameObjectInScene.end())
					{
						gameObjectInScene[i]->SetChild(*it);
					}
				}
			}
		}

		for (int i = 0; i < sceneData->prefabs.size(); i++)
		{
			// 게임오브젝트 생성
			std::shared_ptr<GameObject> gameObject = scene->CreateEmpty();

			std::wstring gameObjectName;

			gameObjectName.assign(sceneData->prefabs[i].name.begin(), sceneData->prefabs[i].name.end());

			// 게임오브젝트 이름 세팅
			gameObject->SetName(gameObjectName);

			auto tmpTransform = gameObject->AddComponent<Transform>();

			tmpTransform->SetPosition(Vector3{
				sceneData->prefabs[i].transform.localPosition.x,
				sceneData->prefabs[i].transform.localPosition.y,
				sceneData->prefabs[i].transform.localPosition.z });

			tmpTransform->SetRotation(Vector3{
				sceneData->prefabs[i].transform.localRotation.x,
				sceneData->prefabs[i].transform.localRotation.y,
				sceneData->prefabs[i].transform.localRotation.z });

			tmpTransform->SetScale(Vector3{
				sceneData->prefabs[i].transform.localScale.x,
				sceneData->prefabs[i].transform.localScale.y,
				sceneData->prefabs[i].transform.localScale.z });

			gameObject->AddComponent<MeshRenderer>()->Init(sceneData->prefabs[i].name + ".model");
		}
	}

	std::shared_ptr<GameObject> SceneManager::FindObject(std::string uid)
	{
		int _uid = std::stoi(uid);
		auto objectVector = GetCurrentScene()->GetGameObjectInScene();
		for (int i = 0; i < objectVector.size(); i++)
		{
			if (objectVector[i]->GetObjectID() == _uid)
				return objectVector[i];
		}

		return nullptr;
	}

	std::shared_ptr<GameEngineSpace::GameObject> SceneManager::FindObjectName(tstring name)
	{
		auto objectVector = GetCurrentScene()->GetGameObjectInScene();
		for (int i = 0; i < objectVector.size(); i++)
		{
			if (objectVector[i]->GetName() == name)
				return objectVector[i];
		}

		return nullptr;
	}

	void SceneManager::ChangeScene()
	{
		if (currentScene != nullptr)
		{
			// 기존 Scene의 내용을 지워준다..
			currentScene->ClearScene();
			std::string name;
			tstring tname = currentScene->sceneName;
			name.assign(tname.begin(), tname.end());
			PhysicsManager::GetInstance()->DeleteScene(name);
			PhysicsManager::GetInstance()->CollidersClear();
			// 렌더러의 벡터 지워주기
				// 사실 이게 리소스 매니저가 나오면 거기서 처리 해줘야한다
				// 지금은 씬 하나 만들 때마다 중복되는 리소스 관리 없이
				// 생성한다는 문제가 있다.
			//GraphicsManager::GetInstance()->GetRenderer()->ClearRenderVector();
			
		}

		// 여기서 포인트 라이트를 위한 작업 => Reset Data
		GraphicsManager::GetInstance()->GetRenderer()->ResetShadowPass();

		// navmesh reset
		NavigationManager::GetInstance()->Release();

		currentScene = scenes.at(reservedLoadSceneName);
		std::string name;
		tstring tname = currentScene->sceneName;
		name.assign(tname.begin(), tname.end());

		reservedLoadScene = false;

		isChangeScene = true;

		// 씬 초기화.
		PhysicsManager::GetInstance()->AddScene(name);
		PhysicsManager::GetInstance()->SetCurrentScene(name);
		currentScene->BuildScene();
		// 이 때 오브젝트를 init 해주자
		//GraphicsManager::GetInstance()->GetRenderer()->InitObject();
		currentScene->Awake();
		currentScene->Start();
	}

	std::shared_ptr<SceneManager> SceneManager::GetInstance()
	{
		// 인스턴스가 없으면 만들어주고 있으면 인스턴스 리턴
		if (instance == nullptr)
		{
			instance = std::make_shared<SceneManager>();
		}

		return instance;
	}

	void SceneManager::Init()
	{
		printf("SceneManager Init Clear\n");

		// 이후에 물리를 넣으면 여기서 물리를 Init해준다.
		physicsManager = PhysicsManager::GetInstance();

		physicsManager->Init();

		printf("PhysicsManager Init Clear\n");

	}

	void SceneManager::Release()
	{
		currentScene.reset();

		physicsManager->Release();

		// 씬 전체 릴리즈
		for (auto& scene : scenes)
		{
			scene.second->ClearScene();
		}
	}

	void SceneManager::Update()
	{
		// 현재 씬이 있다면..
		if (currentScene != nullptr)
		{
			float deltaTime = Timer::GetInstance()->DeltaTime();

			// 델타 타임 테스트용 코드
				// 틱이 변했으면 -> 함수에 의해 변경된 값을 사용한다.
			if (isChangedTick == true)
			{
				deltaTime *= deltaMultiple;
				// 음수 체크를 해준다.
				if (deltaTime < 0.0f)
					deltaTime = 0.0f;
			}

			// tick이 튀는거 방지
			if (isChangeScene == true)
			{
				isChangeScene = false;
				deltaTime = 0.0166f;
			}

			// 물리 Simulate를 위한 고정된 tick 주기의 FixedUpdate
			currentScene->FixedUpdate(deltaTime);

			// 씬 업데이트 전 충돌 업데이트..
			physicsManager->Simulate(deltaTime);

			// 순서대로 업데이트 해준다.
			currentScene->PreUpdate(deltaTime);
			currentScene->Update(deltaTime);		
			
			UIManager::GetInstance()->Update();
			currentScene->LateUpdate(deltaTime);

			// 이후 렌더 => 데이터 이동
			currentScene->Render(deltaTime);

			if (InputManager::GetInstance()->GetInputState(VK_F1, KeyState::DOWN))
			{
				bool selectMode = UIManager::GetInstance()->GetMainCanvas()->GetSelectMode();
				UIManager::GetInstance()->GetMainCanvas()->SetSelectMode(!selectMode);
			}

			auto hWnd = WindowManager::GetInstance()->GetHWND();

			// Key 입력 윈도우 크기 변경 테스트
			/*
			*/
			if (InputManager::GetInstance()->GetInputState(VK_F2, KeyState::DOWN))
			{
				SetWindowPos(hWnd, nullptr, 0, 0, 1920, 1080, SWP_NOMOVE | SWP_NOZORDER);
			}

			if (InputManager::GetInstance()->GetInputState(VK_F3, KeyState::DOWN))
			{
				SetWindowPos(hWnd, nullptr, 0, 0, 1600, 900, SWP_NOMOVE | SWP_NOZORDER);
			}

			if (InputManager::GetInstance()->GetInputState(VK_F4, KeyState::DOWN))
			{
				SetWindowPos(hWnd, nullptr, 0, 0, 1280, 720, SWP_NOMOVE | SWP_NOZORDER);
			}

			// 디버깅
			if (InputManager::GetInstance()->GetInputState(VK_TAB, KeyState::DOWN))
			{
				bool debug = currentScene->GetDebugRender();
				currentScene->SetDebugRender(!debug);
			}

			// 그리고 OnEnable..
			currentScene->OnEnable();
		}

		// 씬 변경이 예정되어 있으면.. 바꿔준다.
		if (reservedLoadScene)
		{
			ChangeScene();
		}
	}

	void SceneManager::AddGameScene(std::shared_ptr<SceneBase> _scene)
	{
		if (_scene != nullptr)
		{
			scenes.insert({ _scene->GetSceneName(), _scene });
		}
	}

	void SceneManager::LoadScene(tstring _sceneName)
	{
		// 씬이 Map에 존재하는가.
		assert(scenes.find(_sceneName) != scenes.end());

		// 씬을 예약한다.
		reservedLoadScene = true;
		reservedLoadSceneName = _sceneName;
	}

	void SceneManager::UpdateMainCameraAspectRatio(uint32 _screenWidth, uint32 _screenHeight)
	{
		if (currentScene != nullptr)
		{
			std::shared_ptr<Camera> _mainCamera = currentScene->GetMainCamera();

			if (_mainCamera != nullptr)
			{
				currentScene->GetMainCamera()->SetAspectRatio(static_cast<float>(_screenWidth) / _screenHeight);
			}
		}
	}

	void SceneManager::ChangeTick(float multiple)
	{
		isChangedTick = true;

		deltaMultiple = multiple;
	}
}
