#include "GraphicsPch.h"
#include "Animator.h"

#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	Animator::Animator(int boneSize)
		: nowAnimClip(nullptr)
		, nowKeyFrameIdx(0)
		, isLoop(true)
		, isPlay(true)
		, nowFrameTime(0.f)
	{
		// 해당 obj가 가지고 있는 본 크기 만큼의 Matrix vector를 생성해준다.
		modifiedBoneTM.assign(boneSize, SimpleMath::Matrix::Identity);
	}

	Animator::~Animator()
	{
	}

	std::vector<SimpleMath::Matrix> Animator::UpdateBoneTM(float tick)
	{
		// 내부적으로 벡터를 가지고 있어야 해당 벡터의 값이 사라지지 않고 전달 된다.
		// 계산, 보간 및 Matrix 생성후 추가해준다.

		// 일단 가장 간단한 Animation을 구현해본다.
			// Frame Rate나 TickPerFrame 등은 아직 구현하지 않는다.

		// 기본적으로 애니메이션은
			// 현재 FrameTime에 delta Tick을 계속 더해주면서
			// FrameTime 보다 작은 prev KeyFrame과
			// FrameTime 보다 큰 next KeyFrame을 보간해서 현재 TM을 만들어낸다.

		// 재생 여부 파악
		if (isPlay != true || nowAnimClip == nullptr)
			return modifiedBoneTM;

		KeyFrame nowKeyFrame;
		KeyFrame nextKeyFrame;

		// 마지막 키프레임 체크, 루프 체크
		if (nowKeyFrameIdx == nowAnimClip->endKeyFrame)
		{
			if (isLoop == true)
				nowKeyFrameIdx = nowAnimClip->startKeyFrame;
			else
			{
				isPlay = false;	
				return modifiedBoneTM;	// 마지막 프레임에서 멈춤.
			}
		}

		// 애니메이션 클립의 모든 키프레임을 돌면서..
			// cf)AnimationClip의 구조
			// 내부에 KeyFrames가 핵심
			// n번본의 m번째 Frame이 담겨있음.
		// 즉 모든 본의 정보를 추가해야 하기 때문에.. 모든 본을 돌아줘야함
		for (size_t i = 0; i < modifiedBoneTM.size(); i++)
		{
			// 애니메이션 클립의 해당 키프레임에 접근
			nowKeyFrame = nowAnimClip->keyFrames[i][nowKeyFrameIdx];
			// 마지막 프레임 체크
			if(nowKeyFrameIdx + 1 == nowAnimClip->endKeyFrame)
			{
				if(isLoop == true)
					nextKeyFrame = nowAnimClip->keyFrames[i][nowAnimClip->startKeyFrame];
				else
					nextKeyFrame = nowAnimClip->keyFrames[i][nowKeyFrameIdx];
			}

			// TODO 보간 후 Matrix를 만들어서 넣어준다.
			// (SRT)
			SimpleMath::Matrix nowTM = SimpleMath::Matrix::CreateScale(nowKeyFrame.frameScale)
			* SimpleMath::Matrix::CreateFromQuaternion(nowKeyFrame.frameRot)
			* SimpleMath::Matrix::CreateTranslation(nowKeyFrame.framePos);

			// i번째 본에 해당 TM을 넣어준다.
			modifiedBoneTM[i] = nowTM;
		}

		// KeyFrame TimeCheck (위에서 next KeyFrame이 나왔다고 가정.)
			// 다음 keyFrame의 Time보다 nowFrameTime이 크다면.
			// 다음 키프레임으로 넘겨준다.
		if(nextKeyFrame.time < nowFrameTime)
			nowKeyFrameIdx++;

		nowFrameTime += tick;

		// 현재 프레임 타임이 최총 프레임 타임을 넘어가면. 최초로 되돌려준다.
		int endKeyFrameIdx = nowAnimClip->endKeyFrame - 1;
		if(nowFrameTime > nowAnimClip->keyFrames[0][endKeyFrameIdx].time)
		{
			nowFrameTime = nowAnimClip->keyFrames[0][nowAnimClip->startKeyFrame].time;
		}

		// 마지막으로 해당 TMVector를 리턴해준다.
		return modifiedBoneTM;
	}

	void Animator::SetAnimationClip(std::shared_ptr<AnimationClipResource> animClip, bool isLoop)
	{
		nowAnimClip = animClip;
		// 첫 키 프레임 기록.
		nowKeyFrameIdx = nowAnimClip->startKeyFrame;

		this->isLoop = isLoop;
	}
}
