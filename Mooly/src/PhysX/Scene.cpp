#include "pch.h"
#include "physX/Scene.h"
#include "PxSceneLock.h"
#include "PhysX/EventCallback.h"
#include "characterkinematic/PxControllerManager.h"
#include "characterkinematic/PxCapsuleController.h"

bool MoolyEngine::Scene::Initialize(physx::PxPhysics* _physics, physx::PxCpuDispatcher* _dispatcher, physx::PxTolerancesScale& _scale)
{
	physx::PxSceneDesc sceneDesc(_scale);

	sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f);
	sceneDesc.cpuDispatcher = _dispatcher;
	sceneDesc.filterShader = MoolyEngine::PhysicsWorldFilterShader;

	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;

	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;

	m_scene = _physics->createScene(sceneDesc);

	controllerManager = PxCreateControllerManager(*m_scene);

#if defined(DEBUG) || defined(_DEBUG)

	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);

	physx::PxPvdSceneClient* _pvdClient = m_scene->getScenePvdClient();
	if (_pvdClient)
	{
		_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		_pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif

	m_material = _physics->createMaterial(0.5f, 0.5f, 0.1f);

	//m_eventCallback = new MoolyEngine::EventCallback();
	//m_scene->setSimulationEventCallback((physx::PxSimulationEventCallback*)m_eventCallback);

	m_eventCallback = std::make_shared<EventCallback>();
	m_scene->setSimulationEventCallback(m_eventCallback.get());

	physx::PxU32 defaultLayerFilterID = 0x00000001;
	physx::PxU32 defaultLayerFilterMask = 0xFFFFFFFF;

	m_layer.emplace("Default", std::make_pair(defaultLayerFilterID, defaultLayerFilterMask));

	return true;
}

bool MoolyEngine::Scene::ClearScene()
{
	for (auto actors : m_rigidActors)
	{
		if (actors.second->userData != nullptr)
		{
			auto _data = reinterpret_cast<UserData*>(actors.second->userData);
			delete _data;
			actors.second->userData = nullptr;
		}
	}

	m_rigidActors.clear();

	return true;
}

bool MoolyEngine::Scene::Finalize()
{
	for (auto actors : m_rigidActors)
	{
		if (actors.second->userData != nullptr)
		{
			auto _data = reinterpret_cast<UserData*>(actors.second->userData);
			delete _data;
			actors.second->userData = nullptr;
		}
	}

	m_scene->release();
	m_rigidActors.clear();
	if(controllerManager != nullptr)
		controllerManager->purgeControllers();

	return true;
}

bool MoolyEngine::Scene::Simulate(float _deltaTime)
{
	SCENE_LOCK sceneLock(*m_scene);

	if (_deltaTime > 0.f)
	{
		for (auto actors : m_rigidActors)
		{
			if (actors.second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
			{
				((physx::PxRigidDynamic*)actors.second)->setMaxDepenetrationVelocity(3.0f);
			}
		}

		m_scene->simulate(_deltaTime);
		m_scene->fetchResults(true);
		m_eventCallback->CallonTriggerPersist();
	}

	return true;
}

bool MoolyEngine::Scene::CreateBoxActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform _transform, Vector3 halfExtend, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto foundedActor = m_rigidActors.find(objName);
	if (foundedActor != m_rigidActors.end())
		return false;

	physx::PxTransform _pxTransform = TransformToPxTransform(_transform);
	physx::PxShape* _shape = _physics->createShape(physx::PxBoxGeometry(halfExtend.vec[0], halfExtend.vec[1], halfExtend.vec[2]), *m_material, true);
	physx::PxRigidActor* _box = nullptr;

	physx::PxFilterData _filterData;
	_filterData.word0 = 0x00000001;
	_filterData.word1 = 0xFFFFFFFF;

	_shape->setSimulationFilterData(_filterData);

	switch (physicsType)
	{
		case PhysicsType::STATIC:
		{
			_box = physx::PxCreateStatic(*_physics, _pxTransform, *_shape);
		}
		break;

		case PhysicsType::DYNAMIC:
		{
			_box = physx::PxCreateDynamic(*_physics, _pxTransform, *_shape, density);
		}
		break;

		default:
		{
			return false;
		}
	}

	m_scene->addActor(*_box);
	UserData* _data = new UserData;
	_data->m_name = objName;
	_box->userData = _data;
	m_rigidActors.emplace(objName, (_box));

	_shape->release();

	return true;
}

