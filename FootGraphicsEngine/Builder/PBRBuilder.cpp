#include "GraphicsPch.h"
#include "PBRBuilder.h"

#include "Object/PBRObj.h"
#include "GraphicsCore/RasterizerState.h"

#include "Manager/ResourceManager.h"
#include <typeinfo>

namespace GraphicsEngineSpace
{
	PBRBuilder::~PBRBuilder()
	{
	}

	std::shared_ptr<IDXObject> PBRBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// 들어온 오브젝트 확인
		if (std::dynamic_pointer_cast<PBRObj>(pDXObj) == nullptr)
		{
			OutputDebugString(L"Not PB OBJ");

			return nullptr;
		}

		// 먼저 basicMeshes에 있는지 찾아본다.
		auto findIt = resourceManager->basicMeshes.find(objectName);

		if (findIt != resourceManager->basicMeshes.end())
		{
			std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();
			auto mesh = resourceManager->basicMeshes[objectName];
			_tmpObjRes->ObjName = mesh->GetName();
			pDXObj->SetMeshResources(_tmpObjRes);

			return pDXObj;
		}

		// basicMeshes에 없다면 외부에서 불러온다.? 근데 PBRObj 를.. 파일로 저장할수있을까?
		// 애초에 게임오브젝트로 프리팹화해서 파일로 만들 수 있다면 좋았겠지만.. 그렇다면? 일단은 만들어서 넘겨준다치고

		// pbrobj를 만들어보자 근데 그냥 이거 함수쓰면 되지않나? 그렇네
		BuildGeometry(pDXObj, objectName);

