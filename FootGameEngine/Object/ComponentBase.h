#pragma once
#include "Object.h"

namespace GameEngineSpace
{
	class GameObject;

	// 컴포넌트 타입을 정의해둔 EnumClass
		// 새로운 타입이 생기면 그 때 그 때 추가해보자..
	enum class ComponentType
	{
		TRANSFORM = 0,
		COLLIDER = 1,
		RENDERER = 2,
		MONOBEHAVIOR = 3,
		NAVAGENT = 4,
		CAMERA = 5,
		LIGHT = 6,
		ANIMATOR = 7,
		BONE = 8,
		CANVAS = 9,
		TEXT = 10,
		NONE = 11
	};

	/**
	 * \brief 모든 컴포넌트들의 기반이 되는 부모 클래스.
	 *
	 * 2022.07.22(금) Dev.BlackFoot
	 */
	class ComponentBase : public Object
	{
		// 해당 컴포넌트가 가리키고 있는 게임 오브젝트.
			// 순환참조의 가능성이 있기 때문에 Weak point를 사용한다.
		std::weak_ptr<GameObject> gameObject;

		// 스타트가 실행되었는가..! => 기본적으로 false.. 스타트가 실행되면 true로.
		bool isStarted;

	public:
		ComponentType componentType;

		// YamlParser에서 받았을경우 ID 부여
		std::string gameObjectID;
		std::string componentID;

	public:
		ComponentBase(std::weak_ptr<GameObject> _gameObject, ComponentType _componentType = ComponentType::NONE);

		virtual ~ComponentBase();

		virtual void Release() override;

		std::shared_ptr<GameObject> GetGameObject() { return gameObject.lock(); }

		bool GetIsStarted() { return isStarted; }
		void SetIsStarted(bool _isStarted) { isStarted = _isStarted; }
	};

	// 괄호에 대한 오퍼레이터 오버로딩
		// Inner class를 멤버 변수로 가지고 있기 때문에 아래에 작성
	struct ComponentOperator
	{
		// 컴포넌트 타입에 대한 순서 비교용 오퍼레이터..!
			// second 인자보다 first 인자가 우선순위가 높으면(숫자가 낮으면 == enumClass 상으로 위에 위치해 있으면)
			// true를 반환합니다.
		bool operator() (std::shared_ptr<ComponentBase> first, std::shared_ptr<ComponentBase> second)
		{
			return static_cast<uint32>(first->componentType) < static_cast<uint32>(second->componentType);
		}

	};

}