bool MoolyEngine::Scene::CreatePlainActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor != m_rigidActors.end());
	return false;

	auto _foundedPlain = m_rigidActors.find("Plain");
	physx::PxMaterial* _material = nullptr;
	if (_foundedPlain == m_rigidActors.end())
		_material = _physics->createMaterial(0.5f, 0.5f, 0.6f);

	physx::PxRigidActor* _plain = physx::PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 0), *_material);

	UserData* _data = new UserData;
	_data->m_name = objName;
	_plain->userData = _data;

	m_rigidActors.emplace(objName, _plain);
	m_scene->addActor(*_plain);

	return true;
}

bool MoolyEngine::Scene::CreateSphereActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform _transform, float radius, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);

	if (_foundedActor != m_rigidActors.end())
		return false;

	physx::PxFilterData _filterData;
	_filterData.word0 = 0x00000001;
	_filterData.word1 = 0xFFFFFFFF;

	physx::PxTransform _pxTransform = TransformToPxTransform(_transform);
	physx::PxShape* _shape = _physics->createShape(physx::PxSphereGeometry(radius), *m_material, true);
	_shape->setSimulationFilterData(_filterData);
	_filterData.word1 = 0x00000000;
	_shape->setQueryFilterData(_filterData);

	physx::PxRigidActor* _sphere = nullptr;

	switch (physicsType)
	{
		case PhysicsType::STATIC:
		{
			_sphere = physx::PxCreateStatic(*_physics, TransformToPxTransform(_transform), *_shape);
		}
		break;

		case PhysicsType::DYNAMIC:
		{
			_sphere = physx::PxCreateDynamic(*_physics, TransformToPxTransform(_transform), *_shape, density);
		}
		break;

		default:
		{
			return false;
		}
	}

	UserData* _data = new UserData;
	_data->m_name = objName;
	_sphere->userData = _data;

	m_scene->addActor(*_sphere);
	m_rigidActors.emplace(objName, (_sphere));

	_shape->release();

	return true;
}

bool MoolyEngine::Scene::CreateCapsuleActor(physx::PxPhysics* _physics, const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform _transform, float halfHeight, float radius, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);

	if (_foundedActor != m_rigidActors.end())
		return false;

	physx::PxFilterData _filterData;
	_filterData.word0 = 0x00000001;
	_filterData.word1 = 0xFFFFFFFF;

	physx::PxTransform _pxTransform = TransformToPxTransform(_transform);
	physx::PxShape* _shape = _physics->createShape(physx::PxCapsuleGeometry(radius, halfHeight), *m_material, true);
	_shape->setSimulationFilterData(_filterData);
	_filterData.word1 = 0x00000000;
	_shape->setQueryFilterData(_filterData);

	physx::PxRigidActor* _capsule = nullptr;

	switch (physicsType)
	{
		case PhysicsType::STATIC:
		{
			_capsule = physx::PxCreateStatic(*_physics, TransformToPxTransform(_transform), *_shape);
		}
		break;

		case PhysicsType::DYNAMIC:
		{
			_capsule = physx::PxCreateDynamic(*_physics, TransformToPxTransform(_transform), *_shape, density);
		}
		break;

		default:
		{
			return false;
		}
	}

	UserData* _data = new UserData;
	_data->m_name = objName;
	_capsule->userData = _data;

	m_scene->addActor(*_capsule);
	m_rigidActors.emplace(objName, (_capsule));

	_shape->release();

	return true;
}

