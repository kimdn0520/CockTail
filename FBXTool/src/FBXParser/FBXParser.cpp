#include "pch.h"
#include "FBXParser/FBXParser.h"
#include "ParserData/ParserData.h"
#include "BinarySerializer/BinarySerializer.h"

FBXParser::FBXParser()
{}

FBXParser::~FBXParser()
{
	if (scene)
		scene->Destroy();

	if (manager)
		manager->Destroy();
}

void FBXParser::LoadFbx(const std::string& fbxFilePath, const std::string& saveName)
{
	fbxModel = std::make_shared<FBXModel>();

	Import(fbxFilePath);

	LoadAnimation();

	ProcessBones(scene->GetRootNode(), 0, -1);

	ProcessMesh(scene->GetRootNode(), FbxNodeAttribute::eMesh);

	std::shared_ptr<BinarySerializer> binarySerializer = std::make_shared<BinarySerializer>();

	std::string saveBinaryPath = "BinaryFile/";

	binarySerializer->SaveBinaryFile(fbxModel, saveName, saveBinaryPath);
}

void FBXParser::Import(const std::string& path)
{
	// FBX SDK 관리자 객체 생성
	manager = fbxsdk::FbxManager::Create();

	// IOSettings 객체 생성 및 설정
	fbxsdk::FbxIOSettings* settings = fbxsdk::FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);

	// 씬 객체 생성
	scene = fbxsdk::FbxScene::Create(manager, "");

	// FbxImporter 객체 생성
	importer = fbxsdk::FbxImporter::Create(manager, "");

	// importer 초기화
	importer->Initialize(path.c_str(), -1, manager->GetIOSettings());

	// fbx 파일 내용을 scene 으로 가져온다.
	importer->Import(scene);

	FbxSystemUnit lFbxFileSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
	FbxSystemUnit lFbxOriginSystemUnit = scene->GetGlobalSettings().GetOriginalSystemUnit();
	double factor = lFbxOriginSystemUnit.GetScaleFactor();

	const FbxSystemUnit::ConversionOptions lConversionOptions =
	{
	  true,
	  true,
	  true,
	  true,
	  true,
	  true
	};
	lFbxFileSystemUnit.m.ConvertScene(scene, lConversionOptions);
	lFbxOriginSystemUnit.m.ConvertScene(scene, lConversionOptions);

	// GeometryConverter 객체 생성
	geometryConverter = new FbxGeometryConverter(manager);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다. 
	geometryConverter->Triangulate(scene, true, true);

	// 여러 머터리얼이 하나의 메쉬에 할당된것을 하나의 메쉬가 하나의 머터리얼로 가지게함
	geometryConverter->SplitMeshesPerMaterial(scene, true);

	// importer 파괴
	importer->Destroy();
}

/// <summary>
/// Mesh들을 로드해서 FBXModel의 fbxMeshInfoList에 저장한다.
/// 버텍스의 정보를 모두 담고 bone에 영향을 받는 mesh가 있다면 bone 가중치도 넣어주고
/// bone에 offsetMatrix 정보도 추가해준다
/// <summary>
void FBXParser::ProcessMesh(fbxsdk::FbxNode* node, FbxNodeAttribute::EType attribute)
{
	fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute && nodeAttribute->GetAttributeType() == attribute)
	{
		// mesh는 하나로 쓸거임 이 mesh의 vertexList에 vertex들 다담고 material이랑 indexBuffer를 여러개 둘거임
		std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

		fbxMeshInfo->indices.resize(node->GetNodeAttributeCount());

		for (int meshCnt = 0; meshCnt < node->GetNodeAttributeCount(); meshCnt++)
		{
			fbxsdk::FbxMesh* mesh = (fbxsdk::FbxMesh*)node->GetNodeAttributeByIndex(meshCnt);

			LoadMesh(node, mesh, fbxMeshInfo, meshCnt);

			fbxsdk::FbxLayerElementMaterial* findMatIndex = mesh->GetElementMaterial(0);

			if (findMatIndex != nullptr)
			{
				int index = findMatIndex->GetIndexArray().GetAt(0);

				fbxsdk::FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<fbxsdk::FbxSurfaceMaterial>(index);

				LoadMaterial(surfaceMaterial, fbxMeshInfo);
			}
		}

		fbxModel->fbxMeshInfoList.emplace_back(fbxMeshInfo);
	}

	// Tree 구조 재귀 호출
	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; ++i)
		ProcessMesh(node->GetChild(i), FbxNodeAttribute::eMesh);
}


