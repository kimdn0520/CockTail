#include "GraphicsPch.h"
#include "Animator.h"

#include "Resources/ObjectResources.h"

#include "Util/AnimatorState.h"
#include "Util/AnimatorParmeter.h"
#include "Util/AnimatorTransition.h"

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

		blendBoneTM.assign(boneSize, SimpleMath::Matrix::Identity);

		// Parameter 생성
		animatorParameter = std::make_shared<AnimatorParmeter>();

		// Any State 생성
		//std::shared_ptr<AnimatorState> anyState
	}

	Animator::~Animator()
	{
		if (animResources != nullptr)
		{
			for (auto animClip : animResources->animationClipList)
			{
				SafeReset(animClip.second);
			}
		}

		SafeReset(animResources);
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

		// 김재우 02-20 시작
		// Aniamtor 제작 & Aniamtion blending

		// state가 전환이 되었다면
		if (currentStateName.compare(currentAnimatorState->GetStateName()) != 0)
		{
			// transitionDuration 에 따라 blending 을 한다.
			if (currentTransitionTime > 0.0f)
			{
				// 상태 전환 시간을 빼줌
				currentTransitionTime -= tick;

				// blending 을 해보자..
				const float blendFactor = 1.0f - (currentTransitionTime / currentTransitionDuration);

				// 이전의 bone 행렬
				auto curBoneTM = modifiedBoneTM;
				
				KeyFrame nextStateStartFrame;

				nowAnimClip = animResources->animationClipList[states[currentStateName]->GetMotion()];

				for (size_t i = 0; i < blendBoneTM.size(); i++)
				{
					// 애니메이션 클립의 해당 키프레임에 접근
					nextStateStartFrame = nowAnimClip->keyFrames[i][0];

					// (SRT)
					SimpleMath::Vector3 pos = nextStateStartFrame.framePos;
					SimpleMath::Quaternion rot = nextStateStartFrame.frameRot;
					SimpleMath::Vector3 scale = nextStateStartFrame.frameScale;

					SimpleMath::Matrix nextStateStartFrameTM = SimpleMath::Matrix::CreateScale(scale)
														* SimpleMath::Matrix::CreateFromQuaternion(rot)
														* SimpleMath::Matrix::CreateTranslation(pos);

					blendBoneTM[i] = curBoneTM[i] * (1.0f - blendFactor) + nextStateStartFrameTM * blendFactor;
				}

				return blendBoneTM;
			}
			else
			{
				// 현재 상태를 갱신해준다.
				currentAnimatorState = states[currentStateName];

				nowKeyFrameIdx = 0;

				currentTransitionTime = 0.0f;

				// 블렌딩 후 다음 애니메이션 바로 return 방지
				nowFrameTime = 100.0f;
				
				return UpdateFrame(tick);
			}
		}
		else
		{	
			currentStateName = currentAnimatorState->UpdateState(animatorParameter);

			if (currentAnimatorState->operateTransition != nullptr)
			{
				currentTransitionTime = currentAnimatorState->operateTransition->GetTransitionDuration();

				currentTransitionDuration = currentTransitionTime;

				currentAnimatorState->operateTransition = nullptr;
			}

			return UpdateFrame(tick);
		}
	}

	std::vector<SimpleMath::Matrix> Animator::UpdateFrame(float tick)
	{
		nowAnimClip = animResources->animationClipList[currentAnimatorState->GetMotion()];

		isLoop = currentAnimatorState->GetLoop();

		// 재생 여부 파악
		if (isPlay != true || nowAnimClip == nullptr)
			return modifiedBoneTM;

		nowFrameTime += tick;

		// 블렌딩 후 여기로 들어가서 idle->run 할때 잠깐 idle 반짝하는 이유..
		if (nowFrameTime < 1.0f / nowAnimClip->frameRate)
			return modifiedBoneTM;

		nowFrameTime = 0.f;

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

		// 보간이 필요하다.
		float currentTick = nowKeyFrameIdx * nowAnimClip->tickPerFrame;
		float t = 0.f;

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
			if (nowKeyFrameIdx + 1 == nowAnimClip->endKeyFrame)
			{
				if (isLoop == true)
					nextKeyFrame = nowAnimClip->keyFrames[i][nowAnimClip->startKeyFrame];
				else
					nextKeyFrame = nowAnimClip->keyFrames[i][nowKeyFrameIdx];
			}
			else
				nextKeyFrame = nowAnimClip->keyFrames[i][nowKeyFrameIdx + 1];

			// TODO 보간 후 Matrix를 만들어서 넣어준다.
			t = (currentTick - nowKeyFrame.time) / (nextKeyFrame.time - nowKeyFrame.time);

			// (SRT)
			SimpleMath::Vector3 pos = XMVectorLerp(nowKeyFrame.framePos, nextKeyFrame.framePos, t);
			SimpleMath::Quaternion rot = XMQuaternionSlerp(nowKeyFrame.frameRot, nextKeyFrame.frameRot, t);
			SimpleMath::Vector3 scale = XMVectorLerp(nowKeyFrame.frameScale, nextKeyFrame.frameScale, t);

			SimpleMath::Matrix nowTM = SimpleMath::Matrix::CreateScale(scale)
				* SimpleMath::Matrix::CreateFromQuaternion(rot)
				* SimpleMath::Matrix::CreateTranslation(pos);
			/*
			SimpleMath::Matrix nowTM = SimpleMath::Matrix::CreateScale(nowKeyFrame.frameScale)
			* SimpleMath::Matrix::CreateFromQuaternion(nowKeyFrame.frameRot)
			* SimpleMath::Matrix::CreateTranslation(nowKeyFrame.framePos);
			 */

			 // i번째 본에 해당 TM을 넣어준다.
			modifiedBoneTM[i] = nowTM;
		}

		// KeyFrame TimeCheck (위에서 next KeyFrame이 나왔다고 가정.)
			// 다음 keyFrame의 Time보다 nowFrameTime이 크다면.
			// 다음 키프레임으로 넘겨준다.
		nowKeyFrameIdx++;

		return modifiedBoneTM;
	}

	void Animator::SetAnimationClip(std::shared_ptr<AnimationClipResource> animClip, bool isLoop)
	{
		nowAnimClip = animClip;
		// 첫 키 프레임 기록.
		nowKeyFrameIdx = nowAnimClip->startKeyFrame;

		this->isLoop = isLoop;
	}

	int Animator::GetTotalFrame()
	{
		return nowAnimClip->totalKeyFrame;
	}


	void Animator::SetAnimatorState(const std::string& stateName, std::shared_ptr<AnimatorState> animatorState)
	{
		states.insert(std::make_pair(stateName, animatorState));
	}

	void Animator::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, float val)
	{
		std::string delimiter = "->";
		std::string stateName = transitionName.substr(0, transitionName.find(delimiter));

		Condition condition;
		condition.parameterName = parameterName;
		condition.floatVal = val;
		
		if(conditionName == "Greater")
			condition.type = ConditionType::Greater;
		else if(conditionName == "Less")
			condition.type = ConditionType::Less;

		states[stateName]->GetTransition(transitionName)->AddCondition(condition);
	}

	void Animator::EntryAnimatorState(const std::string& name)
	{
		currentStateName = name;

		currentAnimatorState = states[name];
	}

	void Animator::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, int val)
	{
		std::string delimiter = "->";
		std::string stateName = transitionName.substr(0, transitionName.find(delimiter));

		Condition condition;
		condition.parameterName = parameterName;
		condition.intVal = val;

		if (conditionName == "Greater")
			condition.type = ConditionType::Greater;
		else if (conditionName == "Less")
			condition.type = ConditionType::Less;
		else if(conditionName == "Equlals")
			condition.type = ConditionType::Equals;
		else if(conditionName == "NotEqual")
			condition.type = ConditionType::NotEqual;

		states[stateName]->GetTransition(transitionName)->AddCondition(condition);
	}

	void Animator::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName, bool val)
	{
		std::string delimiter = "->";
		std::string stateName = transitionName.substr(0, transitionName.find(delimiter));

		Condition condition;
		condition.parameterName = parameterName;
		condition.intVal = val;
		condition.type = ConditionType::None;

		states[stateName]->GetTransition(transitionName)->AddCondition(condition);
	}

	void Animator::AddCondition(const std::string& transitionName, const std::string& parameterName, const std::string& conditionName)
	{
		std::string delimiter = "->";
		std::string stateName = transitionName.substr(0, transitionName.find(delimiter));

		Condition condition;
		condition.parameterName = parameterName;
		condition.type = ConditionType::None;

		states[stateName]->GetTransition(transitionName)->AddCondition(condition);
	}

	void Animator::AddFloat(const std::string& name, float val)
	{
		animatorParameter->AddFloat(name, val);
	}

	void Animator::AddInteger(const std::string& name, int val)
	{
		animatorParameter->AddInteger(name, val);
	}

	void Animator::AddBool(const std::string& name, bool val)
	{
		animatorParameter->AddBool(name, val);
	}

	void Animator::AddTrigger(const std::string& name)
	{
		animatorParameter->AddTrigger(name);
	}

	void Animator::SetFloat(const std::string& name, float val)
	{
		animatorParameter->SetFloat(name, val);
	}

	void Animator::SetInteger(const std::string& name, int val)
	{
		animatorParameter->SetInteger(name, val);
	}

	void Animator::SetBool(const std::string& name, bool val)
	{
		animatorParameter->SetBool(name, val);
	}

	void Animator::SetTrigger(const std::string& name)
	{
		animatorParameter->SetTrigger(name);
	}
}