bool MoolyEngine::Scene::CreateTriangleMeshActor(physx::PxPhysics* _physics, physx::PxCooking* cooking, physx::PxVec3* vertices, float vertexCount, physx::PxU32* indices, float indexCount, const std::string& objName, const std::string& shapeName, PhysicsType physicsType, Transform _transform, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	physx::PxTriangleMeshDesc _meshDesc;
	_meshDesc.points.count = vertexCount;
	_meshDesc.points.stride = sizeof(physx::PxVec3);
	_meshDesc.points.data = vertices;

	_meshDesc.triangles.count = indexCount / 3;
	_meshDesc.triangles.stride = sizeof(physx::PxU32) * 3;
	_meshDesc.triangles.data = indices;

	physx::PxDefaultMemoryOutputStream _writeBuffer;
	physx::PxTriangleMeshCookingResult::Enum _result;
	bool _status = cooking->cookTriangleMesh(_meshDesc, _writeBuffer, &_result);

	if (_status == false)
		return NULL;

	physx::PxDefaultMemoryInputData _readBuffer(_writeBuffer.getData(), _writeBuffer.getSize());
	auto _triangleMesh = _physics->createTriangleMesh(_readBuffer);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor != m_rigidActors.end())
		return false;

	physx::PxFilterData _filterData;
	_filterData.word0 = 0x00000001;
	_filterData.word1 = 0xFFFFFFFF;

	physx::PxTransform _pxTransform = TransformToPxTransform(_transform);
	physx::PxShape* _shape = _physics->createShape(physx::PxTriangleMeshGeometry(_triangleMesh), *m_material, true);

	_shape->setSimulationFilterData(_filterData);
	_filterData.word1 = 0x00000000;
	_shape->setQueryFilterData(_filterData);

	physx::PxRigidActor* _actor = nullptr;

	switch (physicsType)
	{
		case PhysicsType::STATIC:
		{
			_actor = physx::PxCreateStatic(*_physics, TransformToPxTransform(_transform), *_shape);
		}
		break;

		case PhysicsType::DYNAMIC:
		{
			_actor = physx::PxCreateKinematic(*_physics, TransformToPxTransform(_transform), *_shape, density);
		}
		break;

		default:
		{
			return false;
		}
	}

	UserData* _data = new UserData;
	_data->m_name = objName;
	_actor->userData = _data;

	m_scene->addActor(*_actor);
	m_rigidActors.emplace(objName, _actor);

	_shape->release();
}

bool MoolyEngine::Scene::CreateConvexMeshActor(physx::PxPhysics* physics, physx::PxCooking* cooking, physx::PxVec3* vertices, float vertexCount, physx::PxU32* indices, float indexCounter, const std::string& objName, const std::string& shapeName, PhysicsType phsycsType, Transform _transform, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor != m_rigidActors.end())
		return false;

	physx::PxConvexMeshDesc _meshDesc;
	_meshDesc.points.count = vertexCount;
	_meshDesc.points.stride = sizeof(physx::PxVec3);
	_meshDesc.points.data = vertices;

	_meshDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
	physx::PxDefaultMemoryOutputStream _writeBuffer;
	physx::PxConvexMeshCookingResult::Enum _result;

	bool _status = cooking->cookConvexMesh(_meshDesc, _writeBuffer, &_result);
	if (_status == false)
		return NULL;

	physx::PxDefaultMemoryInputData _readBuffer(_writeBuffer.getData(), _writeBuffer.getSize());
	physx::PxConvexMesh* _convexMesh = physics->createConvexMesh(_readBuffer);

	physx::PxShape* _convexShape = physics->createShape(physx::PxConvexMeshGeometry(_convexMesh, physx::PxMeshScale({ 0.25f,0.25f,0.25f })), *m_material, true);

	physx::PxFilterData _filterData;
	_filterData.word0 = 0x00000001;
	_filterData.word1 = 0xFFFFFFFF;

	physx::PxTransform _pxTrasnform = TransformToPxTransform(_transform);

	_convexShape->setSimulationFilterData(_filterData);
	_filterData.word1 = 0x00000000;
	_convexShape->setQueryFilterData(_filterData);

	physx::PxRigidActor* _actor = nullptr;

	switch (phsycsType)
	{
		case PhysicsType::STATIC:
		{
			_actor = physx::PxCreateStatic(*physics, TransformToPxTransform(_transform), *_convexShape);
		}
		break;

		case PhysicsType::DYNAMIC:
		{
			_actor = physx::PxCreateDynamic(*physics, TransformToPxTransform(_transform), *_convexShape, density);
		}
		break;

		default:
		{
			return false;
		}
	}

	UserData* _data = new UserData;
	_data->m_name = objName;
	_actor->userData = _data;

	m_scene->addActor(*_actor);
	m_rigidActors.emplace(objName, _actor);

	_convexShape->release();
}

