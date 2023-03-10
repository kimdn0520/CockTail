#pragma once

namespace MoolyEngine
{
	class EventCallback;

	class Scene : public std::enable_shared_from_this<Scene>
	{

	public:
		Scene() = default;
		~Scene() = default;

		bool Initialize(physx::PxPhysics* _physics, physx::PxCpuDispatcher* _dispatcher, physx::PxTolerancesScale& _scale);
		bool ClearScene();
		bool Finalize();

		using SCENE_LOCK = physx::PxSceneWriteLock;

		// Update
		bool Simulate(float deltaTime);

		// Create Actor
		bool CreateBoxActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform _transform, Vector3 halfExtend, float density);
		bool CreatePlainActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName);
		bool CreateSphereActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform _transform, float radius, float density);
		bool CreateCapsuleActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform _transform, float halfHeight, float radius, float density);
		//bool CreateMeshActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName);
		bool CreateTriangleMeshActor(physx::PxPhysics* _physics, physx::PxCooking* cooking, physx::PxVec3* vertices, float vertexCount, physx::PxU32* indices, float indexCounter, const std::string& objName, const std::string& shapeName, PhysicsType phsicsType, Transform _transform, float density);
		bool CreateConvexMeshActor(physx::PxPhysics* physics, physx::PxCooking* cooking, physx::PxVec3* vertices, float vertexCount, physx::PxU32* indices, float indexCounter, const std::string& objName, const std::string& shapeName, PhysicsType phsicsType, Transform _transform, float density);

		bool CreatePlayerController(physx::PxVec3 pos);

		// Trigger control
		bool SetTriggerShape(const std::string& objName, bool flag);

		bool ArrangeBoxActor(physx::PxPhysics* physics, const std::string& objName, Vector3 halfExtend, float density);
		bool ArrangePlainActor(physx::PxPhysics* physics, const std::string& objName);
		bool ArrangeSphereActor(physx::PxPhysics* physics, const std::string& objName, float radius, float density);

		//bool ArrangeCapsuleActor(physx::PxPhysics* physics, const std::string& objName);
		//bool ArrangeMeshShape(physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName);

		//????
		bool DeleteActor(const std::string& objName);
		//????????, ???????? ???????? ??????????.
		bool DisableActor(const std::string& objName);
		bool EnableActor(const std::string& objName);

		//Actor ????
		// ????????
		bool ClearAllForce(const std::string& objName);

		bool AddForce(const std::string& objName, physx::PxVec3 force);
		bool AddImpulse(const std::string& objName, physx::PxVec3 force);
		bool AddTorque(const std::string& objName, physx::PxVec3 force);
		bool AddTorqueImpulse(const std::string& objName, physx::PxVec3 force);

		bool MoveKinematicObject(const std::string& objName, physx::PxVec3 position);

		// PCC ????
		bool MovePCC(const physx::PxVec3& disp);
		//bool RotatePCC(const physx::PxVec3& )

		//???? , drag(????) ????????
		bool SetLinearDamping(const std::string& objName, float damping);
		bool SetAngularDamping(const std::string& objName, float damping);

		//
		bool SetLinearVelocity(const std::string& objName, physx::PxVec3 vel);
		bool SetAnguarVelocity(const std::string& objName, physx::PxVec3 vel);

		bool SetMaxLinearVelocity(const std::string& objName, physx::PxReal vel);
		bool SetMaxAngularVelocity(const std::string& objName, physx::PxReal vel);

		bool SetFriction(const std::string& objName, float value, PhysicsType physicsType);

		bool SetKinematic(const std::string& objName, bool flag);
		bool SetGravity(const std::string& objName, bool flag);
		bool SetAxisLock(const std::string& objName, PhysicsAxisLock lockInfo);
		bool SetTransform(const std::string& objName, physx::PxTransform transform);
		bool SetMass(const std::string& objName, float mass);

		bool SetFilterData(const std::string& objName, physx::PxU32 word0, physx::PxU32 word1);

		bool GetLinearVelocity(const std::string& objName, physx::PxVec3& output);
		bool GetAngularVelocity(const std::string& objName, physx::PxVec3& output);

		bool Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, RayCastHit& hit);
		bool Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, std::vector<std::string> filteredLayers, PhysicsType physicsFlags, RayCastHit& hit);

		bool CheckBox(physx::PxVec3 center, physx::PxVec3 halfExtents, std::vector<std::string> filteredLayers);

		bool GetTransform(const std::string& objName, physx::PxTransform& transform);

		//?????? ????
		bool AddColliderEvent(std::string eventHolder, IEventCollider* callbackclass);
		bool AddTriggerEvent(std::string eventHolder, IEventTrigger* callbackclass);

		//?????????? ?????? ???? ?????? ???? 31?????? ????????
		bool AddLayer(const std::string& layerName);
		//FilteredLayer?? ???? ?????? ???? ???????? ?????? ????????. ?????? ??. ???? ???????? ????????, ?????? ????????. 
		//?????????? ????, ?????? ???? ?????????????? ?????? ???? ?? ????. ???????? ???? ??????????
		bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers);
		//?????????? ???????? ????????. ???????????? defaultSetting?? ???? default????.
		bool SetLayer(const std::string& objName, const std::string& layerName);

		void Test(physx::PxPhysics* physics);



	private:
		physx::PxScene* m_scene;

		//???????? ????
		std::unordered_map<std::string, physx::PxRigidActor*> m_rigidActors;

		std::unordered_map<std::string, physx::PxRigidActor*> m_ContactEvents;
		std::unordered_map<std::string, physx::PxRigidActor*> m_TriggerEvents;

		//?????????? ??????????, <????ID, ??????????>
		std::unordered_map<std::string, std::pair<physx::PxU32, physx::PxU32>> m_layer;

		physx::PxMaterial* m_material;
		std::shared_ptr<EventCallback> m_eventCallback;
		//EventCallback* m_eventCallback;

		//??????
		//std::unordered_map<std::string, physx::PxShape*> m_Shapes;
		physx::PxControllerManager* controllerManager;
		physx::PxController* controller;
	};

	physx::PxFilterFlags PhysicsWorldFilterShader
	(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);
}
