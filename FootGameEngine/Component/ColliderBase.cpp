#include "GamePch.h"
#include "ColliderBase.h"

#include "Object/GameObject.h"
#include "Transform.h"

#include "SceneManager.h"
#include "SceneBase.h"

namespace GameEngineSpace
{
	ColliderBase::ColliderBase(std::weak_ptr<GameObject> gameObj)
		: ComponentBase(gameObj, ComponentType::COLLIDER)
		, isTrigger(false)
		, color(Vector3{1.0f, 1.0f, 1.0f})
	{
		transform = GetGameObject()->GetComponent<Transform>();
	}

	ColliderBase::~ColliderBase()
	{
	}

	// TODO Awake가 불리지 않아서 충돌 처리가 제대로 될 수 없을 수도 있다..!
		// 해당 이슈 참고해서 나중에 따로 부르는 식으로 고치자..
	void ColliderBase::Awake()
	{
		// 현재 씬에 콜라이더를 추가해준다.
			// 기본적으로 콜라이더가 게임 오브젝트에 붙으면 추가한다.
		SceneManager::GetInstance()->GetCurrentScene()->AddCollider(this->shared_from_this());
	}
}