bool MoolyEngine::Scene::CreatePlayerController(physx::PxVec3 pos)
{
	physx::PxCapsuleControllerDesc desc;
	desc.height = 1.0f;
	desc.material = m_material;
	desc.slopeLimit = 0.5f;
	desc.stepOffset = 0.5f;
	desc.upDirection = physx::PxVec3(0.0f, 1.0f, 0.0f);
	physx::PxExtendedVec3 _pos(pos.x, pos.y, pos.z);
	desc.position = _pos;
	controller = controllerManager->createController(desc);

	controllerManager->setOverlapRecoveryModule(true);
	controllerManager->setPreciseSweeps(true);
	return true;
}

bool MoolyEngine::Scene::SetTriggerShape(const std::string& objName, bool flag)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	physx::PxShape** _oldShape = new physx::PxShape*;

	_foundedActor->second->getShapes(_oldShape, 1);

	if (_oldShape != nullptr)
	{
		(*_oldShape)->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !flag);
		(*_oldShape)->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, flag);

		delete _oldShape;
		return true;
	}

	delete _oldShape;
	return false;
}

bool MoolyEngine::Scene::ArrangeBoxActor(physx::PxPhysics* physics, const std::string& objName, Vector3 halfExtend, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	physx::PxShape** _oldShape = nullptr;

	_foundedActor->second->getShapes(_oldShape, 1);
	(*_oldShape)->release();

	physx::PxShape* _shape = physics->createShape(physx::PxBoxGeometry(halfExtend.vec[0], halfExtend.vec[1], halfExtend.vec[2]), *m_material);

	_foundedActor->second->attachShape(*_shape);
	return true;
}


bool MoolyEngine::Scene::ArrangePlainActor(physx::PxPhysics* physics, const std::string& objName)
{
	return false;
}

bool MoolyEngine::Scene::ArrangeSphereActor(physx::PxPhysics* physics, const std::string& objName, float radius, float density)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);

	if (_foundedActor == m_rigidActors.end())
		return false;

	physx::PxShape** _oldShape = nullptr;

	_foundedActor->second->getShapes(_oldShape, 1);
	(*_oldShape)->release();

	physx::PxShape* _shape = physics->createShape(physx::PxSphereGeometry(radius), *m_material);

	_foundedActor->second->attachShape(*_shape);
	return true;

}

bool MoolyEngine::Scene::DeleteActor(const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);

	if (_foundedActor == m_rigidActors.end())
		return false;

	m_scene->removeActor(*(_foundedActor->second));
	auto _data = reinterpret_cast<UserData*>(_foundedActor->second->userData);
	delete _data;

	_foundedActor->second->userData = nullptr;
	_foundedActor->second->release();
	m_rigidActors.erase(objName);

	return true;

}

bool MoolyEngine::Scene::DisableActor(const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	//UINT _numberOfShape = _foundedActor->second->getNbShapes();
	//std::vector<physx::PxShape*> _shapes;
	//_shapes.resize(_numberOfShape);

	//_foundedActor->second->getShapes(&_shapes.front(), _numberOfShape);
	//for (auto _temp : _shapes)
	//{
	//	_temp->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
	//}
	_foundedActor->second->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);

	return true;
}

bool MoolyEngine::Scene::EnableActor(const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	UINT _numberOfShape = _foundedActor->second->getNbShapes();
	std::vector<physx::PxShape*> _shapes;
	_shapes.resize(_numberOfShape);

	_foundedActor->second->getShapes(&_shapes.front(), _numberOfShape);

	_foundedActor->second->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->wakeUp();
	}

	return true;
}

bool MoolyEngine::Scene::ClearAllForce(const std::string& objName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->setLinearVelocity({ 0.f, 0.f, 0.f });
		((physx::PxRigidDynamic*)_foundedActor->second)->setAngularVelocity({ 0.f, 0.f, 0.f });

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::AddForce(const std::string& objName, physx::PxVec3 force)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->addForce(force, physx::PxForceMode::eFORCE, true);
		return true;
	}

	return false;
}