		return pDXObj;
	}

	std::shared_ptr<MeshResources> PBRBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject,
		std::string objectPath)
	{
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

		_tmpObjRes->ObjName = objectPath;

		// 오브젝트 이름을 판별합니다.
		std::string extension = objectPath;
		int _extensionIdx = objectPath.rfind(".");
		if (_extensionIdx != -1)
		{
			extension = objectPath.substr(_extensionIdx);
		}

		if (extension == ".model")
		{
			// 모델을 불러오자 일단 path.. 임시
			std::ifstream ifs("BinaryFile/Model/" + objectPath, std::ios_base::binary);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
			buffer.push(boost::iostreams::zlib_decompressor());
			buffer.push(ifs);
			boost::archive::binary_iarchive ia(buffer);

			FBXBinaryData::ModelData modelData;

			ia >> modelData;

			_tmpObjRes.reset();

			// 애니메이션이 없으면, 정적 메시 생성
			if (modelData.isSkinnedAnimation != true)
			{
				// 메시가 여러개일 가능성도 있기 때문에 내부에서 다 만들어준다.
				BuildStaticMeshBinaryResources(DXObject, modelData);
				return nullptr;
			}
			// 있다면 skinned Mesh 생성
			else
			{
				BuildSkinnedMeshBinaryResources(DXObject, modelData);
				return nullptr;
			}
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}
	}

	void PBRBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 리소스 매니저 캐싱
		resourceManager = ResourceManager::GetInstance();
	}

	void PBRBuilder::BuildStaticMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
		// 모든 메쉬를 돌면서 버텍스버퍼와 인덱스 버퍼를 만들어줍니다.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			objRes->setMesh = true;

			objRes->setMaterial = true;

			objRes->ObjName = mesh;

			DXObject->SetMeshResources(objRes);
		}
	}

	void PBRBuilder::BuildSkinnedMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
		// 모든 메쉬를 돌면서 메시 이름과 isSkinnedMesh를 넣어준다.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			objRes->isSkinnedMesh = true;

			objRes->setMesh = true;

			objRes->setMaterial = true;

			objRes->ObjName = mesh;

			DXObject->SetMeshResources(objRes);
		}

		// skinned 기 때문에 bone이 존재
		std::shared_ptr<BoneResources> boneRes = std::make_shared<BoneResources>();

		for (size_t i = 0; i < modelData.boneInfoList.size(); i++)
		{
			SimpleMath::Matrix boneOffset;
			SimpleMath::Matrix node;

			auto& bone = modelData.boneInfoList[i];

			boneOffset.m[0][0] = bone.offsetMatrix.m[0][0];
			boneOffset.m[0][1] = bone.offsetMatrix.m[0][1];
			boneOffset.m[0][2] = bone.offsetMatrix.m[0][2];
			boneOffset.m[0][3] = bone.offsetMatrix.m[0][3];

			boneOffset.m[1][0] = bone.offsetMatrix.m[1][0];
			boneOffset.m[1][1] = bone.offsetMatrix.m[1][1];
			boneOffset.m[1][2] = bone.offsetMatrix.m[1][2];
			boneOffset.m[1][3] = bone.offsetMatrix.m[1][3];

			boneOffset.m[2][0] = bone.offsetMatrix.m[2][0];
			boneOffset.m[2][1] = bone.offsetMatrix.m[2][1];
			boneOffset.m[2][2] = bone.offsetMatrix.m[2][2];
			boneOffset.m[2][3] = bone.offsetMatrix.m[2][3];

			boneOffset.m[3][0] = bone.offsetMatrix.m[3][0];
			boneOffset.m[3][1] = bone.offsetMatrix.m[3][1];
			boneOffset.m[3][2] = bone.offsetMatrix.m[3][2];
			boneOffset.m[3][3] = bone.offsetMatrix.m[3][3];

			node.m[0][0] = bone.nodeMatrix.m[0][0];
			node.m[0][1] = bone.nodeMatrix.m[0][1];
			node.m[0][2] = bone.nodeMatrix.m[0][2];
			node.m[0][3] = bone.nodeMatrix.m[0][3];

			node.m[1][0] = bone.nodeMatrix.m[1][0];
			node.m[1][1] = bone.nodeMatrix.m[1][1];
			node.m[1][2] = bone.nodeMatrix.m[1][2];
			node.m[1][3] = bone.nodeMatrix.m[1][3];

			node.m[2][0] = bone.nodeMatrix.m[2][0];
			node.m[2][1] = bone.nodeMatrix.m[2][1];
			node.m[2][2] = bone.nodeMatrix.m[2][2];
			node.m[2][3] = bone.nodeMatrix.m[2][3];

			node.m[3][0] = bone.nodeMatrix.m[3][0];
			node.m[3][1] = bone.nodeMatrix.m[3][1];
			node.m[3][2] = bone.nodeMatrix.m[3][2];
			node.m[3][3] = bone.nodeMatrix.m[3][3];
			

			// 초기 월드 값으로 바꿔준다.
			//boneOffset = boneOffset * node;

			// 해당 본의 offest, node, parentIdx를 넣어준다
			Bone tmpBone;
			tmpBone.offsetTransform = boneOffset;
			tmpBone.nodeTransform = node;
			if(i == 0)
				tmpBone.parentIdx = -1;
			else
				tmpBone.parentIdx = bone.parentIndex;

			boneRes->bones.emplace_back(tmpBone);
		}

		// 모든 본을 넣어준 뒤 해당 resource를 PBRObj에 넣어준다.
		DXObject->SetBoneResource(boneRes);

		// Animation 세팅
		std::shared_ptr<AnimationResources> animRes = std::make_shared<AnimationResources>();

		// 모델 데이터의 모든 Animation Clip을 넣는다..
		for (auto& animClipName : modelData.animationClipList)
		{
			// 애니메이션 파일 불러오기
			FBXBinaryData::AnimationClipData animClip;

			std::ifstream ifs("BinaryFile/Animation/" + animClipName + ".anim", std::ios_base::binary);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
			buffer.push(boost::iostreams::zlib_decompressor());
			buffer.push(ifs);
			boost::archive::binary_iarchive iaAnim(buffer);
			iaAnim >> animClip;

			// 들어갈 AnimClip 생성
			std::shared_ptr<AnimationClipResource> animClipRes = std::make_shared<AnimationClipResource>();

			// 내부의 내용을 채워준다.
			animClipRes->animationName = animClip.animationName;
			animClipRes->frameRate = animClip.frameRate;
			animClipRes->startKeyFrame = animClip.startKeyFrame;
			animClipRes->totalKeyFrame = animClip.totalKeyFrame;
			animClipRes->endKeyFrame = animClip.endKeyFrame;
			animClipRes->tickPerFrame = animClip.tickPerFrame;

			// 각각의 KeyFrame을 추가해준다.
				// 각각의 Bone이 가지고 있는 모든 Keyframe 데이터를 넣어준다.
			for (size_t i = 0; i < modelData.boneInfoList.size(); i++)
			{
				std::vector<KeyFrame> boneKeyFrames;

				for (auto& keyFrame : animClip.keyFrameList[i])
				{
					KeyFrame keyFrameRes;
					
					keyFrameRes.framePos.x = keyFrame.localTransform.x;
					keyFrameRes.framePos.y = keyFrame.localTransform.y;
					keyFrameRes.framePos.z = keyFrame.localTransform.z;

					keyFrameRes.frameRot.x = keyFrame.localRotation.x;
					keyFrameRes.frameRot.y = keyFrame.localRotation.y;
					keyFrameRes.frameRot.z = keyFrame.localRotation.z;
					keyFrameRes.frameRot.w = keyFrame.localRotation.w;

					keyFrameRes.frameScale.x = keyFrame.localScale.x;
					keyFrameRes.frameScale.y = keyFrame.localScale.y;
					keyFrameRes.frameScale.z = keyFrame.localScale.z;

					keyFrameRes.time = keyFrame.time;

					// 각각의 keyFrame 목록을 벡터로 만들어 주고
					boneKeyFrames.emplace_back(keyFrameRes);
				}

				// 모든 keyFrame이 벡터로 들어갔을때. 해당 벡터를 넣어준다.
				animClipRes->keyFrames.emplace_back(boneKeyFrames);
			}

			animRes->animationClipList.insert(std::make_pair(animClipRes->animationName, animClipRes));
		}

		// 모든 애니메이션을 세팅한 뒤 해당 리소스를 PBRObj에 넣어준다.
		DXObject->SetAnimationResource(animRes);
	}
}

