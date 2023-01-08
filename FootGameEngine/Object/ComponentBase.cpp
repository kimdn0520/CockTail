#include "GamePch.h"
#include "ComponentBase.h"

namespace GameEngineSpace
{
	ComponentBase::ComponentBase(std::weak_ptr<GameObject> _gameObject, ComponentType _componentType) :
		Object()
		, gameObject(_gameObject)
		, isStarted(false)
		, componentType(_componentType)
	{

	}

	ComponentBase::~ComponentBase()
	{

	}

	void ComponentBase::Release()
	{
		if (gameObject.expired() != true)
		{
			gameObject.lock().reset();
		}
	}

}