bool MoolyEngine::Scene::AddImpulse(const std::string& objName, physx::PxVec3 force)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->addForce(force, physx::PxForceMode::eIMPULSE, true);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::AddTorque(const std::string& objName, physx::PxVec3 force)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->addTorque(force, physx::PxForceMode::eFORCE, true);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::AddTorqueImpulse(const std::string& objName, physx::PxVec3 force)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->addTorque(force, physx::PxForceMode::eIMPULSE, true);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::MoveKinematicObject(const std::string& objName, physx::PxVec3 position)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		//if (((physx::PxRigidDynamic*)_foundedActor->second)->getRigidBodyFlags() == physx::PxRigidBodyFlag::eKINEMATIC)
		{
			physx::PxTransform transform = _foundedActor->second->getGlobalPose();
			transform.p = position;
			((physx::PxRigidDynamic*)_foundedActor->second)->setKinematicTarget(transform);
			return true;
		}
	}

	return false;
}

bool MoolyEngine::Scene::MovePCC(const physx::PxVec3& disp)
{
	if (controllerManager == nullptr)
		return false;

	if (controller == nullptr)
		return false;

	physx::PxControllerCollisionFlags collisionFlags = controller->move(disp, 0.01f, 1.0f / 60.0f, physx::PxControllerFilters());

}

bool MoolyEngine::Scene::SetLinearDamping(const std::string& objName, float damping)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->setLinearDamping(damping);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::SetAngularDamping(const std::string& objName, float damping)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->setAngularDamping(damping);
		return true;
	}

	return false;

	return false;
}

bool MoolyEngine::Scene::SetLinearVelocity(const std::string& objName, physx::PxVec3 vel)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->setLinearVelocity(vel);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::SetAnguarVelocity(const std::string& objName, physx::PxVec3 vel)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->setAngularVelocity(vel);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::SetMaxLinearVelocity(const std::string& objName, physx::PxReal vel)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->setMaxLinearVelocity(vel);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::SetMaxAngularVelocity(const std::string& objName, physx::PxReal vel)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
	{
		return false;
	}

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->setMaxAngularVelocity(vel);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::SetFriction(const std::string& objName, float value, PhysicsType physicsType)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	/*if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		(physx::PxRigidActor*)_foundedActor->first->setMaterial
	}*/

	return false;
}

bool MoolyEngine::Scene::SetKinematic(const std::string& objName, bool flag)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
	{
		return false;
	}

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		((physx::PxRigidDynamic*)_foundedActor->second)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, flag);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::SetGravity(const std::string& objName, bool flag)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (flag)
	{
		_foundedActor->second->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
	}

	else
	{
		_foundedActor->second->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	}

	return true;

	return false;
}

bool MoolyEngine::Scene::SetAxisLock(const std::string& objName, PhysicsAxisLock lockInfo)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	auto _actor = _foundedActor->second;
	if (_actor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		auto _castedActor = (physx::PxRigidDynamic*)_actor;
		_castedActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, lockInfo.linearX);
		_castedActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, lockInfo.linearY);
		_castedActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, lockInfo.linearZ);

		_castedActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, lockInfo.angularX);
		_castedActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, lockInfo.angularY);
		_castedActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, lockInfo.angularZ);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::SetTransform(const std::string& objName, physx::PxTransform transform)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	_foundedActor->second->setGlobalPose(transform);
	return true;

}

bool MoolyEngine::Scene::SetMass(const std::string& objName, float mass)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		return physx::PxRigidBodyExt::setMassAndUpdateInertia(*((physx::PxRigidDynamic*)_foundedActor->second), mass);

	return false;
}

bool MoolyEngine::Scene::GetLinearVelocity(const std::string& objName, physx::PxVec3& output)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		output = ((physx::PxRigidDynamic*)_foundedActor->second)->getLinearVelocity();
		return true;
	}

	return false;
}

bool MoolyEngine::Scene::GetAngularVelocity(const std::string& objName, physx::PxVec3& output)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);
	if (_foundedActor == m_rigidActors.end())
		return false;

	if (_foundedActor->second->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		output = ((physx::PxRigidDynamic*)_foundedActor->second)->getAngularVelocity();
		return true;
	}

	return false;
}

