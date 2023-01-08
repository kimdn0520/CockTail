#pragma once
#include "FBXBinaryLayout.h"

struct FBXModel;

class BinarySerializer
{
private:


public:
	BinarySerializer() = default;
	~BinarySerializer() {};

public:
	FBXBinaryData::Float4x4 ConvertFloat4x4(DirectX::SimpleMath::Matrix matrix);

	void SaveBinaryFile(std::shared_ptr<FBXModel> fbxModel, std::string name, std::string path);

	void ReconstructionAnimName(std::string& name);
};

inline FBXBinaryData::Float4x4 BinarySerializer::ConvertFloat4x4(DirectX::SimpleMath::Matrix matrix)
{
	return FBXBinaryData::Float4x4
	(
		matrix._11, matrix._12, matrix._13, matrix._14,
		matrix._21, matrix._22, matrix._23, matrix._24,
		matrix._31, matrix._32, matrix._33, matrix._34,
		matrix._41, matrix._42, matrix._43, matrix._44
	);
}