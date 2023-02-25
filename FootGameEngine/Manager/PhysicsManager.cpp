#include "GamePch.h"
#include "PhysicsManager.h"
#include "SceneManager.h"

#include "Object/GameObject.h"
#include "Component/Transform.h"
#include "Component/Collider.h"
#include "Component/BoxCollider.h"
#include "Component/SphereCollider.h"
#include "Component/CapsuleCollider.h"
#include "Component/RayCollider.h"

namespace GameEngineSpace
{
	std::shared_ptr<PhysicsManager> PhysicsManager::instance = nullptr;

	std::shared_ptr<PhysicsManager> PhysicsManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<PhysicsManager>();

		return instance;
	}

	void PhysicsManager::Init()
	{
		// TODO 구획 분리 나눠서 처리.
		physicsEngine = MoolyEngine::MoolyEngineInstance();

		printf("PhysicsManager Instance Clear\n");

		printf("PhysicsManager Initialize Start\n");
		physicsEngine->Initialize();
		printf("PhysicsManager Initialize End\n");
		physicsEngine->AddScene("Demo");
		physicsEngine->SetCurrentScene("Demo");

		physicsEngine->AddLayer("Default");
		physicsEngine->AddLayer("Player");
		physicsEngine->AddLayer("Plane");
		physicsEngine->AddLayer("Enemy");
		physicsEngine->AddLayer("Wall");
		physicsEngine->AddLayer("Trigger");
		physicsEngine->AddLayer("Slope");
		physicsEngine->AddLayer("CharacterBlocker");


		physicsEngine->SetLayerFilterData("CharacterBlocker", { "Default", "Plane", "Wall","Trigger", "Slope", "Player", "Enemy" });
		physicsEngine->SetLayerFilterData("Player", { "Enemy" });
		physicsEngine->SetLayerFilterData("Enemy", { "Player" });

		physicsEngine->Test();

		defaultQuat = EulerToQuat({ 0,0,90.0f });
		reverseQuat = EulerToQuat({ 0,0,-90.0f });
	}

	void PhysicsManager::AddScene(const std::string& sceneName)
	{
		physicsEngine->AddScene(sceneName);
	}

	void PhysicsManager::SetCurrentScene(const std::string& sceneName)
	{
		physicsEngine->SetCurrentScene(sceneName);

		physicsEngine->AddLayer("Default");
		physicsEngine->AddLayer("Player");
		physicsEngine->AddLayer("Plane");
		physicsEngine->AddLayer("Enemy");
		physicsEngine->AddLayer("Wall");
		physicsEngine->AddLayer("Trigger");
		physicsEngine->AddLayer("Slope");
		physicsEngine->AddLayer("CharacterBlocker");

		/*physicsEngine->SetLayerFilterData("Player", { "Enemy" });
		physicsEngine->SetLayerFilterData("Enemy", { "Player", "Enemy" });*/
		physicsEngine->SetLayerFilterData("CharacterBlocker", { "Default", "Plane", "Wall","Trigger", "Slope" });

		physicsEngine->SetLayerFilterData("CharacterBlocker", { "Default", "Plane", "Wall","Trigger", "Slope", "Player"});
		physicsEngine->SetLayerFilterData("Player", { "CharacterBlocker" });

		//physicsEngine->Test();
	}

	void PhysicsManager::ClearScene(const std::string& sceneName)
	{
		physicsEngine->ClearScene(sceneName);
	}

	void PhysicsManager::DeleteScene(const std::string& sceneName)
	{
		physicsEngine->DeleteScene(sceneName);
	}

	void PhysicsManager::AddObject(Collider* collider)
	{
		DirectX::SimpleMath::Vector3 _wPos = collider->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
		DirectX::SimpleMath::Quaternion _wRot = collider->GetGameObject()->GetComponent<Transform>()->GetWorldRotationQuat();

		DirectX::SimpleMath::Vector3 _offset = collider->GetCenter();

		MoolyEngine::Transform _input;
		_input.quat[0] = _wRot.x;
		_input.quat[1] = _wRot.y;
		_input.quat[2] = _wRot.z;
		_input.quat[3] = _wRot.w;

		_input.pos[0] = _wPos.x + _offset.x;
		_input.pos[1] = _wPos.y + _offset.y;
		_input.pos[2] = _wPos.z + _offset.z;

		switch (collider->GetColliderType())
		{
			case MoolyEngine::GeometryType::BOX:
			{
				collider->GetGameObject()->GetComponent<Transform>()->Update(0.0f);
				BoxCollider* boxCollider = static_cast<BoxCollider*>(collider);
				Vector3 _extend = boxCollider->GetSize();
				Vector3 _wScale = collider->GetGameObject()->GetComponent<Transform>()->GetWorldScale();
				_extend.x *= _wScale.x;
				_extend.y *= _wScale.y;
				_extend.z *= _wScale.z;
				boxCollider->SetSize(_extend);

				physicsEngine->CreateBoxActor(std::to_string(collider->GetGameObject()->GetObjectID()), "Box",
					boxCollider->GetPhysicsType(), _input, { _extend.x / 2.0f, _extend.y / 2.0f, _extend.z / 2.0f }, 1);
			}
			break;

			case MoolyEngine::GeometryType::SPHERE:
			{
				SphereCollider* sphereCollider = static_cast<SphereCollider*>(collider);
				float _radius = sphereCollider->GetRadius();

				physicsEngine->CreateSphereActor(std::to_string(collider->GetGameObject()->GetObjectID()), "Sphere", sphereCollider->GetPhysicsType(),
					_input, _radius, 1);
			}
			break;

			case MoolyEngine::GeometryType::CAPSULE:
			{
				collider->GetGameObject()->GetComponent<Transform>()->Update(0.0f);
				CapsuleCollider* capsuleCollider = static_cast<CapsuleCollider*>(collider);
				Quaternion _defaultRot = { 0, 0, 0.7071068f, 0.7071068f };
				_defaultRot *= _wRot;

				_input.quat[0] = _defaultRot.x;
				_input.quat[1] = _defaultRot.y;
				_input.quat[2] = _defaultRot.z;
				_input.quat[3] = _defaultRot.w;

				float _radius = capsuleCollider->GetRadius();
				float _height = capsuleCollider->GetHeight();

				physicsEngine->CreateCapsuleActor(std::to_string(collider->GetGameObject()->GetObjectID()), "Capsule", capsuleCollider->GetPhysicsType(),
					_input, _radius, _height / 2, 1);
			}
			break;

			default:
				break;
		}

		if (collider->IsTrigger())
		{
			physicsEngine->SetTriggerShape(std::to_string(collider->GetGameObject()->GetObjectID()), true);
			physicsEngine->SetLayer(std::to_string(collider->GetGameObject()->GetObjectID()), "TRIGGER");
		}

		physicsEngine->SetAngularDamping(std::to_string(collider->GetGameObject()->GetObjectID()), 5.0f);
		physicsEngine->SetLinearDamping(std::to_string(collider->GetGameObject()->GetObjectID()), 1.5f);
	}

	void PhysicsManager::DeleteObject(Collider* collider)
	{
		physicsEngine->DeleteActor(std::to_string(collider->GetGameObject()->GetObjectID()));
	}

	void PhysicsManager::EnableObject(Collider* collider)
	{
		physicsEngine->EnableActor(std::to_string(collider->GetGameObject()->GetObjectID()));
	}

	void PhysicsManager::DisableObject(Collider* collider)
	{
		physicsEngine->DisableActor(std::to_string(collider->GetGameObject()->GetObjectID()));
	}

	void PhysicsManager::CreatePlayerController(MoolyEngine::Vector3 pos)
	{
		physicsEngine->CreatePlayerController(pos);
	}

	void PhysicsManager::AddPhysicsQueue(Collider* collider, MoolyEngine::Transform& worldTransform)
	{
		colliders.emplace_back(collider, worldTransform);
	}

	void PhysicsManager::SetTransform(Collider* collider, MoolyEngine::Transform worldTransform)
	{
		if (collider->GetColliderType() == MoolyEngine::GeometryType::CAPSULE)
		{
			MoolyEngine::Transform _input;
			Vector3 _offset = collider->GetCenter();

			_input = worldTransform;

			Quaternion _quat(worldTransform.quat[0], worldTransform.quat[1], worldTransform.quat[2], worldTransform.quat[3]);
			Quaternion _defaultRot = defaultQuat;
			_defaultRot *= _quat;

			if (_defaultRot.w > 1.f)
				_defaultRot.w = 1.0f;

			_input.quat[0] = _defaultRot.x;
			_input.quat[1] = _defaultRot.y;
			_input.quat[2] = _defaultRot.z;
			_input.quat[3] = _defaultRot.w;

			_input.pos[0] += _offset.x;
			_input.pos[1] += _offset.y;
			_input.pos[2] += _offset.z;

			physicsEngine->SetTransform(std::to_string(collider->GetGameObject()->GetObjectID()), _input);
		}

		else
		{
			DirectX::SimpleMath::Vector3 _offset = collider->GetCenter();

			worldTransform.pos[0] += _offset.x;
			worldTransform.pos[1] += _offset.y;
			worldTransform.pos[2] += _offset.z;

			physicsEngine->SetTransform(std::to_string(collider->GetGameObject()->GetObjectID()), worldTransform);
		}
	}

	void PhysicsManager::UpdateTransform(Collider* collider)
	{
		MoolyEngine::Transform _output;

		if (physicsEngine->GetTransform(std::to_string(collider->GetGameObject()->GetObjectID()), _output))
		{
			if (collider->GetColliderType() == MoolyEngine::GeometryType::CAPSULE)
			{
				Vector3 _offset = collider->GetCenter();
				Vector3 _pos(_output.pos[0] - _offset.x, _output.pos[1] - _offset.y, _output.pos[2] - _offset.z);
				Quaternion _quat(_output.quat[0], _output.quat[1], _output.quat[2], _output.quat[3]);
				Quaternion _defaultRot = reverseQuat;

				_defaultRot *= _quat;
				_defaultRot.Normalize();
				collider->GetTransform()->SetPosition(_pos);
				collider->GetTransform()->SetRotation(_defaultRot);
			}

			else
			{
				DirectX::SimpleMath::Vector3 _offset = collider->GetCenter();
				DirectX::SimpleMath::Vector3 _pos(_output.pos[0] - _offset.x, _output.pos[1] - _offset.y, _output.pos[2] - _offset.z);
				DirectX::SimpleMath::Quaternion _rot(_output.quat[0], _output.quat[1], _output.quat[2], _output.quat[3]);

				collider->GetTransform()->SetPosition(_pos);
				collider->GetTransform()->SetRotation(_rot);
			}
		}
	}

	void PhysicsManager::AddForce(Collider* collider, MoolyEngine::Vector3 force)
	{
		physicsEngine->AddForce(std::to_string(collider->GetGameObject()->GetObjectID()), force);
	}

	void PhysicsManager::AddImpulse(Collider* collider, MoolyEngine::Vector3 impulse)
	{
		physicsEngine->AddImpulse(std::to_string(collider->GetGameObject()->GetObjectID()), impulse);
	}

	void PhysicsManager::AddTriggerEvent(Collider* collider, MoolyEngine::IEventTrigger* callback)
	{
		physicsEngine->AddTriggerEvent(std::to_string(collider->GetGameObject()->GetObjectID()), callback);
	}

	void PhysicsManager::AddColliderEvent(Collider* collider, MoolyEngine::IEventCollider* callback)
	{
		physicsEngine->AddColliderEvent(std::to_string(collider->GetGameObject()->GetObjectID()), callback);
	}

	void PhysicsManager::MovePlayer(Vector3 disp)
	{
		physicsEngine->MovePCC({disp.x, disp.y, disp.z});
	}

	void PhysicsManager::MoveKinematicObject(Collider* collider, MoolyEngine::Vector3 position)
	{

	}

	void PhysicsManager::SetAngularDamp(Collider* collider, float damp)
	{
		physicsEngine->SetAngularDamping(std::to_string(collider->GetGameObject()->GetObjectID()), damp);
	}

	void PhysicsManager::SetAxisLock(Collider* collider, MoolyEngine::PhysicsAxisLock lock)
	{
		physicsEngine->SetAxisLock(std::to_string(collider->GetGameObject()->GetObjectID()), lock);
	}

	void PhysicsManager::SetLinearDamp(Collider* collider, float damp)
	{
		physicsEngine->SetLinearDamping(std::to_string(collider->GetGameObject()->GetObjectID()), damp);
	}

	void PhysicsManager::SetMass(Collider* collider, float mass)
	{
		physicsEngine->SetMass(std::to_string(collider->GetGameObject()->GetObjectID()), mass);
	}

	void PhysicsManager::SetLinearVelocity(Collider* collider, MoolyEngine::Vector3 velocity)
	{
		physicsEngine->SetLinearVelocity(std::to_string(collider->GetGameObject()->GetObjectID()), velocity);
	}

	void PhysicsManager::SetAngularVelocity(Collider* collider, MoolyEngine::Vector3 angular)
	{
		physicsEngine->SetAngularVelocity(std::to_string(collider->GetGameObject()->GetObjectID()), angular);
	}

	void PhysicsManager::SetMaxLinearVelocity(Collider* collider, float value)
	{
		physicsEngine->SetMaxLinearVelocity(std::to_string(collider->GetGameObject()->GetObjectID()), value);
	}

	void PhysicsManager::SetMaxAngularVelocity(Collider* collider, float value)
	{
		physicsEngine->SetMaxAngularVelocity(std::to_string(collider->GetGameObject()->GetObjectID()), value);
	}

	void PhysicsManager::SetGravity(Collider* collider, bool value)
	{
		physicsEngine->SetGravity(std::to_string(collider->GetGameObject()->GetObjectID()), value);
	}

	void PhysicsManager::SetKinematic(Collider* collider, bool value)
	{
		physicsEngine->SetKinematic(std::to_string(collider->GetGameObject()->GetObjectID()), value);
	}

	void PhysicsManager::SetLayer(Collider* collider, std::string layer)
	{
		physicsEngine->SetLayer(std::to_string(collider->GetGameObject()->GetObjectID()), layer);
	}

	RaycastHit PhysicsManager::Raycast(MoolyEngine::Vector3 origin, MoolyEngine::Vector3 dir, float dist, std::vector<std::string> filterLayers, MoolyEngine::PhysicsType type)
	{
		MoolyEngine::RayCastHit hitInfo;
		bool ret = physicsEngine->Raycast(origin, dir, dist, filterLayers, type, hitInfo);

		if (ret == false)
			return RaycastHit();

		auto gameObject = SceneManager::GetInstance()->FindObject(hitInfo.objectName);

		if (gameObject == nullptr)
			return RaycastHit();

		return RaycastHit(hitInfo.objectName.c_str(), hitInfo.distance, ret, gameObject, hitInfo.normal, hitInfo.hitPos);
	}

	bool PhysicsManager::CheckBox(MoolyEngine::Vector3 center, MoolyEngine::Vector3 halfExtents, std::vector<std::string> filteredLayers)
	{
		return physicsEngine->CheckBox(center, halfExtents, filteredLayers);
	}

	DirectX::SimpleMath::Vector3 PhysicsManager::GetAngularVelocity(Collider* collider)
	{
		MoolyEngine::Vector3 _result;

		if (physicsEngine->GetAngularVelocity(std::to_string(collider->GetGameObject()->GetObjectID()), _result))
		{
			return { _result.vec[0], _result.vec[1], _result.vec[2] };
		}

		else
		{
			return { 0.0f, 0.0f, 0.0f };
		}
	}

	DirectX::SimpleMath::Vector3 PhysicsManager::GetLinearVelocity(Collider* collider)
	{
		MoolyEngine::Vector3 _result;

		if (physicsEngine->GetLinearVelocity(std::to_string(collider->GetGameObject()->GetObjectID()), _result))
		{
			return { _result.vec[0], _result.vec[1], _result.vec[2] };
		}

		else
		{
			return { 0.0f, 0.0f, 0.0f };
		}
	}

	void PhysicsManager::Simulate(float deltaTime)
	{
		if (deltaTime >= fixedTimeStep)
		{
			for (auto& collider : colliders)
			{
				SetTransform(collider.first, collider.second);
			}

			physicsEngine->Simulate(deltaTime);

			for (auto& collider : colliders)
			{
				UpdateTransform(collider.first);
			}

			colliders.clear();
		}
	}

	void PhysicsManager::Release()
	{
		// TODO 그런건없다
	}

	void PhysicsManager::CollidersClear()
	{
		colliders.clear();
	}


}
