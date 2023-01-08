#pragma once

struct FBXModel;
struct FBXMeshInfo;

class FBXParser
{
private:

	std::shared_ptr<FBXModel> fbxModel;

	fbxsdk::FbxManager* manager = nullptr;
	fbxsdk::FbxScene* scene = nullptr;
	fbxsdk::FbxImporter* importer = nullptr;
	fbxsdk::FbxGeometryConverter* geometryConverter;

	fbxsdk::FbxArray<fbxsdk::FbxString*> animNames;

	bool isNegativeScale = false;

public:
	FBXParser();
	~FBXParser();

public:
	void LoadFbx(const std::string& fbxFilePath, const std::string& saveName);

	void Import(const std::string& path);

	void ProcessMesh(fbxsdk::FbxNode* node, FbxNodeAttribute::EType attribute);

	void LoadMesh(fbxsdk::FbxNode* node, fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshData, int meshCnt);

	void ProcessBones(fbxsdk::FbxNode* node, int idx, int parentIdx);

	void LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial, std::shared_ptr<FBXMeshInfo>& meshData);

	void LoadAnimation();

	void ProcessAnimationData(FbxNode* node);

	int FindBoneIndex(std::string boneName);

	DirectX::SimpleMath::Matrix GetNodeTM(fbxsdk::FbxNode* node);

	DirectX::SimpleMath::Vector3 GetNormal(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	void GetTangent(std::shared_ptr<FBXMeshInfo>& meshInfo, int meshCnt);

	DirectX::SimpleMath::Vector2 GetUV(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	std::string GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty);

	fbxsdk::FbxAMatrix GetTransformMatrix(FbxNode* node);

	DirectX::SimpleMath::Vector4 ConvertVector4(fbxsdk::FbxVector4 v4);

	DirectX::SimpleMath::Matrix ConvertMatrix(fbxsdk::FbxMatrix matrix);
};

inline DirectX::SimpleMath::Vector4 FBXParser::ConvertVector4(fbxsdk::FbxVector4 v4)
{
	// xyzw -> xzyw
	return DirectX::SimpleMath::Vector4
	(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[3])
	);
}

inline DirectX::SimpleMath::Matrix FBXParser::ConvertMatrix(fbxsdk::FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	return DirectX::SimpleMath::Matrix
	(
		ConvertVector4(r1),
		ConvertVector4(r3),
		ConvertVector4(r2),
		ConvertVector4(r4)
	);
}