void FBXParser::LoadMesh(fbxsdk::FbxNode* node, fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshData, int meshCnt)
{
	// Node TM 넣기
	FbxAMatrix nodeTransform = scene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);

	DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);

	DirectX::XMVECTOR det = XMMatrixDeterminant(nodeMatrix);

	// 음수면 네거리브~
	if (det.m128_f32[0] < 0)
	{
		// Decompose 했다가 scale -주고 다시 합쳐야함..
		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR rotQuat;
		DirectX::XMVECTOR trans;
		DirectX::XMMatrixDecompose(&scale, &rotQuat, &trans, nodeMatrix);
		DirectX::XMVECTOR minusScale = { -scale.m128_f32[0], -scale.m128_f32[1], -scale.m128_f32[2] };
		scale = minusScale;

		// 다시 SRT 조립
		nodeMatrix = DirectX::XMMatrixScaling(scale.m128_f32[0], scale.m128_f32[1], scale.m128_f32[2]) *
			DirectX::XMMatrixRotationQuaternion(rotQuat) *
			DirectX::XMMatrixTranslation(trans.m128_f32[0], trans.m128_f32[1], trans.m128_f32[2]);

		isNegativeScale = true;
	}

	const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

	const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

	DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

	nodeMatrix *= XMMatrixRotationQuaternion(q);

	meshData->nodeTM = nodeMatrix;

	// mesh 이름 넣기
	meshData->meshName = mesh->GetName();

	// Node Parent 찾기
	std::string parentName = node->GetParent()->GetName();

	// 부모 이름 넣기
	meshData->parentName = parentName;

	const int vertexCount = mesh->GetControlPointsCount();

	// 임시 버텍스리스트
	std::vector<Vertex> tmpMeshVertexList;
	tmpMeshVertexList.resize(vertexCount);

	// Position정보를 가져옴(축 바꿔서 가져옴)
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < vertexCount; ++i)
	{
		tmpMeshVertexList[i].position.x = static_cast<float>(controlPoints[i].mData[0]);
		tmpMeshVertexList[i].position.y = static_cast<float>(controlPoints[i].mData[2]);
		tmpMeshVertexList[i].position.z = static_cast<float>(controlPoints[i].mData[1]);
	}

	const int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < deformerCount; i++)
	{
		meshData->isSkinned = true;

		fbxModel->isSkinnedAnimation = true;	// 일단 여기서..

		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (fbxSkin)
		{
			FbxSkin::EType type = fbxSkin->GetSkinningType();

			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				// FbxCluster는 Skinning정보가 있는 뼈대의 갯수만 준다.
				const int clusterCount = fbxSkin->GetClusterCount();

				for (int j = 0; j < clusterCount; j++)
				{
					FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					// 해당 본이 뭔지 찾아옴
					int boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
					assert(boneIdx >= 0);

					// 해당 본에 영향을 받는 정점의 인덱스 갯수가 나옴
					const int indicesCount = cluster->GetControlPointIndicesCount();

					for (int k = 0; k < indicesCount; k++)
					{
						double weight = cluster->GetControlPointWeights()[k];	// 해당 정점에서의 가중치

						int vtxIdx = cluster->GetControlPointIndices()[k];		// 해당 정점의 인덱스를 얻기

						// 최대 8개로 할거야
						// 돌면서 빈곳에 넣고 break 함
						for (int weightIdx = 0; weightIdx < 8; weightIdx++)
						{
							if (tmpMeshVertexList[vtxIdx].boneIndices[weightIdx] == -1)
							{
								tmpMeshVertexList[vtxIdx].weights[weightIdx] = weight;

								tmpMeshVertexList[vtxIdx].boneIndices[weightIdx] = boneIdx;

								break;
							}
						}
					}

					// 계층구조를 가진 BoneOffsetMatrix를 구해야한다.
					// GetTrasnformLinkMatrix로 해당 Bone의 월드 Matrix를 얻고 GetTransformMatrix로는 부모까지 오게된 Matrix를 얻는다.
					// 해당 부모 행렬의 역행렬을 곱하면 부모행렬기준 자기 자신의 정보에 대한 행렬값만 남게 되서 계층 구조적 행렬 값을 갖는다.
					FbxAMatrix matClusterTransformMatrix;
					FbxAMatrix matClusterLinkTransformMatrix;

					cluster->GetTransformMatrix(matClusterTransformMatrix);				// The transformation of the mesh at binding time 
					cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);		// The transformation of the cluster(joint) at binding time from joint space to world space 

					// Bone Matrix 설정..
					DirectX::SimpleMath::Matrix clusterMatrix = ConvertMatrix(matClusterTransformMatrix);
					DirectX::SimpleMath::Matrix clusterlinkMatrix = ConvertMatrix(matClusterLinkTransformMatrix);

					// BindPose 행렬을 구하자
					FbxAMatrix geometryTransform = GetTransformMatrix(mesh->GetNode());
					DirectX::SimpleMath::Matrix geometryMatrix = ConvertMatrix(geometryTransform);

					// OffsetMatrix는 WorldBindPose의 역행렬
					DirectX::SimpleMath::Matrix offsetMatrix = clusterMatrix * clusterlinkMatrix.Invert() * geometryMatrix;

					fbxModel->fbxBoneInfoList[boneIdx]->offsetMatrix = offsetMatrix;
				}
			}
		}
	}

	const int polygonSize = mesh->GetPolygonSize(0);

	assert(polygonSize == 3);		// 삼각형이 아니면 오류

	int arrIdx[3];
	int vertexCounter = 0;			// 정점의 개수

	const int triCount = mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다

	std::map<std::tuple<unsigned, float, float, float, float, float>, unsigned> indexMap;

	for (int i = 0; i < triCount; i++) // 삼각형의 개수
	{
		for (int j = 0; j < 3; j++)	   // 삼각형은 세 개의 정점으로 구성
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점의 인덱스 추출

			arrIdx[j] = controlPointIndex;

			DirectX::SimpleMath::Vector3 normal;

			normal = GetNormal(mesh, controlPointIndex, vertexCounter);

			DirectX::SimpleMath::Vector2 uv;

			uv = GetUV(mesh, controlPointIndex, vertexCounter);

			const auto indexPair = std::make_tuple(controlPointIndex, uv.x, uv.y, normal.x, normal.y, normal.z);

			const auto iter = indexMap.find(indexPair);

			// map에 없으면 insert 및 새로운 버텍스 제작
			if (iter == indexMap.end())
			{
				Vertex vertex;
				vertex.position = tmpMeshVertexList[controlPointIndex].position;	// 포지션은 동일

				// 가중치 정보 동일
				for (int weightIdx = 0; weightIdx < 8; weightIdx++)
				{
					vertex.weights[weightIdx] = tmpMeshVertexList[controlPointIndex].weights[weightIdx];

					vertex.boneIndices[weightIdx] = tmpMeshVertexList[controlPointIndex].boneIndices[weightIdx];
				}

				vertex.uv = uv;

				vertex.normal = normal;

				meshData->meshVertexList.emplace_back(vertex);								// 새로운 버텍스 삽입

				controlPointIndex = meshData->meshVertexList.size() - 1;				// index 새로운 버텍스 껄로 바꾸기

				arrIdx[j] = controlPointIndex;

				indexMap.insert({ indexPair, controlPointIndex });
			}
			// map에 있던거라면
			else
			{
				arrIdx[j] = iter->second;
			}

			vertexCounter++;
		}

		meshData->indices[meshCnt].emplace_back(arrIdx[0]);
		meshData->indices[meshCnt].emplace_back(arrIdx[2]);
		meshData->indices[meshCnt].emplace_back(arrIdx[1]);
	}

	// tangent 정보를 가져온다.
	if (mesh->GetElementNormalCount() >= 1)
		GetTangent(meshData, meshCnt);

	// 네거티브 bool 초기화
	isNegativeScale = false;
}

