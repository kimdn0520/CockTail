#pragma once
#include "Object/ComponentBase.h"
#include "Define/FootEngineTypedef.h"
#include "MoolyInfo.h"

namespace GameEngineSpace
{
	class Collider;

	class Rigidbody : public ComponentBase
	{
	private:
		std::shared_ptr<Collider> collider;
		MoolyEngine::PhysicsAxisLock physAxisLock;
		float mass;
		float damp;
		float angularDamp;
		bool bGravity;
		bool bKinematic;

	public:
		Rigidbody(std::weak_ptr<GameObject> _object);
		virtual ~Rigidbody() {}

		void Awake() override;

		void AddForce(MoolyEngine::Vector3 force);
		void AddImpulse(MoolyEngine::Vector3 impulse);

		void MoveKinematic(MoolyEngine::Vector3 position);

		void SetMass(float mass);
		void SetAngularDamp(float damp);
		void SetLinearDamp(float damp);
		
		void SetMaxAngularVelocity(float veloicty);
		void SetMaxLinearVelocity(float velocity);
		void SetAxisLock(MoolyEngine::PhysicsAxisLock axisLock);
		void SetPhysAxisLock(bool _lx, bool _ly, bool _lz, bool _ax, bool _ay, bool _az);

		void SetLinearVelocity(Vector3 velocity);
		void SetAngularVelocity(Vector3 angular);

		void SetMaxAngularDampVelocity(float velocity);
		void SetMaxLinearDampVelocity(float velocity);

		void SetGravity(bool value);
		void SetKinematic(bool value);

		Vector3 GetLinearVelocity();

		void UpdateRigidbody();

		void InitRigidBody();
	};
}