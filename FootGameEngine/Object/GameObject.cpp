#include "GamePch.h"
#include "GameObject.h"
#include "ComponentBase.h"
#include "ITriggerable.h"


namespace GameEngineSpace
{

	GameObject::GameObject() :
		name(TEXT("GameObject"))
		, tag("Default")
		, parent(std::weak_ptr<GameObject>())
		, children(std::vector<std::shared_ptr<GameObject>>())
		, components(std::vector<std::shared_ptr<ComponentBase>>())
		, layer(0)
	{
		// 생성 시에는 현재 객체와 관련된 스마트 포인터가 생성되지 않았기 때문에.. 일단은 빈공간으로 만들어준다.
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Release()
	{
		if (parent.expired() != true)
		{
			parent.lock().reset();
		}

		transform->Release();

		// 부모 객체가 Release되면.. 자식 객체들도 모두 Release 된다.
		for (auto& child : children)
		{
			child->Release();
		}

		// 컴포넌트도 사라진다.
		for (auto& component : components)
		{
			component->Release();
		}
	}

	// 아래의 오버라이딩 된 함수들은 모두 컴포넌트들의 해당 함수를 불러준다.
	void GameObject::Awake()
	{
		for (auto& component : components)
		{
			component->Awake();
		}
	}

	void GameObject::Start()
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->Start();
				// 스타트를 실행 했으니..
				component->SetIsStarted(true);
			}
		}
	}

	void GameObject::FixedUpdate(float tick)
	{
		for (auto& component : components)
		{
			component->FixedUpdate(tick);
		}
	}

	void GameObject::PreUpdate(float tick)
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->PreUpdate(tick);
			}
		}
	}

	void GameObject::Update(float tick)
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->Update(tick);
			}
		}
	}

	void GameObject::LateUpdate(float tick)
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->LateUpdate(tick);
			}
		}
	}

	void GameObject::DebugIMGUIRender(float tick)
	{
		for (auto& component : components)
		{
			if (component->GetIsEnable())
			{
				component->DebugIMGUIRender(tick);
			}
		}
	}

	std::shared_ptr<GameObject> GameObject::GetChild(tstring _name)
	{
		// 이름에 해당하는 자식을 찾아서 반환
		for (const auto& child : children)
		{
			if (child->GetName() == _name)
			{
				return child;
			}
		}

		// 차일드를 순회했지만 없으면.. null 리턴
		return nullptr;
	}

	void GameObject::SetChild(std::shared_ptr<GameObject> _child)
	{
		// 자식 목록에 이미 있으면 리턴 => 예외처리
		for (auto& child : children)
		{
			if (child == _child)
			{
				return;
			}
		}

		// 없다면.. 뒤에 넣어줍니다.
		children.push_back(_child);

		// 그리고 부모를 지정해줍니다.
			// shared_from_this == this 포인터
		_child->SetParent(shared_from_this());
	}

	void GameObject::SetGameObjectEnable(bool _isEnable)
	{
		// 컴포넌트 돌면서 다 is Enable
		this->SetIsEnable(_isEnable);

		for (auto& component : components)
		{
			component->SetIsEnable(_isEnable);

			if (_isEnable == false)
				continue;

			// is Enable이 true면서, Start를 하지 않았다면.. Start를 해준다.
			if (component->GetIsStarted() != true)
			{
				component->Start();
				component->SetIsStarted(true);
			}
		}

		// 자식이 있으면..
		for (auto& child : children)
		{
			child->SetGameObjectEnable(_isEnable);
		}

	}

	void GameObject::EraseParent()
	{
		// 부모를 지워줍니다..

		// 만약 부모가  null이면? 리턴
		if (parent.lock() == nullptr)
		{
			return;
		}

		// 부모의 자식들 목록에서 나를 지워줍니다..
		// 부모의 자식들 목록을 받고
		auto& parentsChildren = parent.lock()->GetChildren();

		// 나 자신을 받아서
		std::shared_ptr<GameObject> _sharedThis = shared_from_this();

		// 부모의 자식 목록 벡터를 돌면서 나를 지워줍니다.
		for (int i = 0; i < parentsChildren.size(); i++)
		{
			if (_sharedThis == parentsChildren[i])
			{
				// 시작점으로부터 i번째에 있는 나를 지워줍니다.
				parentsChildren.erase(parentsChildren.begin() + i);

				break;
			}
		}

		// 부모 포인터를 null로 만듭니다
		parent = std::weak_ptr<GameObject>();
	}

	bool GameObject::CompareTag(const std::string& tagName)
	{
		return tag == tagName;
	}

	void GameObject::SetParent(std::shared_ptr<GameObject> _parent)
	{
		// 기존 부모객체가 있었다면. 지워주고. 새로 넣어줍니다.
		this->EraseParent();

		parent = _parent;
	}

	void GameObject::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
	{
		// 트리거 이벤트 가능한 컴포넌트 들의 트리거 이벤트를 호출해준다.
		for (auto& triggerCom : triggerables)
			triggerCom->OnTriggerEnter(other);

	}

	void GameObject::OnTriggerStay(std::shared_ptr<ColliderBase> other)
	{
		for (auto& triggerCom : triggerables)
			triggerCom->OnTriggerStay(other);
	}

	void GameObject::OnTriggerExit(std::shared_ptr<ColliderBase> other)
	{
		for (auto& triggerCom : triggerables)
			triggerCom->OnTriggerExit(other);
	}
}