/// <summary>
/// FBXModel의 fbxBoneInfoList에 bone들을 전부 저장한다.
/// 정보는 boneName, parentIndex 이 담긴다.
/// </summary>
void FBXParser::ProcessBones(fbxsdk::FbxNode* node, int idx, int parentIdx)
{
	fbxsdk::FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
	{
		std::shared_ptr<FBXBoneInfo> fbxBoneInfo = std::make_shared<FBXBoneInfo>();

		fbxBoneInfo->boneName = node->GetName();

		fbxBoneInfo->parentIndex = parentIdx;

		FbxAMatrix nodeTransform = node->EvaluateGlobalTransform(fbxsdk::FbxTime(0));

		DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);

		const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

		const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

		DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

		nodeMatrix *= XMMatrixRotationQuaternion(q);

		fbxBoneInfo->nodeMatrix = nodeMatrix;

		fbxModel->fbxBoneInfoList.push_back(fbxBoneInfo);

		ProcessAnimationData(node);
	}

	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; i++)
	{
		if (attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			ProcessBones(node->GetChild(i), static_cast<int>(fbxModel->fbxBoneInfoList.size()), idx);
		}
		else
		{
			ProcessBones(node->GetChild(i), static_cast<int>(fbxModel->fbxBoneInfoList.size()), parentIdx);
		}
	}
}

