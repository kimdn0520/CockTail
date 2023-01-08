#pragma once
#include <memory>

#include "MoolyInfo.h"

namespace MoolyEngine
{
	class MOOLY_ENGINE IMoolyEngine
	{
	protected:
		IMoolyEngine() = default;
		virtual ~IMoolyEngine() = default;

	public:
		virtual bool Initialize() abstract;
		virtual bool Finalaize() abstract;
		
		// Scene
		virtual bool AddScene(const std::string& sceneName) abstract;
		virtual bool SetCurrentScene(const std::string& sceneName) abstract;
		virtual bool DeleteScene(const std::string& sceneName) abstract;

		// Actor Manage
		virtual bool DisableActor(const std::string& objName) abstract;
		virtual bool EnableActor(const std::string& objName) abstract;
		
		// Create Actor
		virtual bool CreateBoxActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, Vector3 halfExtend, float density) abstract;
		virtual bool CreateSphereActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, float radius, float density) abstract;
		virtual bool CreatePlainActor(const std::string& objName, const std::string& shapeName) abstract;
		virtual bool CreateCapsuleActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, float radius, float halfHeight, float density) abstract;
		virtual bool CreateTriangleMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCount, UINT32* indices, float indexCount, PhysicsType physicsType, Transform transform, float density) abstract;
		virtual bool CreateConvexMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, PhysicsType phsicsType, Transform transform, float density) abstract;


		virtual bool DeleteActor(const std::string& objName) abstract;

		// Actor Trigger control
		virtual bool SetTriggerShape(const std::string& objName, bool flag) abstract;

		// Actor Collider size change
		virtual bool SetCollider(const std::string& objName, Vector3 halfExtend, float mass) abstract;

		// Object control
		virtual bool AddForce(const std::string& objName, Vector3 vec) abstract;
		virtual bool AddImpulse(const std::string& objName, Vector3 force) abstract;
		virtual bool AddTorque(const std::string& objName, Vector3 force) abstract;
		virtual bool AddTorqueImpulse(const std::string& objName, Vector3 force) abstract;
		virtual bool SetLinearDamping(const std::string& objName, float damping) abstract;
		virtual bool SetAngularDamping(const std::string& objName, float damping) abstract;
		virtual bool SetLinearVelocity(const std::string& objName, Vector3 vel) abstract;
		virtual bool SetAngularVelocity(const std::string& objName, Vector3 vel) abstract;

		virtual bool SetMaxLinearVelocity(const std::string& objName, float vel) abstract;
		virtual bool SetMaxAngularVelocity(const std::string& objName, float vel) abstract;

		virtual bool GetAngularVelocity(const std::string& objName, Vector3& output) abstract;
		virtual bool GetLinearVelocity(const std::string& objName, Vector3& output) abstract;

		virtual bool SetTransform(const std::string& objName, Transform transform) abstract;
		virtual bool SetGravity(const std::string& objName, bool flag) abstract;
		virtual bool SetKinematic(const std::string& objName, bool flag) abstract;
		virtual bool SetMass(const std::string& objName, float mass) abstract;
		virtual bool SetAxisLock(const std::string& objName, PhysicsAxisLock lockInfo) abstract;

		virtual bool Raycast(Vector3 origin, Vector3 dir, float dist, RayCastHit& hit) abstract;
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, std::vector<std::string> filteredLayers, PhysicsType PhysicsFlags, RayCastHit& hit) abstract;

		// Update
		virtual bool Simulate(float deltaTime) abstract;

		virtual bool GetTransform(const std::string& objName, Transform& outputTransform) abstract;


		// Layer Max = 31
		virtual bool AddLayer(const std::string& layerName) abstract;

		virtual bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> FilteredLayers) abstract;
		virtual bool SetLayer(const std::string& objName, const std::string& layerName) abstract;

		virtual bool AddColliderEvent(std:: string eventHolder, IEventCollider*) abstract;
		virtual bool AddTriggerEvent(std:: string eventHolder, IEventTrigger*) abstract;

		virtual bool Test() abstract;
	};

	static std::shared_ptr<MoolyEngine::IMoolyEngine> g_MoolyEngine = nullptr;

	MOOLY_ENGINE std::shared_ptr<IMoolyEngine> MoolyEngineInstance();
	MOOLY_ENGINE void MoolyEngineRelease();
}
