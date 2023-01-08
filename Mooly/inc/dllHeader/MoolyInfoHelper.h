#pragma once
#include "Export/MoolyInfo.h"

namespace MoolyEngine
{
	physx::PxTransform TransformToPxTransform(const Transform& value);
	Transform PxTransformToTransform(const physx::PxTransform& value);

	physx::PxVec3 Vector3ToPxVec3(const Vector3& value);
	Vector3 PxVec3ToVector3(const physx::PxVec3& value);
}