void FBXParser::LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial, std::shared_ptr<FBXMeshInfo>& meshData)
{
	std::string matName = surfaceMaterial->GetName();

	std::shared_ptr<FBXMeshInfo> meshInfo = meshData;

	// 메시에는 머터리얼 이름만
	meshInfo->materials.emplace_back(matName);

	auto it = find_if(fbxModel->materialList.begin(), fbxModel->materialList.end(), [&name = matName](const std::shared_ptr<FBXMaterialInfo>& s)->bool {return (s->materialName == name); });

	// 이미 있던 material이면 return
	if (it != fbxModel->materialList.end())
		return;

	std::shared_ptr<FBXMaterialInfo> material = std::make_shared<FBXMaterialInfo>();

	material->materialName = surfaceMaterial->GetName();

	if (surfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Ambient Data
		material->material_Ambient.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[0]) * 10.0f;
		material->material_Ambient.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[1]) * 10.0f;
		material->material_Ambient.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[2]) * 10.0f;
		material->material_Ambient.w = 1.0f;

		// Diffuse Data
		material->material_Diffuse.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[0]);
		material->material_Diffuse.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[1]);
		material->material_Diffuse.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[2]);
		material->material_Diffuse.w = 1.0f;

		// Specular Data
		material->material_Specular.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[0]);
		material->material_Specular.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[1]);
		material->material_Specular.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[2]);
		material->material_Specular.w = 1.0f;

		// Emissive Data
		material->material_Emissive.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[0]);
		material->material_Emissive.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[1]);
		material->material_Emissive.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[2]);
		material->material_Emissive.w = 1.0f;

		// Emissive Strength
		material->emissionFactor = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->EmissiveFactor);

		// Transparecy Data
		material->material_Transparency = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->TransparencyFactor.Get());

		// Shininess Data
		material->roughness = material->roughness = 1.0f - float(sqrt(fmax(static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Shininess.Get()), 0.0)) / 10.0);

		// metallic 값인듯함?
		material->metallic = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->ReflectionFactor.Get());

		// Reflectivity Data 일단...
		material->material_Reflectivity = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->ReflectionFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Ambient Data
		material->material_Ambient.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[0]);
		material->material_Ambient.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[1]);
		material->material_Ambient.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[2]);
		material->material_Ambient.w = 1.0f;

		// Diffuse Data
		material->material_Diffuse.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[0]);
		material->material_Diffuse.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[1]);
		material->material_Diffuse.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[2]);
		material->material_Diffuse.w = 1.0f;

		// Emissive Data
		material->material_Emissive.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[0]);
		material->material_Emissive.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[1]);
		material->material_Emissive.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[2]);
		material->material_Emissive.w = 1.0f;

		// Emissive Strength
		material->emissionFactor = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->EmissiveFactor);

		// Transparecy Data
		material->material_Transparency = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->TransparencyFactor.Get());
	}

	material->albedoMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
	material->normalMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
	material->roughnessMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sShininess);
	material->emissiveMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sEmissive);

	// 머터리얼 리스트에 추가
	fbxModel->materialList.push_back(material);
}

void FBXParser::LoadAnimation()
{
	// animNames Array안에 모든 애니메이션 이름이 담긴다.
	scene->FillAnimStackNameArray(OUT animNames);

	const int animCount = animNames.GetCount();

	for (int i = 0; i < animCount; i++)
	{
		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animNames[i]->Buffer());

		if (animStack == nullptr)
			continue;

		std::shared_ptr<FBXAnimClipInfo> animClip = std::make_shared<FBXAnimClipInfo>();
		animClip->animationName = animStack->GetName();						// 애니메이션 이름

		// 시작시간, 종료시간, 초당 프레임에 대한 정보
		FbxTakeInfo* takeInfo = scene->GetTakeInfo(animStack->GetName());
		double startTime = takeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		double endTime = takeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
		double frameRate = (float)FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());

		animClip->frameRate = frameRate;

		if (startTime < endTime)
		{
			animClip->totalKeyFrame = (int)((endTime - startTime) * (double)frameRate);
			animClip->endKeyFrame = (int)((endTime - startTime) * (double)frameRate);
			animClip->tickPerFrame = (endTime - startTime) / animClip->totalKeyFrame;
			animClip->startKeyFrame = (int)(startTime)*animClip->totalKeyFrame;
		}

		fbxModel->animationClipList.push_back(animClip);
	}
}

