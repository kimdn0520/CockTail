#include "GamePch.h"
#include "RendererBase.h"

#include "Object/GameObject.h"
#include "Transform.h"

#include "SceneManager.h"
#include "SceneBase.h"

namespace GameEngineSpace
{
	RendererBase::RendererBase(std::weak_ptr<GameObject> _object)
		: ComponentBase(_object, ComponentType::RENDERER)
	{
		// 트랜스폼 지정
		transform = GetGameObject()->GetComponent<Transform>();

		// 렌더러를 상속받은 자식들에게 특수화된 IDXObject를 생성. => 팩토리와 빌더 사용.
			// 포인터를 만들고 자식에서 팩토리를 통해 만든 뒤 여기에 넣어준다..!
			// reset사용하면 될듯..!
		//m_RenderObj = std::make_shared<IDXObject>();
	}

	RendererBase::~RendererBase()
	{

	}

	void RendererBase::Release()
	{
		transform.reset();
		__super::Release();
	}

	void RendererBase::Awake()
	{
		// 해당 렌더러를 씬에 등록한다.
		SceneManager::GetInstance()->GetCurrentScene()->AddRenderer(this->shared_from_this());
	}

}
