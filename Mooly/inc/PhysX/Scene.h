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

		// Trigger control
		bool SetTriggerShape(const std::string& objName, bool flag);

		bool ArrangeBoxActor(physx::PxPhysics* physics, const std::string& objName, Vector3 halfExtend, float density);
		bool ArrangePlainActor(physx::PxPhysics* physics, const std::string& objName);
		bool ArrangeSphereActor(physx::PxPhysics* physics, const std::string& objName, float radius, float density);

		//bool ArrangeCapsuleActor(physx::PxPhysics* physics, const std::string& objName);
		//bool ArrangeMeshShape(physx::PxPhysics* physics, const std::string& objName, const std::string& shapeName);

		//삭제
		bool DeleteActor(const std::string& objName);
		//비활성화, 물리적인 판정에서 제외됩니다.
		bool DisableActor(const std::string& objName);
		bool EnableActor(const std::string& objName);

		//Actor 제어
		// 물리관련
		bool ClearAllForce(const std::string& objName);

		bool AddForce(const std::string& objName, physx::PxVec3 force);
		bool AddImpulse(const std::string& objName, physx::PxVec3 force);
		bool AddTorque(const std::string& objName, physx::PxVec3 force);
		bool AddTorqueImpulse(const std::string& objName, physx::PxVec3 force);

		//제동 , drag(항력) 프로퍼티
		bool SetLinearDamping(const std::string& objName, float damping);
		bool SetAngularDamping(const std::string& objName, float damping);

		//
		bool SetLinearVelocity(const std::string& objName, physx::PxVec3 vel);
		bool SetAnguarVelocity(const std::string& objName, physx::PxVec3 vel);

		bool SetMaxLinearVelocity(const std::string& objName, physx::PxReal vel);
		bool SetMaxAngularVelocity(const std::string& objName, physx::PxReal vel);

		bool SetKinematic(const std::string& objName, bool flag);
		bool SetGravity(const std::string& objName, bool flag);
		bool SetAxisLock(const std::string& objName, PhysicsAxisLock lockInfo);
		bool SetTransform(const std::string& objName, physx::PxTransform transform);
		bool SetMass(const std::string& objName, float mass);

		bool GetLinearVelocity(const std::string& objName, physx::PxVec3& output);
		bool GetAngularVelocity(const std::string& objName, physx::PxVec3& output);

		bool Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, RayCastHit& hit);
		bool Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, std::vector<std::string> filteredLayers, PhysicsType physicsFlags, RayCastHit& hit);

		bool GetTransform(const std::string& objName, physx::PxTransform& transform);

		//이벤트 관련
		bool AddColliderEvent(std::string eventHolder, IEventCollider* callbackclass);
		bool AddTriggerEvent(std::string eventHolder, IEventTrigger* callbackclass);

		//충돌레이어 지정을 위한 레이어 최대 31개까지 추가가능
		bool AddLayer(const std::string& layerName);
		//FilteredLayer에 있는 이름과 같은 레이어는 충돌이 무시된다. 비용이 큼. 엔진 초기화에 추가하고, 마스킹 세팅할것. 
		//중간수정은 되나, 기존에 있던 오브젝트들에게 적용이 안될 수 있음. 버그발생 확률 매우높아짐
		bool SetLayerFilterData(const std::string& layerName, std::vector<std::string> Filteredlayers);
		//오브젝트의 레이어를 설정한다. 오브젝트들의 defaultSetting은 모두 default이다.
		bool SetLayer(const std::string& objName, const std::string& layerName);

		void Test(physx::PxPhysics* physics);



	private:
		physx::PxScene* m_scene;

		//오브젝트 이름
		std::unordered_map<std::string, physx::PxRigidActor*> m_rigidActors;

		std::unordered_map<std::string, physx::PxRigidActor*> m_ContactEvents;
		std::unordered_map<std::string, physx::PxRigidActor*> m_TriggerEvents;

		//자기자신의 레이어이름, <필터ID, 필터마스크>
		std::unordered_map<std::string, std::pair<physx::PxU32, physx::PxU32>> m_layer;

		physx::PxMaterial* m_material;
		std::shared_ptr<EventCallback> m_eventCallback;
		//EventCallback* m_eventCallback;

		//쉐이프
		//std::unordered_map<std::string, physx::PxShape*> m_Shapes;
	};

	physx::PxFilterFlags PhysicsWorldFilterShader
	(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);
}
