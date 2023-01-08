#pragma once

namespace GraphicsEngineSpace
{
	class KeyFrame;
	class AnimationClipResource;

	// 현재 애니메이션 클립을 가지고 있고,
	// Tick을 받아 현재 프레임에 각각의 본이 가져야할 Bone TM을 계산해주는 Helper Class
	// 2022.12.22 Dev.BlackFoot
	class Animator
	{
	private:
		// 현재 AnimClip
		std::shared_ptr<AnimationClipResource> nowAnimClip;

		// 현재 키 프레임의 index정보를 가지고 있을 변수
		int nowKeyFrameIdx;

		// 무한 반복 애니메이션으로 할 것인지 여부.
		bool isLoop;

		// 재생 여부
		bool isPlay;

		// DeltaTick들을 더한 현재 프레임 타임
		float nowFrameTime;

		// 밖으로 보내줄 BoneTransForm
		std::vector<SimpleMath::Matrix> modifiedBoneTM;

	public:
		// BoneSize를 받아서 Vector를 초기화시켜 준다.
		Animator(int boneSize);
		virtual ~Animator();

		// DeltaTick을 받아 시간을 계산하면서 새로운 BoneTM을 만들어서 뽑아준다.
			// KeyFrame들을 Bone Idx별로 계산해서 보간한뒤 넣어준다..
		std::vector<SimpleMath::Matrix> UpdateBoneTM(float tick);

		// Object에서 현재의 animationClip을 세팅할 수 있게 해주는 함수
			// loop 여부까지 지정
		virtual void SetAnimationClip(std::shared_ptr<AnimationClipResource> animClip, bool isLoop);
		
	};

}