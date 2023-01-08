#include "pch.h"
#include "dllHeader/MoolyInfoHelper.h"


physx::PxTransform MoolyEngine::TransformToPxTransform(const Transform& value)
{
	physx::PxTransform _temp;

	_temp.p.x = value.pos[0];
	_temp.p.y = value.pos[1];
	_temp.p.z = value.pos[2];

	_temp.q.x = value.quat[0];
	_temp.q.y = value.quat[1];
	_temp.q.z = value.quat[2];
	_temp.q.w = value.quat[3];

	return _temp;
}

MoolyEngine::Transform MoolyEngine::PxTransformToTransform(const physx::PxTransform& value)
{
	Transform _temp;

	_temp.pos[0] = value.p.x;
	_temp.pos[1] = value.p.y;
	_temp.pos[2] = value.p.z;

	_temp.quat[0] = value.q.x;
	_temp.quat[1] = value.q.y;
	_temp.quat[2] = value.q.z;
	_temp.quat[3] = value.q.w;

	return _temp;
}

physx::PxVec3 MoolyEngine::Vector3ToPxVec3(const Vector3& value)
{
	physx::PxVec3 _temp;

	_temp.x = value.vec[0];
	_temp.y = value.vec[1];
	_temp.z = value.vec[2];

	return _temp;
}

MoolyEngine::Vector3 MoolyEngine::PxVec3ToVector3(const physx::PxVec3& value)
{
	Vector3 _temp;

	_temp.vec[0] = value.x;
	_temp.vec[1] = value.y;
	_temp.vec[2] = value.z;

	return _temp;
}