void FBXParser::ProcessAnimationData(FbxNode* node)
{
	const int animCount = fbxModel->animationClipList.size();

	for (int animIdx = 0; animIdx < animCount; animIdx++)
	{
		std::vector<std::shared_ptr<FBXKeyFrameInfo>> keyFrameList;
		
		std::shared_ptr<FBXKeyFrameInfo> fbxKeyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

		FbxTime::EMode timeMode = scene->GetGlobalSettings().GetTimeMode();

		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animNames[animIdx]->Buffer());
		scene->SetCurrentAnimationStack(OUT animStack);

		for (FbxLongLong frame = 0; frame < fbxModel->animationClipList[animIdx]->totalKeyFrame; frame++)
		{
			std::shared_ptr<FBXKeyFrameInfo> keyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

			FbxTime fbxTime = 0;

			fbxTime.SetFrame(frame, timeMode);

			// Local Transform = 부모 Bone의 Global Transform의 inverse Transform * 자신 Bone의 Global Transform;
			FbxAMatrix localTransform = node->EvaluateGlobalTransform(fbxTime);

			DirectX::SimpleMath::Matrix localTM;

			if (FbxNode* parent = node->GetParent())
			{
				FbxNodeAttribute* ParentAttribute = parent->GetNodeAttribute();

				if (ParentAttribute && ParentAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
				{
					FbxAMatrix GlobalParentTransform = parent->EvaluateGlobalTransform(fbxTime);

					localTransform = GlobalParentTransform.Inverse() * localTransform;

					localTM = ConvertMatrix(localTransform);
				}
				else
				{
					localTM = ConvertMatrix(localTransform);

					const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

					const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

					DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

					localTM *= XMMatrixRotationQuaternion(q);
				}
			}

			DirectX::XMVECTOR localScale;
			DirectX::XMVECTOR localRot;
			DirectX::XMVECTOR localPos;
			XMMatrixDecompose(&localScale, &localRot, &localPos, localTM);

			keyFrameInfo->time = fbxTime.GetSecondDouble();

			keyFrameInfo->localTransform = DirectX::SimpleMath::Vector3(localPos);
			keyFrameInfo->localRotation = DirectX::SimpleMath::Quaternion(localRot);
			keyFrameInfo->localScale = DirectX::SimpleMath::Vector3(localScale);

			keyFrameList.push_back(keyFrameInfo);
		}

		fbxModel->animationClipList[animIdx]->keyFrameList.push_back(keyFrameList);
	}
}

int FBXParser::FindBoneIndex(std::string boneName)
{
	for (int i = 0; i < fbxModel->fbxBoneInfoList.size(); ++i)
	{
		if (fbxModel->fbxBoneInfoList[i]->boneName == boneName)
			return i;
	}

	return -1;
}

DirectX::SimpleMath::Matrix FBXParser::GetNodeTM(fbxsdk::FbxNode* node)
{
	FbxMatrix localpos = scene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

	DirectX::SimpleMath::Matrix localTM = ConvertMatrix(localpos);

	return localTM;
}

DirectX::SimpleMath::Vector3 FBXParser::GetNormal(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	DirectX::SimpleMath::Vector3 fbxNormal;

	if (mesh->GetElementNormalCount() >= 1)
	{
		FbxGeometryElementNormal* normal = mesh->GetElementNormal();
		int normalIdx = 0;

		// 인덱스를 기준으로 노멀 값이 들어간다
		// 버텍스 스플릿이 필요하다.
		if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
				normalIdx = vertexCounter;
			else
				normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
		}
		// 정점을 기준으로 노멀 값이 들어간다.
		else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
				normalIdx = controlPointIndex;
			else
				normalIdx = normal->GetIndexArray().GetAt(controlPointIndex);
		}

		FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);

		if (isNegativeScale)
		{
			fbxNormal.x = static_cast<float>(-vec.mData[0]);
			fbxNormal.y = static_cast<float>(-vec.mData[2]);
			fbxNormal.z = static_cast<float>(-vec.mData[1]);
		}
		else
		{
			fbxNormal.x = static_cast<float>(vec.mData[0]);
			fbxNormal.y = static_cast<float>(vec.mData[2]);
			fbxNormal.z = static_cast<float>(vec.mData[1]);
		}

		return fbxNormal;
	}

	fbxNormal = { -1.f, -1.f, -1.f };

	return fbxNormal;
}

