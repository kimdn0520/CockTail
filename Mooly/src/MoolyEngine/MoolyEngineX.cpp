#include "pch.h"
#include "MoolyEngine/MoolyEngineX.h"
#include "PhysX/Scene.h"

MoolyEngine::MoolyEngineX::MoolyEngineX()
{

}

MoolyEngine::MoolyEngineX::~MoolyEngineX()
{

}

bool MoolyEngine::MoolyEngineX::Initialize()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocatorCallback, defaultErrorCallback);
	if (foundation)
		PX_ASSERT(false);

	bool _recordMemoryAlloc = true;
	toleranceScale.length = 1.0f;
	toleranceScale.speed = 10.0f;

#if defined(DEBUG) || defined(_DEBUG)
	pvd = PxCreatePvd(*foundation);
	pvdTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	pvd->connect(*pvdTransport, physx::PxPvdInstrumentationFlag::eALL);
#else
	pvd = NULL;
#endif

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, toleranceScale, true, pvd);
	cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);

	if (!physics)
	{
		assert(false);
		return false;
	}

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(toleranceScale));
	return true;
}

bool MoolyEngine::MoolyEngineX::Finalaize()
{
	for (auto _scene : scenes)
	{
		_scene.second->Finalize();
		_scene.second.reset();
	}

	currentScene.reset();
	cooking->release();

#if defined(DEBUG) || defined(_DEBUG)
	pvd->disconnect();
	pvdTransport->disconnect();
	pvdTransport->release();
#endif

	cpuDispatcher->release();
	physics->release();

#if defined(DEBUG) || defined(_DEBUG)
	pvd->release();
#endif

	foundation->release();
	return true;
}