bool MoolyEngine::Scene::Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, RayCastHit& hit)
{
	SCENE_LOCK sceneLock(*m_scene);

	const physx::PxHitFlags _outputFlags = physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL;

	physx::PxQueryFilterData _filterData;
	_filterData.data.word0 = 1 << 2;	// 임의 플래그 비교
	_filterData.flags = physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC;
	physx::PxQueryFilterCallback* _filterCallback = nullptr;

	physx::PxRaycastBuffer _buffer1;

	bool result = m_scene->raycast(origin, dir, distance, _buffer1, physx::PxHitFlags(physx::PxHitFlag::eDEFAULT), _filterData);

	if (_buffer1.hasBlock)
	{
		if (_buffer1.block.actor->userData == nullptr)
		{
			return false;
		}

		if (_buffer1.block.actor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			hit.objectName = ((UserData*)_buffer1.block.actor->userData)->m_name;
			hit.distance = _buffer1.block.distance;
		}

		hit.normal = PxVec3ToVector3(_buffer1.block.normal);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::Raycast(physx::PxVec3 origin, physx::PxVec3 dir, float distance, std::vector<std::string> filteredLayers, PhysicsType physicsFlags, RayCastHit& hit)
{
	SCENE_LOCK sceneLock(*m_scene);

	const physx::PxHitFlags _outputFlags = physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL;

	physx::PxQueryFilterData _filterData;

	_filterData.data.word0 = 0xFFFFFFFF;

	for (auto _filterLayer : filteredLayers)
	{
		auto _foundedLayer = m_layer.find(_filterLayer);
		if (_foundedLayer == m_layer.end())
		{
			continue;
		}

		_filterData.data.word0 -= _foundedLayer->second.first;
	}

	switch (physicsFlags)
	{
		case MoolyEngine::PhysicsType::STATIC:
		{
			_filterData.flags = physx::PxQueryFlag::eSTATIC;
		}
		break;

		case MoolyEngine::PhysicsType::DYNAMIC:
		{
			_filterData.flags = physx::PxQueryFlag::eDYNAMIC;
		}
		break;

		default:
			break;
	}

	physx::PxRaycastBuffer _buf01;

	bool _result = m_scene->raycast(origin, dir, distance, _buf01, _outputFlags, _filterData);

	if (_buf01.hasBlock)
	{
		if (_buf01.block.actor->userData == nullptr)
			return false;

		auto _type = _buf01.block.actor->getType();

		hit.objectName = ((UserData*)_buf01.block.actor->userData)->m_name;
		hit.distance = _buf01.block.distance;
		hit.normal = PxVec3ToVector3(_buf01.block.normal);
		hit.hitPos = PxVec3ToVector3(_buf01.block.position);

		return true;
	}

	return false;
}

bool MoolyEngine::Scene::CheckBox(physx::PxVec3 center, physx::PxVec3 halfExtents, std::vector<std::string> filteredLayers)
{
	SCENE_LOCK sceneLock(*m_scene);

	physx::PxTransform transform(center);

	physx::PxBoxGeometry geometry(halfExtents);

	constexpr int bufSize = 128;
	physx::PxOverlapHit hitBuffer[bufSize];
	physx::PxOverlapBuffer buf(hitBuffer, bufSize);

	physx::PxQueryFilterData filterData;

	for (auto _filterLayer : filteredLayers)
	{
		auto _foundedLayer = m_layer.find(_filterLayer);
		if (_foundedLayer == m_layer.end())
		{
			continue;
		}

		filterData.data.word0 -= _foundedLayer->second.first;
	}

	bool result = m_scene->overlap(geometry, transform, buf, filterData);

	return result;
}

bool MoolyEngine::Scene::GetTransform(const std::string& objName, physx::PxTransform& transform)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);

	if (_foundedActor == m_rigidActors.end())
	{
		return false;
	}

	else
	{
		auto _actor = _foundedActor->second;
		if (_actor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
		{
			if (((physx::PxRigidDynamic*)_actor)->isSleeping() == false)
			{
				transform = ((physx::PxRigidDynamic*)_actor)->getGlobalPose();
				return true;
			}

			return false;
		}
	}

	return false;
}

bool MoolyEngine::Scene::AddColliderEvent(std::string eventHolder, IEventCollider* callbackclass)
{
	return m_eventCallback->AddColliderEvent(eventHolder, callbackclass);
}

bool MoolyEngine::Scene::AddTriggerEvent(std::string eventHolder, IEventTrigger* callbackclass)
{
	return m_eventCallback->AddTriggerEvent(eventHolder, callbackclass);
}

bool MoolyEngine::Scene::AddLayer(const std::string& layerName)
{
	UINT _layerCount = m_layer.size();

	if (_layerCount > 31)
	{
		return false;
	}

	physx::PxU32 _filterID = 1 << _layerCount;
	physx::PxU32 _defaultFilterMask = 0xFFFFFFFF;

	m_layer.emplace(layerName, std::make_pair(_filterID, _defaultFilterMask));

	return true;
}

bool MoolyEngine::Scene::SetLayerFilterData(const std::string& layerName, std::vector<std::string> filteredlayers)
{
	physx::PxU32 _filterMask = 0XFFFFFFFF;

	auto _currentSettingLayer = m_layer.find(layerName);

	if (_currentSettingLayer == m_layer.end())
	{
		return false;
	}

	for (auto _filterLayer : filteredlayers)
	{
		auto _foundedLayer = m_layer.find(_filterLayer);
		if (_foundedLayer == m_layer.end())
		{
			continue;
		}

		_filterMask -= _foundedLayer->second.first;
	}

	_currentSettingLayer->second.second = _filterMask;
	return true;
}

bool MoolyEngine::Scene::SetLayer(const std::string& objName, const std::string& layerName)
{
	SCENE_LOCK sceneLock(*m_scene);

	auto _foundedActor = m_rigidActors.find(objName);

	if (_foundedActor == m_rigidActors.end())
	{
		return false;
	}

	else
	{
		auto _foundedLayer = m_layer.find(layerName);
		if (_foundedLayer == m_layer.end())
		{
			return false;
		}

		physx::PxFilterData _filterData;

		_filterData.word0 = _foundedLayer->second.first;
		_filterData.word1 = _foundedLayer->second.second;

		physx::PxShape** _shapes = new physx::PxShape*;
		(*_shapes) = nullptr;
		_foundedActor->second->getShapes(_shapes, 1);

		if (*_shapes != nullptr)
		{
			(*_shapes)->setSimulationFilterData(_filterData);
			_filterData.word1 = 0x00000000;
			(*_shapes)->setQueryFilterData(_filterData);
			delete _shapes;
			return true;
		}

		delete _shapes;
	}

	return false;
}

void MoolyEngine::Scene::Test(physx::PxPhysics* physics)
{
	SCENE_LOCK sceneLock(*m_scene);

	physx::PxMaterial* mMaterial = NULL;

	mMaterial = physics->createMaterial(0.6f, 0.6f, 0.2f);
	physx::PxRigidStatic* groundPlane = PxCreatePlane(*physics, physx::PxPlane(0, 1.f, 0, 10.f), *mMaterial);
	physx::PxShape** shapes = new physx::PxShape*;
	physx::PxFilterData filterData;

	filterData.word0 = 0x00000001;
	filterData.word1 = 0xFFFFFFFF;

	(*shapes) = nullptr;
	groundPlane->getShapes(shapes, 1);
	if (*shapes != nullptr)
	{
		(*shapes)->setSimulationFilterData(filterData);
	}

	delete shapes;
	m_scene->addActor(*groundPlane);

}

physx::PxFilterFlags MoolyEngine::PhysicsWorldFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	PX_UNUSED(constantBlock);
	PX_UNUSED(constantBlockSize);

	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	auto const _layerMask01 = filterData0.word0;
	auto const _layer01 = filterData0.word1;

	auto const _layerMask02 = filterData1.word0;
	auto const _layer02 = filterData1.word1;

	auto const _collision01 = _layerMask01 & _layer01;
	auto const _collision02 = _layerMask02 & _layer02;

	if (_collision01 == 0 || _collision02 == 0)
	{
		return physx::PxFilterFlag::eSUPPRESS;
	}

	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

	return physx::PxFilterFlag::eDEFAULT;
}