void FBXParser::GetTangent(std::shared_ptr<FBXMeshInfo>& meshInfo, int meshCnt)
{
	for (int i = 0; i < meshInfo->indices[meshCnt].size(); i += 3) // 삼각형의 개수
	{
		int i0 = meshInfo->indices[meshCnt][i];
		int i1 = meshInfo->indices[meshCnt][i + 1];
		int i2 = meshInfo->indices[meshCnt][i + 2];

		// e1 = p1 - p0, e2 = p2 - p0
		DirectX::SimpleMath::Vector3 e1 = meshInfo->meshVertexList[i1].position - meshInfo->meshVertexList[i0].position;
		DirectX::SimpleMath::Vector3 e2 = meshInfo->meshVertexList[i2].position - meshInfo->meshVertexList[i0].position;

		float x1 = meshInfo->meshVertexList[i1].uv.x - meshInfo->meshVertexList[i0].uv.x; // u1 - u0
		float y1 = meshInfo->meshVertexList[i1].uv.y - meshInfo->meshVertexList[i0].uv.y; // v1 - v0
		float x2 = meshInfo->meshVertexList[i2].uv.y - meshInfo->meshVertexList[i0].uv.x; // u2 - u0
		float y2 = meshInfo->meshVertexList[i2].uv.y - meshInfo->meshVertexList[i0].uv.y; // v2 - v0

		float det = (x1 * y2) - (x2 * y1); // ad - bc
		if (det == 0.f) { det = 0.00001f; }
		float r = 1.0f / det;

		DirectX::SimpleMath::Vector3 tangent = (e1 * y2 - e2 * y1) * r;

		meshInfo->meshVertexList[i0].tangent += tangent;
		meshInfo->meshVertexList[i1].tangent += tangent;
		meshInfo->meshVertexList[i2].tangent += tangent;
	}
}

DirectX::SimpleMath::Vector2 FBXParser::GetUV(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	DirectX::SimpleMath::Vector2 fbxUV;

	if (mesh->GetElementUVCount() >= 1)
	{
		int uvIndex;

		FbxGeometryElementUV* uv = mesh->GetLayer(0)->GetUVs();

		if (uv->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (uv->GetReferenceMode() == FbxGeometryElement::eDirect)
				uvIndex = vertexCounter;
			else
				uvIndex = uv->GetIndexArray().GetAt(vertexCounter);
		}
		else if (uv->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (uv->GetReferenceMode() == FbxGeometryElement::eDirect)
				uvIndex = controlPointIndex;
			else
				uvIndex = uv->GetIndexArray().GetAt(controlPointIndex);
		}

		fbxUV.x = static_cast<float>(uv->GetDirectArray().GetAt(uvIndex).mData[0]);
		fbxUV.y = 1.f - static_cast<float>(uv->GetDirectArray().GetAt(uvIndex).mData[1]);

		if (fbxUV.x < 0)
			fbxUV.x += 1;

		return fbxUV;
	}

	fbxUV = { 0.f, 0.f };

	return fbxUV;
}

std::string FBXParser::GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty)
{
	std::string name;

	fbxsdk::FbxProperty textureProperty = surface->FindProperty(materialProperty);

	if (textureProperty.IsValid())
	{
		int count = textureProperty.GetSrcObjectCount();

		if (1 <= count)
		{
			FbxFileTexture* texture = textureProperty.GetSrcObject<FbxFileTexture>(0);
			if (texture)
				name = texture->GetRelativeFileName();
		}
	}

	std::wstring wstr = L"";

	wstr.assign(name.begin(), name.end());

	std::wstring tempName = fs::path(name).filename();

	const std::string fileName(tempName.begin(), tempName.end());

	return fileName;
}

FbxAMatrix FBXParser::GetTransformMatrix(FbxNode* node)
{
	const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 scaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(translation, rotation, scaling);
}
