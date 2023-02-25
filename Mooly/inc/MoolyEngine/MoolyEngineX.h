#pragma once
#include "Export/IMoolyEngine.h"
#define PVD_HOST "127.0.0.1"

namespace MoolyEngine
{
	class Scene;

	class MoolyEngineX : public IMoolyEngine, public physx::PxDeletionListener
	{
	public:
		MoolyEngineX();
		~MoolyEngineX() override;

		bool Initialize() override;
		bool Finalaize() override;

		// Scene
		virtual bool AddScene(const std::string& sceneName) override;
		virtual bool SetCurrentScene(const std::string& sceneName) override;
		virtual bool DeleteScene(const std::string& sceneName) override;
		virtual bool ClearScene(const std::string& sceneName) override;

		// Actor Manage
		virtual bool DisableActor(const std::string& objName) override;
		virtual bool EnableActor(const std::string& objName) override;

		virtual bool CreateBoxActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, Vector3 halfExtend, float density) override;
		virtual bool CreateSphereActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, float radius, float density) override;
		virtual bool CreatePlainActor(const std::string& objName, const std::string& shapeName) override;
		virtual bool CreateCapsuleActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, float radius, float halfHeight, float density) override;
		virtual bool CreateTriangleMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCount, UINT32* indices, float indexCount, PhysicsType physicsType, Transform transform, float density) override;
		virtual bool CreateConvexMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCnt, UINT32* indices, float indexCnt, PhysicsType phsicsType, Transform transform, float density) override;

		virtual bool CreatePlayerController(Vector3 pos = Vector3(0.0f, 5.0f, 0.0f)) override;

		virtual bool DeleteActor(const std::string& objName) override;

		// Actor Trigger control
		virtual bool SetTriggerShape(const std::string& objName, bool flag) override;

		// Actor Collider size change
		virtual bool SetCollider(const std::string& objName, Vector3 halfExtend, float mass) override;

		// Object control
		virtual bool AddForce(const std::string& objName, Vector3 vec) override;
		virtual bool AddImpulse(const std::string& objName, Vector3 force) override;
		virtual bool AddTorque(const std::string& objName, Vector3 force) override;
		virtual bool AddTorqueImpulse(const std::string& objName, Vector3 force) override;
		virtual bool SetLinearDamping(const std::string& objName, float damping) override;
		virtual bool SetAngularDamping(const std::string& objName, float damping) override;
		virtual bool SetLinearVelocity(const std::string& objName, Vector3 vel) override;
		virtual bool SetAngularVelocity(const std::string& objName, Vector3 vel) override;

		virtual bool MovePCC(const Vector3& disp) override;

		virtual bool MoveKinematicObject(const std::string& objName, Vector3 position) override;

		virtual bool SetFriction(const std::string& objName, float value, PhysicsType physicsType) override;

		virtual bool SetMaxLinearVelocity(const std::string& objName, float vel) override;
		virtual bool SetMaxAngularVelocity(const std::string& objName, float vel) override;

		virtual bool GetAngularVelocity(const std::string& objName, Vector3& output) override;
		virtual bool GetLinearVelocity(const std::string& objName, Vector3& output) override;

		virtual bool SetTransform(const std::string& objName, Transform transform) override;
		virtual bool SetGravity(const std::string& objName, bool flag) override;
		virtual bool SetKinematic(const std::string& objName, bool flag) override;
		virtual bool SetMass(const std::string& objName, float mass) override;
		virtual bool SetAxisLock(const std::string& objName, PhysicsAxisLock lockInfo) override;

		virtual bool Raycast(Vector3 origin, Vector3 dir, float dist, RayCastHit& hit) override;
		virtual bool Raycast(Vector3 origin, Vector3 dir, float distance, std::vector<std::string> filteredLayers, PhysicsType PhysicsFlags, RayCastHit& hit) override;

		virtual bool CheckBox(Vector3 center, Vector3 halfExtents, std::vector<std::string> filteredLayers) override;

		// Update
		virtual bool Simulate(float deltaTime) override;

		virtual bool GetTransform(const std::string& objName, Transform& outputTransform) override;


		// Layer Max = 31
		virtual bool AddLayer(const std::string& layerName) override;

		virtual bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> FilteredLayers) override;
		virtual bool SetLayer(const std::string& objName, const std::string& layerName) override;

		virtual bool AddColliderEvent(std::string eventHolder, IEventCollider* callbackClass) override;
		virtual bool AddTriggerEvent(std::string eventHolder, IEventTrigger* callbackClass) override;

		virtual bool Test() override;

	private:
		virtual void onRelease(const physx::PxBase* observed, void* userData, physx::PxDeletionEventFlag::Enum deletionEvent);

		static physx::PxDefaultErrorCallback defaultErrorCallback;
		static physx::PxDefaultAllocator defaultAllocatorCallback;

		physx::PxFoundation* foundation;
		physx::PxPhysics* physics;
		physx::PxTolerancesScale toleranceScale;
		physx::PxDefaultCpuDispatcher* cpuDispatcher;
		physx::PxCooking* cooking;
		physx::PxPvd* pvd;
		physx::PxPvdTransport* pvdTransport;

		std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
		std::shared_ptr<Scene> currentScene;
	};
}
