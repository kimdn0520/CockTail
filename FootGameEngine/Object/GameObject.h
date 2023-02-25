#pragma once
#include "Object.h"
#include "Component/Collider.h"
#include "Component/Transform.h"
#include "Component/Rigidbody.h"

namespace GameEngineSpace
{
	class ComponentBase;
	class Transform;
	class MeshRenderer;
	class ColliderBase;
	class ITriggerable;

	/**
	 * \brief 기본적으로 Scene에 클라이언트 영역에서 씬에 추가할 수 있는 게임 오브젝트 영역
	 *
	 * 내부적으로 부모, 자식과 같은 위계 구조 및 기능을 담당할 컴포넌트들을 가지고 있다.
	 *
	 * 기본적으로 트랜스폼을 기본적으로 가지고 있다.
	 *
	 * 2022.07.22(금) Dev.BlackFoot
	 */
	 // 게임 오브젝트는 다른 클래스에 상속 되지 않습니다. (부모 클래스가 되지 않습니다.) => final 키워드 사용
	 // 기존 Shared_ptr과 소유권을 공유하기 위해 enable_shared_from_this를 사용합니다.
	class GameObject final : public Object, public std::enable_shared_from_this<GameObject>
	{
		// 오브젝트의 이름
		tstring name;
		// 태그
		std::string tag;
		// 레이어
		uint32 layer;
		// 부모 포인터
		std::weak_ptr<GameObject> parent;
		// 자식 포인터
			// 자식은 여러개가 될 수 있습니다.
		std::vector<std::shared_ptr<GameObject>> children;
		// 중복되지 않는 컴포넌트
		std::vector<std::shared_ptr<ComponentBase>> components;
		// 트랜스폼을 가지고 있습니다.
		std::shared_ptr<Transform> transform;
		// 트리거 가능한 컴포넌트들을 가지고 있는 벡터.
			// 하나의 오브젝트에 다양한 콜라이더들이 붙을 수 있습니다.
		std::vector<std::shared_ptr<ITriggerable>> triggerables;

	public:
		// yamlparsing으로 부터 받은 fileID
		std::string fileID;

	public:
		GameObject();
		virtual ~GameObject();

		virtual void Release() override;

		// 오버 라이딩 된 함수들
		void Awake() override;
		void Start() override;
		void FixedUpdate(float tick) override;
		void PreUpdate(float tick) override;
		void Update(float tick) override;
		void LateUpdate(float tick) override;
		void DebugIMGUIRender(float tick) override;

		// 각종 게터 세터
		const tstring& GetName() { return name; }
		void SetName(const tstring& _name) { name = _name; }

		const std::string& GetTag() { return tag; }
		void SetTag(const std::string& _tag) { tag = _tag; }

		const uint32& GetLayer() { return layer; }
		void SetLayer(const uint32& _layer) { layer = _layer; }

		std::shared_ptr<GameObject> GetParent() { return parent.lock(); }

		std::shared_ptr<GameObject> GetChild(tstring _name);
		std::vector<std::shared_ptr<GameObject>>& GetChildren() { return children; }
		void SetChild(std::shared_ptr<GameObject> _child);

		void SetGameObjectEnable(bool _isEnable);

		std::string GetFileID() { return fileID; }
		void SetFileID(std::string id) { fileID = id; }

		void EraseParent();

		bool CompareTag(const std::string& tagName);

		std::vector<std::shared_ptr<ComponentBase>> GetComponents() { return components; }

	private:
		void SetParent(std::shared_ptr<GameObject> _parent);

	public:
		// 트리거 이벤트 세팅..!
		void OnTriggerEnter(std::shared_ptr<ColliderBase> other);
		void OnTriggerStay(std::shared_ptr<ColliderBase> other);
		void OnTriggerExit(std::shared_ptr<ColliderBase> other);

