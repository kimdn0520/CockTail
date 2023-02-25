#pragma once
#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/binary_iarchive.hpp> 
#include <boost/archive/binary_oarchive.hpp>

namespace YAMLBinaryData
{
	struct Float2
	{
		friend boost::serialization::access;

		Float2() = default;

		Float2(float _x, float _y)
			: x(_x), y(_y)
		{}

		float x;
		float y;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& x;
			ar& y;
		}
	};

	struct Float3
	{
		friend boost::serialization::access;

		Float3() = default;

		Float3(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z)
		{}

		float x;
		float y;
		float z;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& x;
			ar& y;
			ar& z;
		}
	};

	struct Float4
	{
		friend boost::serialization::access;

		Float4() = default;

		Float4(float _x, float _y, float _z, float _w)
			: x(_x), y(_y), z(_z), w(_w)
		{}

		float x;
		float y;
		float z;
		float w;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& x;
			ar& y;
			ar& z;
			ar& w;
		}
	};

	class Transform
	{
	public:
		friend boost::serialization::access;

		Transform(
			Float3 _localPosition = { 0.f, 0.f, 0.f },
			Float3 _localRotation = { 0.f, 0.f, 0.f },
			Float3 _localScale = { 1.f, 1.f, 1.f }
		)
			: gameObjectID("")
			, componentID("")
			, localPosition(std::move(_localPosition))
			, localRotation(std::move(_localRotation))
			, localScale(std::move(_localScale))
		{}

	public:
		std::string gameObjectID;
		std::string componentID;
		std::vector<std::string> childIDList;

		Float3 localPosition;
		Float3 localRotation;
		Float3 localScale;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& gameObjectID;
			ar& componentID;
			ar& childIDList;

			ar& localPosition;
			ar& localRotation;
			ar& localScale;
		}

	};

	class BoxCollider
	{
	public:
		friend boost::serialization::access;

		BoxCollider(
			Float3 _size = { 1.f, 1.f, 1.f },
			Float3 _center = { 0.f, 0.f, 0.f },
			bool _isTrigger = false
		)
			: componentID("")
			, gameObjectID("")
			, size(std::move(_size))
			, center(std::move(_center))
			, isTrigger(_isTrigger)
		{}

	public:
		std::string componentID;
		std::string gameObjectID;

		Float3 size;
		Float3 center;
		bool isTrigger;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& componentID;
			ar& gameObjectID;

			ar& size;
			ar& center;
			ar& isTrigger;
		}
	};

	class SphereCollider
	{
	public:
		friend boost::serialization::access;

		SphereCollider(
			float _radius = 0.f,
			Float3 _center = { 0.f, 0.f, 0.f },
			bool _isTrigger = false
		)
			: componentID("")
			, gameObjectID("")
			, radius(std::move(_radius))
			, center(std::move(_center))
			, isTrigger(_isTrigger)
		{}

	public:
		std::string componentID;
		std::string gameObjectID;

		float radius;
		Float3 center;
		bool isTrigger;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& componentID;
			ar& gameObjectID;

			ar& radius;
			ar& center;
			ar& isTrigger;
		}
	};

	class Light
	{
	public:
		friend boost::serialization::access;

		Light(
			int _type = 0,
			Float4 _color = { 0.f, 0.f, 0.f, 0.f }
		)
			: componentID("")
			, gameObjectID("")
			, type(std::move(_type))
			, color(std::move(_color))
			, intensity(1.f)
			, range(10.f)
			, outerSpotAngle(30.f)
			, innerSpotAngle(40.f)
		{}

	public:
		std::string componentID;
		std::string gameObjectID;

		int type;
		Float4 color;
		float intensity;
		float range;
		float outerSpotAngle;
		float innerSpotAngle;
		int shadow;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& componentID;
			ar& gameObjectID;

			ar& type;
			ar& color;
			ar& intensity;
			ar& range;
			ar& outerSpotAngle;
			ar& innerSpotAngle;
			ar& shadow;
		}
	};

	class Camera
	{
	public:
		friend boost::serialization::access;

		Camera(float _nearPlane = 0.1f, float _farPlane = 1000.f)
			: componentID("")
			, gameObjectID("")
			, projectionMatrixMode(0)
			, nearPlane(_nearPlane)
			, farPlane(_farPlane)
		{}

	public:
		std::string componentID;
		std::string gameObjectID;

		int projectionMatrixMode;

		float nearPlane;
		float farPlane;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& componentID;
			ar& gameObjectID;

			ar& projectionMatrixMode;
			ar& nearPlane;
			ar& farPlane;
		}
	};

	class MeshFilter
	{
	public:
		friend boost::serialization::access;

		MeshFilter(std::string _meshName = "")
			: componentID("")
			, gameObjectID("")
			, meshName(std::move(_meshName))
		{}

	public:
		std::string componentID;
		std::string gameObjectID;

		std::string meshName;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& componentID;
			ar& gameObjectID;

			ar& meshName;
		}
	};

	class GameObject
	{
	public:
		friend boost::serialization::access;

		GameObject(std::string _name = "")
			: name(std::move(_name))
			, tag("")
			, gameObjectID("")
			, transform(nullptr)
			, camera(nullptr)
			, light(nullptr)
			, boxCollider(nullptr)
			, sphereCollider(nullptr)
			, meshFilter(nullptr)
		{}

	public:
		std::string name;
		std::string tag;
		int layer;
		std::string gameObjectID;

		std::shared_ptr<Transform> transform;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Light> light;
		std::shared_ptr<BoxCollider> boxCollider;
		std::shared_ptr<SphereCollider> sphereCollider;
		std::shared_ptr<MeshFilter> meshFilter;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& name;
			ar& tag;
			ar& layer;
			ar& gameObjectID;

			ar& transform;
			ar& camera;
			ar& light;
			ar& boxCollider;
			ar& sphereCollider;
			ar& meshFilter;
		}
	};

	class Prefab
	{
	public:
		friend boost::serialization::access;

		Prefab() = default;

		Prefab(std::string _name, Transform _transform)
			: name(std::move(_name))
			, prefabID("")
			, transform(std::move(_transform))
		{}

	public:
		std::string name = "";
		std::string prefabID;

		Transform transform;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& name;
			ar& prefabID;

			ar& transform;
		}
	};

	class Scene
	{
	public:
		friend boost::serialization::access;

		Scene() = default;

		Scene(
			std::vector<GameObject> _gameObjects,
			std::vector<Prefab> _prefabs,
			Float4 _color = { 0.f, 0.f, 0.f, 0.f }
		)
			: gameObjects(std::move(_gameObjects))
			, prefabs(std::move(_prefabs))
			, ambientSkyColor(_color)
		{}

	public:
		// CubeMesh, SphereMesh.. , Camera, Light
		std::vector<GameObject> gameObjects;

		std::vector<Prefab> prefabs;

		Float4 ambientSkyColor;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& gameObjects;
			ar& prefabs;

			ar& ambientSkyColor;
		}
	};
}