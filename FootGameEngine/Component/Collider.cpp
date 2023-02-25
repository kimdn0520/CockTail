#include "GamePch.h"
#include "Collider.h"

#include "Object/GameObject.h"
#include "Transform.h"

#include "PhysicsManager.h"
#include "SceneManager.h"
#include "SceneBase.h"

namespace GameEngineSpace
{
	Collider::Collider(std::weak_ptr<GameObject> _object)
		: ComponentBase(_object, ComponentType::COLLIDER)
		, colliderType(MoolyEngine::GeometryType::INVALID)
		, bTrigger(false)
		, renderObj(nullptr)
	{
		transform = GetGameObject()->GetComponent<Transform>();
		rigidbody = GetGameObject()->GetComponent<Rigidbody>();
		layer = GetGameObject()->GetTag();

		if (rigidbody == nullptr)
			physicsType = MoolyEngine::PhysicsType::STATIC;

		else
			physicsType = MoolyEngine::PhysicsType::DYNAMIC;
	}

	Collider::~Collider()
	{

	}

	void Collider::Start()
	{
		PhysicsManager::GetInstance()->AddObject(this);
		if (rigidbody != nullptr)
		{
			// 추가하는 뭔가 필요함
		}

		PhysicsManager::GetInstance()->SetLayer(this, layer);

		SceneManager::GetInstance()->GetCurrentScene()->AddCollider(this->shared_from_this());
	}

	void Collider::FixedUpdate(float tick)
	{
		//PhysicsManager::GetInstance()->UpdateTransform(this);
		DirectX::SimpleMath::Vector3 _wPos = transform->GetWorldPosition();
		DirectX::SimpleMath::Quaternion _wRot = transform->GetWorldRotationQuat();

		MoolyEngine::Transform _transform;

		_transform.pos[0] = _wPos.x;
		_transform.pos[1] = _wPos.y;
		_transform.pos[2] = _wPos.z;

		_transform.quat[0] = _wRot.x;
		_transform.quat[1] = _wRot.y;
		_transform.quat[2] = _wRot.z;
		_transform.quat[3] = _wRot.w;

		PhysicsManager::GetInstance()->AddPhysicsQueue(this, _transform);
	}

	void Collider::PreUpdate(float tick)
	{
	}

	void Collider::Update(float tick)
	{
		//PhysicsManager::GetInstance()->SetTransform(this, _transform);
	}

	// 모든 transform이 연산이 되고 이녀석의 위치 정보를 physics에 넘겨준다.
	void Collider::LateUpdate(float tick)
	{
		////PhysicsManager::GetInstance()->UpdateTransform(this);
		//DirectX::SimpleMath::Vector3 _wPos = transform->GetWorldPosition();
		//DirectX::SimpleMath::Quaternion _wRot = transform->GetWorldRotationQuat();

		//MoolyEngine::Transform _transform;

		//_transform.pos[0] = _wPos.x;
		//_transform.pos[1] = _wPos.y;
		//_transform.pos[2] = _wPos.z;

		//_transform.quat[0] = _wRot.x;
		//_transform.quat[1] = _wRot.y;
		//_transform.quat[2] = _wRot.z;
		//_transform.quat[3] = _wRot.w;

		//PhysicsManager::GetInstance()->AddPhysicsQueue(this, _transform);
	}

	void Collider::OnEnable()
	{
		PhysicsManager::GetInstance()->EnableObject(this);
	}

	void Collider::OnDisable()
	{
		PhysicsManager::GetInstance()->DisableObject(this);
	}

	void Collider::SetLayer(const std::string& value)
	{
		layer = value;
		PhysicsManager::GetInstance()->SetLayer(this, value);
	}

	void Collider::AddTriggerEvent(MoolyEngine::IEventTrigger* callbackClass)
	{
		PhysicsManager::GetInstance()->AddTriggerEvent(this, callbackClass);
	}

	void Collider::AddColliderEvent(MoolyEngine::IEventCollider* callbackClass)
	{
		PhysicsManager::GetInstance()->AddColliderEvent(this, callbackClass);
	}

}
