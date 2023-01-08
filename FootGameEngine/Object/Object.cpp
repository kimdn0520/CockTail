#include "GamePch.h"
#include "Object.h"

namespace GameEngineSpace
{
	uint32 ObjectIDGenerator::nextID = 0;

	Object::Object()
		: isEnable(true)
		, objectID(ObjectIDGenerator::IDGenerate())
	{
		
	}

	Object::~Object()
	{
	}

	void Object::Destroy()
	{
	}

	void Object::DontDestroyOnLoad(std::shared_ptr<Object> targetObject)
	{
	}

	void Object::Awake()
	{
	}

	void Object::Start()
	{
	}

	void Object::PreUpdate(float tick)
	{
	}

	void Object::Update(float tick)
	{
	}

	void Object::LateUpdate(float tick)
	{
	}

	void Object::DebugIMGUIRender(float tick)
	{
	}

	void Object::OnEnable()
	{
	}

	void Object::OnDisable()
	{
	}

	void Object::OnDestroy()
	{
	}
}
