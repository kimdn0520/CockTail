#include "GamePch.h"
#include "SceneBase.h"

#include "Object/GameObject.h"
#include "Component/RendererBase.h"
#include "GraphicsManager.h"

#include "Timer.h"
#include "UIManager.h"

#include "Component/ColliderBase.h"

#include "Component/Camera.h"
#include "InputManager.h"
#include "GraphicsManager.h"


namespace GameEngineSpace
{
#ifdef _DEBUG
	bool SceneBase::debugRender = true;
#else
	bool SceneBase::debugRender = false;
#endif

	SceneBase::SceneBase(tstring sceneName) :
		sceneName(sceneName)
		, mainCamera(nullptr)
		, gameObjectInScene(std::vector<std::shared_ptr<GameObject>>())
		, renderObjInScene(std::vector<std::shared_ptr<RendererBase>>())
		, colliderObjInScene(std::vector<std::shared_ptr<ColliderBase>>())
	{

	}

	SceneBase::~SceneBase()
	{
	}

	void SceneBase::Awake()
	{
		// 최초 시작시..
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Awake();
		}
	}

	void SceneBase::Start()
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Start();
		}
	}

	void SceneBase::PreUpdate(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->PreUpdate(tick);
		}
	}

	void SceneBase::Update(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Update(tick);
		}
	}

	void SceneBase::LateUpdate(float tick)
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->LateUpdate(tick);
		}
	}

	void SceneBase::Render(float tick)
	{
		// 각 오브젝트들의 렌더전 작업
		for (auto& renderObj : renderObjInScene)
		{
			// TODO : 바운딩 볼륨 생기면 해당 영역에서 프러스텀 컬링을 해줍니다.

			if (renderObj->GetIsEnable())
			{
				renderObj->PrepareRender(tick);
			}
		}

		auto renderer = GraphicsManager::GetInstance()->GetRenderer();

		renderer->BeginRender();
		//renderer->DebugRender();

		// 그려진 다음 Collider Render
		for (auto& collider : colliderObjInScene)
		{
			if (collider->GetIsEnable())
				collider->ColliderRender();
		}
		renderer->Render();

		// IMGUI 렌더
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->DebugIMGUIRender(tick);
		}
		
		UIManager::GetInstance()->SelectGUIRender();

		// Scene이 다 그려지고, UIManager 렌더
		UIManager::GetInstance()->Render(tick);

		renderer->EndRender();

	}

	void SceneBase::OnEnable()
	{
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->OnEnable();
		}
	}

	void SceneBase::ClearScene()
	{
		// 내부적으로 지워준다.
		// 순서대로 하나씩 리셋 후 마지막으로 릴리즈
		for (auto& gameObj : gameObjectInScene)
		{
			gameObj->Release();
		}

		gameObjectInScene.clear();
		renderObjInScene.clear();
		colliderObjInScene.clear();

		// UI 지워주기
		//UIManager::GetInstance()->ClearUI();
	}

	void SceneBase::AddGameObject(std::shared_ptr<GameObject> newGameObj)
	{
		gameObjectInScene.push_back(newGameObj);

		// 자식들이 있으면 자식들도 넣어준다.
		const std::vector<std::shared_ptr<GameObject>> children = newGameObj->GetChildren();

		for (auto child : children)
		{
			this->AddGameObject(child);
		}
	}

	std::shared_ptr<GameObject> SceneBase::CreateEmpty()
	{
		// 빈게임 오브젝트를 만들고.. 트랜스폼을 붙인 뒤 넣어준다.
		std::shared_ptr<GameObject> newGameObj = std::make_shared<GameObject>();

		newGameObj->AddComponent<Transform>();

		AddGameObject(newGameObj);

		// 반환.
		return newGameObj;
	}

	void SceneBase::AddRenderer(std::shared_ptr<RendererBase> renderObj)
	{
		// 지금 이 부분이, 그래픽스에도 존재는 함.
		renderObjInScene.push_back(renderObj);
	}

	void SceneBase::AddCollider(std::shared_ptr<ColliderBase> colliderObj)
	{
		colliderObjInScene.push_back(colliderObj);
	}

}
