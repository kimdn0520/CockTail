#pragma once
#include "pch.h"

struct Vertex
{
	Vertex(DirectX::SimpleMath::Vector3 _position = { 0.f, 0.f, 0.f },
		DirectX::SimpleMath::Vector4 _color = { 0.0f, 0.0f, 0.0f, 1.0f },
		DirectX::SimpleMath::Vector2 _uv = { 0.f, 0.f },
		DirectX::SimpleMath::Vector3 _normal = { 0.f, 0.f, 0.f },
		DirectX::SimpleMath::Vector3 _tangent = { 0.f, 0.f, 0.f })
		: position(_position), normal(_normal), uv(_uv), tangent(_tangent), color(_color)
	{
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = -1;
		}
	}

	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector4 color;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Vector3 tangent;

	float weights[8];
	short boneIndices[8];
};

struct FBXMaterialInfo
{
	std::string materialName;

	std::string albedoMap;
	std::string normalMap;
	std::string metallicMap;
	std::string roughnessMap;
	std::string AOMap;
	std::string emissiveMap;

	DirectX::SimpleMath::Vector4 material_Ambient;
	DirectX::SimpleMath::Vector4 material_Diffuse;
	DirectX::SimpleMath::Vector4 material_Specular;
	DirectX::SimpleMath::Vector4 material_Emissive;

	float emissionFactor;			// 방출 크기
	float material_Transparency;	// 투명도
	float material_Reflectivity;	// 반사율

	float metallic;					// 금속성
	float roughness;				// 거칠기
};

struct FBXMeshInfo
{
	std::string								meshName;
	std::string								parentName;
	std::vector<std::string>				materials;
	std::vector<Vertex>						meshVertexList;
	std::vector<std::vector<unsigned int>>	indices;

	DirectX::SimpleMath::Matrix nodeTM;

	bool isSkinned = false;		// 스키닝오브젝트 인지 아닌지..
};

struct FBXBoneInfo
{
	std::string				boneName;

	int						parentIndex;

	DirectX::SimpleMath::Matrix offsetMatrix;
	DirectX::SimpleMath::Matrix nodeMatrix;
};

struct FBXKeyFrameInfo
{
	float		time;

	DirectX::SimpleMath::Vector3 localTransform;
	DirectX::SimpleMath::Quaternion localRotation;
	DirectX::SimpleMath::Vector3 localScale;
};

struct FBXAnimClipInfo
{
	std::string	animationName;

	float frameRate = 0.0f;

	float tickPerFrame = 0.0f;

	int totalKeyFrame = 0;

	int startKeyFrame = 0;

	int endKeyFrame = 0;

	std::vector<std::vector<std::shared_ptr<FBXKeyFrameInfo>>> keyFrameList;
};

class FBXModel
{
public:
	std::vector<std::shared_ptr<FBXMeshInfo>> fbxMeshInfoList;

	std::vector<std::shared_ptr<FBXMaterialInfo>>	materialList;

	std::vector<std::shared_ptr<FBXBoneInfo>> fbxBoneInfoList;

	std::vector<std::shared_ptr<FBXAnimClipInfo>> animationClipList;

	bool isSkinnedAnimation;
};

