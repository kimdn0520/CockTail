#pragma once
#include <iostream>
#include <fstream>
#include <boost/serialization/access.hpp>

namespace fs = std::filesystem;

namespace FBXBinaryData
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

	struct Float4x4
	{
		friend boost::serialization::access;

		union
		{
			struct
			{
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};

			float m[4][4];
		};

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& m;
		}
	};

	struct VertexData
	{
		friend boost::serialization::access;

		VertexData(
			Float3 _position = { 0.f, 0.f, 0.f },
			Float4 _color = { 1.0f, 1.0f, 1.0f, 1.0f },
			Float2 _uv = { 0.f, 0.f },
			Float3 _normal = { 0.f, 0.f, 0.f },
			Float3 _tangent = { 0.f, 0.f, 0.f })
			: position(std::move(_position))
			, normal(std::move(_normal))
			, uv(std::move(_uv))
			, tangent(std::move(_tangent))
			, color(std::move(_color))
		{
			for (int i = 0; i < 8; i++)
			{
				weights[i] = 0.f;
				boneIndices[i] = -1;
			}
		}

		Float3 position;
		Float4 color;
		Float2 uv;
		Float3 normal;
		Float3 tangent;

		// Skinned Data
		float weights[8];
		short boneIndices[8];

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& position;
			ar& color;
			ar& uv;
			ar& normal;
			ar& tangent;
			ar& weights;
			ar& boneIndices;
		}
	};

	class MaterialData
	{
	public:
		friend boost::serialization::access;

		MaterialData() = default;

		MaterialData(std::string _materialName, std::string _albedoMap, std::string _normalMap, std::string _metallicMap, std::string _roughnessMap
			, std::string _AOMap, std::string _emissiveMap, Float4 _material_Ambient, Float4 _material_Diffuse, Float4 _material_Specular, Float4 _material_Emissive
			, float _emissionFactor, float _material_Transparency, float _material_Reflectivity, float _metallic, float _roughness)
			: materialName(std::move(_materialName))
			, albedoMap(std::move(_albedoMap))
			, normalMap(std::move(_normalMap))
			, metallicMap(std::move(_metallicMap))
			, roughnessMap(std::move(_roughnessMap))
			, AOMap(std::move(_AOMap))
			, emissiveMap(std::move(_emissiveMap))
			, material_Ambient(std::move(_material_Ambient))
			, material_Diffuse(std::move(_material_Diffuse))
			, material_Specular(std::move(_material_Specular))
			, material_Emissive(std::move(_material_Emissive))
			, emissionFactor(std::move(_emissionFactor))
			, material_Transparency(std::move(_material_Transparency))
			, material_Reflectivity(std::move(_material_Reflectivity))
			, metallic(std::move(_metallic))
			, roughness(std::move(_roughness))
		{}

	public:
		std::string materialName = "";

		std::string albedoMap = "";
		std::string normalMap = "";
		std::string metallicMap = "";
		std::string roughnessMap = "";
		std::string AOMap = "";
		std::string emissiveMap = "";

		Float4 material_Ambient = { 0.f, 0.f, 0.f, 0.f };
		Float4 material_Diffuse = { 0.f, 0.f, 0.f, 0.f };
		Float4 material_Specular = { 0.f, 0.f, 0.f, 0.f };
		Float4 material_Emissive = { 0.f, 0.f, 0.f, 0.f };

		float emissionFactor = 0.f;
		float material_Transparency = 0.f;
		float material_Reflectivity = 0.f;

		float metallic = 0.f;
		float roughness = 0.f;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& materialName;

			ar& albedoMap;
			ar& normalMap;
			ar& metallicMap;
			ar& roughnessMap;
			ar& AOMap;
			ar& emissiveMap;

			ar& material_Ambient;
			ar& material_Diffuse;
			ar& material_Specular;
			ar& material_Emissive;

			ar& material_Transparency;
			ar& material_Reflectivity;

			ar& metallic;
			ar& roughness;
		}
	};

	class MeshData
	{
	public:
		friend boost::serialization::access;

		MeshData() = default;

		MeshData(std::string _meshName, std::string _parentName, std::vector<std::string> _materials, std::vector<VertexData> _meshVertexList
			, std::vector<std::vector<unsigned int>> _indices, bool _isSkinned = false)
			: meshName(std::move(_meshName))
			, parentName(std::move(_parentName))
			, materials(std::move(_materials))
			, meshVertexList(std::move(_meshVertexList))
			, indices(std::move(_indices))
			, isSkinned(std::move(_isSkinned))
		{}

	public:
		std::string								meshName = "";
		std::string								parentName = "";
		std::vector<std::string>				materials;
		std::vector<VertexData>					meshVertexList;
		std::vector<std::vector<unsigned int>>	indices;

		bool isSkinned = false;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& meshName;
			ar& parentName;
			ar& materials;
			ar& meshVertexList;
			ar& indices;
			ar& isSkinned;
		}
	};

	class BoneData
	{
	public:
		friend boost::serialization::access;

		BoneData() = default;

		BoneData(std::string _boneName, int _parentIndex, Float4x4 _offsetMatrix, Float4x4 _nodeMatrix)
			: boneName(std::move(_boneName))
			, parentIndex(std::move(_parentIndex))
			, offsetMatrix(std::move(_offsetMatrix))
			, nodeMatrix(std::move(_nodeMatrix))
		{}

	public:
		std::string	boneName = "";

		short 		parentIndex = 0;

		Float4x4	offsetMatrix;
		Float4x4	nodeMatrix;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boneName;
			ar& parentIndex;
			ar& offsetMatrix;
			ar& nodeMatrix;
		}
	};

	class KeyFrameInfoData
	{
	public:
		friend boost::serialization::access;

		KeyFrameInfoData() = default;

		KeyFrameInfoData(float _time, Float3 _localTransform, Float4 _localRotation, Float3 _localScale)
			: time(std::move(_time))
			, localTransform(std::move(_localTransform))
			, localRotation(std::move(_localRotation))
			, localScale(std::move(_localScale))
		{}

	public:
		float  time = 0.f;

		Float3 localTransform = { 0.f, 0.f, 0.f };
		Float4 localRotation = { 0.f, 0.f, 0.f, 0.f };
		Float3 localScale = { 0.f, 0.f, 0.f };

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& time;
			ar& localTransform;
			ar& localRotation;
			ar& localScale;
		}
	};

	class AnimationClipData
	{
	public:
		friend boost::serialization::access;

		AnimationClipData() = default;

		AnimationClipData(std::string _animationName, float _frameRate, float _tickPerFrame, short  _totalKeyFrame, short  _startKeyFrame, short  _endKeyFrame
			, std::vector<std::vector<KeyFrameInfoData>> _keyFrameList)
			: animationName(std::move(_animationName))
			, frameRate(std::move(_frameRate))
			, tickPerFrame(std::move(_tickPerFrame))
			, totalKeyFrame(std::move(_totalKeyFrame))
			, startKeyFrame(std::move(_startKeyFrame))
			, endKeyFrame(std::move(_endKeyFrame))
			, keyFrameList(std::move(_keyFrameList))
		{}

	public:
		std::string	animationName;

		float frameRate = 0.0f;

		float tickPerFrame = 0.0f;

		short  totalKeyFrame = 0;

		short  startKeyFrame = 0;

		short  endKeyFrame = 0;

		std::vector<std::vector<KeyFrameInfoData>> keyFrameList;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& animationName;
			ar& frameRate;
			ar& tickPerFrame;
			ar& totalKeyFrame;
			ar& startKeyFrame;
			ar& endKeyFrame;
			ar& keyFrameList;
		}
	};

	/*class ModelData
	{
	public:
		friend boost::serialization::access;

		ModelData() = default;

		ModelData(std::vector<MeshData> _meshInfoList, std::vector<MaterialData> _materialList
			, std::vector<BoneData> _boneInfoList, std::vector<AnimationClipData> _animationList
			, bool isSkinnedAnimation = false)
			: meshInfoList(std::move(_meshInfoList))
			, materialList(std::move(_materialList))
			, boneInfoList(std::move(_boneInfoList))
			, animationClipList(std::move(_animationList))
			, isSkinnedAnimation(std::move(isSkinnedAnimation))
		{}

	public:
		std::vector<MeshData> meshInfoList;

		std::vector<MaterialData>	materialList;

		std::vector<BoneData> boneInfoList;

		std::vector<AnimationClipData> animationClipList;

		bool isSkinnedAnimation = false;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& meshInfoList;
			ar& materialList;
			ar& boneInfoList;
			ar& animationClipList;
			ar& isSkinnedAnimation;
		}
	};*/

	class ModelData
	{
	public:
		friend boost::serialization::access;

		ModelData() = default;

		ModelData(std::vector<std::string> _meshInfoList, std::vector<std::string> _materialList,
			std::vector<std::string> _animationList, std::vector<BoneData> _boneInfoList, bool _isSkinnedAnimation)
			: meshInfoList(std::move(_meshInfoList))
			, materialList(std::move(_materialList))
			, animationClipList(std::move(_animationList))
			, boneInfoList(std::move(_boneInfoList))
			, isSkinnedAnimation(_isSkinnedAnimation)
		{}

	public:
		std::string name;

		std::vector<std::string> meshInfoList;

		std::vector<std::string> materialList;

		std::vector<std::string> animationClipList;

		std::vector<BoneData> boneInfoList;

		bool isSkinnedAnimation = false;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& name;

			ar& meshInfoList;
			ar& materialList;
			ar& animationClipList;
			ar& boneInfoList;

			ar& isSkinnedAnimation;
		}
	};
}