	public:
		// 컴포넌트 Get 및 Add
			// 특정 TComponent를 반환하는 GetComponent
		template <typename TComponent>
		std::shared_ptr<TComponent> GetComponent()
		{
			// 컴포넌트를 반드시 상속받아야 합니다. 컴포넌트 상속체크
			static_assert(std::is_base_of<ComponentBase, TComponent>::value, "TComponent는 ComponentBase를 상속받아야 합니다.");

			// 트랜스 폼이면 트랜스폼 반환 => 트랜스폼은 따로 컴포넌트 Set에 들어가 있지 않기 때문
				/// --- 공부노트 ---
				// 이때 cosntexper 즉 상수 표현식을 사용해줘야. 해당 조건식을 컴파일타임(?)에 검사해서
				// 템플릿을 만들 때 실수를 하지 않도록한다.
				///	---------------
			if constexpr (std::is_same<TComponent, Transform>::value == true)
				return transform;

			// 그게 아닐경우 Set을 순회하면서 찾는다.
			for (auto& component : components)
			{
				// 자식 포인터로 다이나믹 캐스팅을 해주고.. 해당 컴포넌트가 존재한다면 반환해준다.

				/// --- 공부 노트 ---
				// Components는 Shared_ptr<ComponentBase> (== ComponentBase*)의 set이고
				// 템플릿의 변수로 들어오는 TComponent는 ComponentBase를 상속한 특정 컴포넌트이다.
				// 따라서 하향 캐스팅을 dynamic_cast를 통해서 해주고, 해당 캐스팅의 결과가 nullptr이 아닐경우
				// TComponent가 Components에 존재한다는 것을 확인할 수 있게 된다.
				// Shared_ptr에는 바로 dynamic_cast를 쓸 수 없기 때문에, 다이나믹 캐스트와 동일한 동작을 하는
				// dynamic_pointer_cast를 사용한다.
				/// -----------------
				std::shared_ptr<TComponent> dcComponent = std::dynamic_pointer_cast<TComponent>(component);

				if (dcComponent != nullptr)
					return dcComponent;
			}

			// 모든 Set을 다 찾아도 컴포넌트가 존재하지 않으면 nullptr을 리턴한다.
			return nullptr;
		}

		// 컴포넌트를 받아서 Components에 추가해주는 함수.
			// 템플릿 형식을 활용해서..
			// AddComponent<TComponent>() 와 같은 형식으로 활용한다.
		template <typename TComponent>
		std::shared_ptr<TComponent> AddComponent()
		{
			// 위와 마찬가지로 컴포넌트 인지 확인해준다.
			static_assert(std::is_base_of<ComponentBase, TComponent>::value, "TComponent는 ComponentBase를 상속받아야 합니다.");

			auto tmp = GetComponent<TComponent>();

			// 이미있던거라면 넣어주지않는다.
			if (tmp != nullptr)
				return tmp;

			if constexpr (std::is_same<TComponent, Transform>::value == true)
			{
				// 트랜스 폼이면 그냥 m_TransForm에 넣어준다.
				transform = std::make_shared<Transform>(this->shared_from_this());

				components.emplace_back(transform);

				return transform;
			}

			std::shared_ptr<TComponent> tmpComponent = std::make_shared<TComponent>(this->shared_from_this());

			components.emplace_back(tmpComponent);

			// ComponetType에 따라 Update순서를 지정해준다.
			sort(components.begin(), components.end(), [](std::shared_ptr<ComponentBase>& a, std::shared_ptr<ComponentBase>& b) { return a->componentType < b->componentType; });

			// 트리거 가능한 컴포넌트면 해당 컴포넌트를 따로 캐싱해준다.
				// 이후에 이벤트를 보내주기 위함 
			if constexpr (std::is_base_of<ITriggerable, TComponent>::value == true)
				triggerables.push_back(static_cast<std::shared_ptr<ITriggerable>>(tmpComponent));

			return tmpComponent;
		}
	};

}

