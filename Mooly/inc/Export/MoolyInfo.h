#pragma once
#include <string>
#include <vector>
#include <basetsd.h>

#include "dllDefine.h"

namespace MoolyEngine
{
	enum class MOOLY_ENGINE GeometryType
	{
		SPHERE,
		PLANE,
		CAPSULE,
		BOX,
		CONVEXMESH,
		TRIANGLEMESH,
		HEIGHTFIELD,
		GEOMETRY_COUNT,		//!< internal use only!
		INVALID = -1		//!< internal use only!
	};

	enum class MOOLY_ENGINE PhysicsType
	{
		STATIC,
		DYNAMIC,
		PHYSICSTYPE_COUNT,
		INVALID = -1
	};

	struct MOOLY_ENGINE Transform
	{
		float pos[3];
		float quat[4];
	};

	struct MOOLY_ENGINE Vector3
	{
		Vector3() :
			vec{ 0.f,0.f,0.f } {};

		Vector3(float x, float y, float z) :
			vec{ x,y,z } {};

		float vec[3];
	};

	struct MOOLY_ENGINE Vector4
	{
		float vec[4];
	};

	struct MOOLY_ENGINE PhysicsAxisLock
	{
		PhysicsAxisLock() = default;
		PhysicsAxisLock(bool _linearX, bool _linearY, bool _linearZ,
			bool _angularX, bool _angularY, bool _angularZ) :

			linearX(_linearX),
			linearY(_linearY),
			linearZ(_linearZ),
			angularX(_angularX),
			angularY(_angularY),
			angularZ(_angularZ) {};

		bool linearX = false;
		bool linearY = false;
		bool linearZ = false;

		bool angularX = false;
		bool angularY = false;
		bool angularZ = false;
	};

	struct MOOLY_ENGINE RayCastHit
	{
		std::string objectName;
		float _distance = 0;
	};

	class MOOLY_ENGINE IEventTrigger
	{
	public:
		virtual void OnTriggerEnter(std::string name) abstract;
		virtual void OnTriggerPersist(std::string name) abstract;
		virtual void OnTriggerExit(std::string name) abstract;
	};

	class MOOLY_ENGINE IEventCollider
	{
	public:
		virtual void OnCollisionEnter(std::string name) abstract;
		virtual void OnCollisionPersist(std::string name) abstract;
		virtual void OnCollisionExit(std::string name) abstract;
	};
}
