#pragma once
#include "Define/d3d11Utils.h"

namespace GraphicsEngineSpace
{
	class Mesh;

	class Texture;

	class RenderingMaterial;

	// 본 하나의 데이터
	struct Bone
	{
		SimpleMath::Matrix offsetTransform;
		SimpleMath::Matrix nodeTransform;
		int parentIdx;

		Bone()
			: offsetTransform(SimpleMath::Matrix::Identity)
			, nodeTransform(SimpleMath::Matrix::Identity)
			, parentIdx(-1)
		{
			
		}

		~Bone() = default;
	};

	// 스키닝 관련 본 데이터.
	struct BoneResources
	{
		bool isSkinnedObj;
		std::vector<Bone> bones;

		// 노드 TM을 이미 곱해줬는가..
		bool setNode;

		BoneResources()
			: isSkinnedObj(false)
			, setNode(false)
		{
			
		}

		~BoneResources() = default;
	};

	// 애니메이션에 들어가는 가장 기초적인 KeyFrame 구조
		// 각각의 본이 변경 되어야 하는 애니메이션 정보.
	struct KeyFrame
	{
		// Local 정보들.
		SimpleMath::Vector3 framePos;
		SimpleMath::Quaternion frameRot;
		SimpleMath::Vector3 frameScale;

		float time;

		KeyFrame()
			: framePos(0.f, 0.f, 0.f)
			, frameRot(0.f, 0.f, 0.f, 0.f)
			, frameScale(0.f, 0.f, 0.f)
			, time(0.f)
		{

		}

		~KeyFrame() = default;
	};

	// 애니메이션 클립 하나하나를 저장하는 Struct
	struct AnimationClipResource
	{
		std::string animationName;

		// 해당 클립의 모든 KeyFrame을 저장해둔 Vector
		// BoneResources에 들어가는 본 순서와 해당 리소스에 들어가는 키 프레임 순서가 일치한다.
		std::vector<std::vector<KeyFrame>> keyFrames;

		// 프레임 Rate
		float frameRate;

		// 애니메이션 첫 번째 프레임
		int startKeyFrame;

		// 애니메이션 총 프레임 수
		int totalKeyFrame;

		// 애니메이션 마지막 프레임
		int endKeyFrame;

		// 프레임당 몇 Tick이 지나가는지
		float tickPerFrame;

		AnimationClipResource()
			: animationName("NONE")
			, frameRate(0.f)
			, startKeyFrame(0)
			, totalKeyFrame(0)
			, endKeyFrame(0)
			, tickPerFrame(0.f)
		{
			
		}

		~AnimationClipResource() = default;
	};

	// 스키닝 관련 애니메이션 데이터
		// 해당 Object가 가지고 있는 모든 애니메이션 정보를 모아둔 Resources
		// Animation Clip들을 List로 가지고 있다.
		// (각 애니메이션 Clip은 이름 등의 정보를 토대로 Animator에서 변경 가능하도록 만든다.)
	struct AnimationResources
	{
		std::unordered_map<std::string, std::shared_ptr<AnimationClipResource>> animationClipList;

		AnimationResources()
		{
			
		}

		~AnimationResources() = default;
	};

	/**
	* \brief 오브젝트가 가지고 있는 각종 리소스들
	*
	* // 해당 내용은 빌더의 종류에 따라, 오브젝트의 종류에 따라. 빌더에서 세팅해서 오브젝트에 넣어준다.
	*
	* 2022.07.11(월) Dev.BlackFoot
	*/
	struct MeshResources
	{
		bool setMesh;

		bool setMaterial;

		std::shared_ptr<Mesh> mesh;

		std::vector<std::shared_ptr<RenderingMaterial>> material;	

		// 오브젝트 이름
		std::string ObjName;

		// 메시 자체의 world TM.. => 여기서 처리해준다.
		SimpleMath::Matrix nodeTM;

		// Skinned 정보 또한 여기서 가지고 있는다.
		bool isSkinnedMesh;

		MeshResources()
			: setMesh(false)
			, setMaterial(false)
			, ObjName("NULL")
			, nodeTM(SimpleMath::Matrix::Identity)
			, isSkinnedMesh(false)
		{}

		~MeshResources()
		{
			
		}
	};
}