bool MoolyEngine::MoolyEngineX::AddScene(const std::string& sceneName)
{
	auto _foundedScene = scenes.find(sceneName);
	if (_foundedScene == scenes.end())
	{
		auto _tempScene = std::make_shared<Scene>();
		_tempScene->Initialize(physics, cpuDispatcher, toleranceScale);
		scenes.emplace(sceneName, _tempScene);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::SetCurrentScene(const std::string& sceneName)
{
	auto _foundedScene = scenes.find(sceneName);
	if (_foundedScene == scenes.end())
		return false;


	currentScene = _foundedScene->second;
	return true;
}

bool MoolyEngine::MoolyEngineX::DeleteScene(const std::string& sceneName)
{
	auto _foundedScene = scenes.find(sceneName);
	if (_foundedScene == scenes.end())
		return false;

	_foundedScene->second->Finalize();
	scenes.erase(sceneName);
	return true;
}

bool MoolyEngine::MoolyEngineX::ClearScene(const std::string& sceneName)
{
	auto _foundedScene = scenes.find(sceneName);
	if (_foundedScene == scenes.end())
		return false;

	_foundedScene->second->Finalize();
	return true;
}

bool MoolyEngine::MoolyEngineX::DisableActor(const std::string& objName)
{
	if (currentScene)
		return currentScene->DisableActor(objName);

	return false;
}

bool MoolyEngine::MoolyEngineX::EnableActor(const std::string& objName)
{
	if (currentScene)
		return currentScene->EnableActor(objName);

	return false;
}

bool MoolyEngine::MoolyEngineX::CreateBoxActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, Vector3 halfExtend, float density)
{
	if (currentScene)
	{
		currentScene->CreateBoxActor(physics, objName, shapeName, physicsType, transform, halfExtend, density);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::CreateSphereActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, float radius, float density)
{
	if (currentScene)
	{
		currentScene->CreateSphereActor(physics, objName, shapeName, physicsType, transform, radius, density);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::CreatePlainActor(const std::string& objName, const std::string& shapeName)
{
	if (currentScene)
	{
		currentScene->CreatePlainActor(physics, objName, shapeName);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::CreateCapsuleActor(const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform transform, float radius, float halfHeight, float density)
{
	if (currentScene)
	{
		currentScene->CreateCapsuleActor(physics, objName, shapeName, physicsType, transform, halfHeight, radius, density);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::CreateTriangleMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCount, UINT32* indices, float indexCount, PhysicsType physicsType, Transform transform, float density)
{
	if (currentScene)
	{
		currentScene->CreateTriangleMeshActor(physics, cooking, reinterpret_cast<physx::PxVec3*>(vertices), vertexCount, indices, indexCount, objName, shapeName, physicsType, transform, density);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::CreateConvexMeshActor(const std::string& objName, const std::string& shapeName, Vector3* vertices, float vertexCount, UINT32* indices, float indexCount, PhysicsType phsicsType, Transform transform, float density)
{
	if (currentScene)
	{
		currentScene->CreateConvexMeshActor(physics, cooking, reinterpret_cast<physx::PxVec3*>(vertices), vertexCount, indices, indexCount, objName, shapeName, phsicsType, transform, density);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::CreatePlayerController(Vector3 pos /*= Vector3(0.0f, 5.0f, 0.0f)*/)
{
	if (currentScene)
		return currentScene->CreatePlayerController(Vector3ToPxVec3(pos));
}

bool MoolyEngine::MoolyEngineX::DeleteActor(const std::string& objName)
{
	if (currentScene)
		return currentScene->DeleteActor(objName);

	return false;
}

bool MoolyEngine::MoolyEngineX::SetTriggerShape(const std::string& objName, bool flag)
{
	if (currentScene)
		return currentScene->SetTriggerShape(objName, flag);

	return false;
}

bool MoolyEngine::MoolyEngineX::SetCollider(const std::string& objName, Vector3 halfExtend, float mass)
{
	return false;
}

bool MoolyEngine::MoolyEngineX::AddForce(const std::string& objName, Vector3 force)
{
	if (currentScene)
		return currentScene->AddForce(objName, Vector3ToPxVec3(force));

	return false;
}

bool MoolyEngine::MoolyEngineX::AddImpulse(const std::string& objName, Vector3 force)
{
	if (currentScene)
		return currentScene->AddImpulse(objName, Vector3ToPxVec3(force));

	return false;
}

bool MoolyEngine::MoolyEngineX::AddTorque(const std::string& objName, Vector3 force)
{
	if (currentScene)
		return currentScene->AddTorque(objName, Vector3ToPxVec3(force));

	return false;
}

bool MoolyEngine::MoolyEngineX::AddTorqueImpulse(const std::string& objName, Vector3 force)
{
	if (currentScene)
		return currentScene->AddTorqueImpulse(objName, Vector3ToPxVec3(force));

	return false;
}

bool MoolyEngine::MoolyEngineX::SetLinearDamping(const std::string& objName, float damping)
{
	if (currentScene)
		return currentScene->SetLinearDamping(objName, damping);

	return false;
}

bool MoolyEngine::MoolyEngineX::SetAngularDamping(const std::string& objName, float damping)
{
	if (currentScene)
		return currentScene->SetAngularDamping(objName, damping);

	return false;
}

bool MoolyEngine::MoolyEngineX::SetLinearVelocity(const std::string& objName, Vector3 vel)
{
	if (currentScene)
		return currentScene->SetLinearVelocity(objName, Vector3ToPxVec3(vel));

	return false;
}

bool MoolyEngine::MoolyEngineX::SetAngularVelocity(const std::string& objName, Vector3 vel)
{
	if (currentScene)
		return currentScene->SetAnguarVelocity(objName, Vector3ToPxVec3(vel));

	return false;
}

bool MoolyEngine::MoolyEngineX::MovePCC(const Vector3& disp)
{
	if (currentScene)
		return currentScene->MovePCC(Vector3ToPxVec3(disp));

	return false;
}

bool MoolyEngine::MoolyEngineX::MoveKinematicObject(const std::string& objName, Vector3 position)
{
	if (currentScene)
		return currentScene->MoveKinematicObject(objName, Vector3ToPxVec3(position));

	return false;
}

bool MoolyEngine::MoolyEngineX::SetFriction(const std::string& objName, float value, PhysicsType physicsType)
{
	return false;
}

bool MoolyEngine::MoolyEngineX::SetMaxLinearVelocity(const std::string& objName, float vel)
{
	if (currentScene)
		return currentScene->SetMaxLinearVelocity(objName, vel);

	return false;
}

bool MoolyEngine::MoolyEngineX::SetMaxAngularVelocity(const std::string& objName, float vel)
{
	if (currentScene)
		return currentScene->SetMaxAngularVelocity(objName, vel);

	return false;
}

bool MoolyEngine::MoolyEngineX::GetAngularVelocity(const std::string& objName, Vector3& output)
{
	if (currentScene)
	{
		physx::PxVec3 _ret;
		bool _result = currentScene->GetAngularVelocity(objName, _ret);
		output.vec[0] = _ret.x;
		output.vec[1] = _ret.y;
		output.vec[2] = _ret.z;

		return _result;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::GetLinearVelocity(const std::string& objName, Vector3& output)
{
	if (currentScene)
	{
		physx::PxVec3 _ret;

		bool _result = currentScene->GetLinearVelocity(objName, _ret);
		output.vec[0] = _ret.x;
		output.vec[1] = _ret.y;
		output.vec[2] = _ret.z;

		return _result;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::SetTransform(const std::string& objName, Transform transform)
{
	if (currentScene)
		return currentScene->SetTransform(objName, TransformToPxTransform(transform));

	return false;
}

bool MoolyEngine::MoolyEngineX::SetGravity(const std::string& objName, bool flag)
{
	if (currentScene)
		return currentScene->SetGravity(objName, flag);

	return false;
}

bool MoolyEngine::MoolyEngineX::SetKinematic(const std::string& objName, bool flag)
{
	if (currentScene)
		return currentScene->SetKinematic(objName, flag);

	return false;
}

bool MoolyEngine::MoolyEngineX::SetMass(const std::string& objName, float mass)
{
	if (currentScene)
		return currentScene->SetMass(objName, mass);

	return false;
}

bool MoolyEngine::MoolyEngineX::SetAxisLock(const std::string& objName, PhysicsAxisLock lockInfo)
{
	if (currentScene)
		return currentScene->SetAxisLock(objName, lockInfo);

	return false;
}

bool MoolyEngine::MoolyEngineX::Raycast(Vector3 origin, Vector3 dir, float dist, RayCastHit& hit)
{
	if (currentScene->Raycast(Vector3ToPxVec3(origin), Vector3ToPxVec3(dir), dist, hit))
		return true;

	return false;
}


bool MoolyEngine::MoolyEngineX::Raycast(Vector3 origin, Vector3 dir, float distance, std::vector<std::string> filteredLayers, PhysicsType PhysicsFlags, RayCastHit& hit)
{
	if (currentScene->Raycast(Vector3ToPxVec3(origin), Vector3ToPxVec3(dir), distance, filteredLayers, PhysicsFlags, hit))
		return true;

	return false;
}

bool MoolyEngine::MoolyEngineX::CheckBox(Vector3 center, Vector3 halfExtents, std::vector<std::string> filteredLayers)
{
	if(currentScene->CheckBox(Vector3ToPxVec3(center), Vector3ToPxVec3(halfExtents), filteredLayers))
		return true;

	return false;
}

bool MoolyEngine::MoolyEngineX::Simulate(float deltaTime)
{
	if (currentScene)
	{
		currentScene->Simulate(deltaTime);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::GetTransform(const std::string& objName, Transform& outputTransform)
{
	physx::PxTransform _tempTransform;
	if (currentScene->GetTransform(objName, _tempTransform))
	{
		outputTransform = PxTransformToTransform(_tempTransform);
		return true;
	}

	return false;
}

bool MoolyEngine::MoolyEngineX::AddLayer(const std::string& layerName)
{
	if (currentScene->AddLayer(layerName))
		return true;

	return false;
}

bool MoolyEngine::MoolyEngineX::SetLayerFilterData(const std::string& layerName, std::vector<std::string> filteredLayers)
{
	if (currentScene->SetLayerFilterData(layerName, filteredLayers))
		return true;

	return false;
}

bool MoolyEngine::MoolyEngineX::SetLayer(const std::string& objName, const std::string& layerName)
{
	if (currentScene->SetLayer(objName, layerName))
		return true;

	return false;
}

bool MoolyEngine::MoolyEngineX::AddColliderEvent(std::string eventHolder, IEventCollider* callbackClass)
{
	if (currentScene->AddColliderEvent(eventHolder, callbackClass))
		return true;

	return false;
}

bool MoolyEngine::MoolyEngineX::AddTriggerEvent(std::string eventHolder, IEventTrigger* callbackClass)
{
	if (currentScene->AddTriggerEvent(eventHolder, callbackClass))
		return true;

	return false;
}

bool MoolyEngine::MoolyEngineX::Test()
{
	currentScene->Test(physics);

	return true;
}

void MoolyEngine::MoolyEngineX::onRelease(const physx::PxBase* observed, void* userData, physx::PxDeletionEventFlag::Enum deletionEvent)
{
	PX_UNUSED(userData);
	PX_UNUSED(deletionEvent);
}

physx::PxDefaultErrorCallback MoolyEngine::MoolyEngineX::defaultErrorCallback;
physx::PxDefaultAllocator MoolyEngine::MoolyEngineX::defaultAllocatorCallback;
