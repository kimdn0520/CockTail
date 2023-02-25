#pragma once

namespace GraphicsEngineSpace
{
	class AnimatorParmeter;

	enum class ConditionType
	{
		Greater,
		Less,
		Equals,
		NotEqual,

		None,
	};

	struct Condition
	{
		std::string parameterName;  // 파라미터 이름

		ConditionType type;		// 조건 타입

		float floatVal;			// 조건 float 값

		int intVal;				// 조건 int 값

		bool boolVal;			// 조건 bool 값
	};

	class AnimatorTransition
	{
	private:
		// Transition State Name
		std::string transitionState;

		// State Start
		std::string	animationStateStart;

		// State End
		std::string	animationStateEnd;

		// 종료 시점을 활성화 하는 옵션
		// 활성화 시 -> 종료 시점이 존재하게 되어 전이의 조건을 만족해도 다음 상태로 전이하지 않게 된다.
		// 비활성화 시 -> 전이 조건을 만족하는 즉시 전이가 시작된다.
		bool hasExitTime;

		// hasExitTime이 활성화시에 설정할 수 있다. (종료 시점)
		float exitTime;

		// 트랜지션의 지속시간, 블렌딩 할때 쓰인다.
		// 현재 애니메이션 클립과 다음 애니메이션 클립을 섞어(블렌딩) 부드럽게 이어주는 역할을 한다.
		float transitionDuration;

		// 조건 리스트
		std::vector<Condition> conditions;

	public:
		AnimatorTransition();
		~AnimatorTransition();

	public:
		const std::string& GetTransitionName() { return transitionState; }

		const std::string& GetAnimationStateStartName() { return animationStateStart; }

		const std::string& GetAnimationStateEndName() { return animationStateEnd; }

		const float GetTransitionDuration() { return transitionDuration; }

		void SetTransitionState(std::string stateStart, std::string stateEnd);

		void SetAnimationStateStart(std::string _animationStateStart) { animationStateStart = _animationStateStart; }
	
		void SetAnimationStateEnd(std::string _animationStateEnd) { animationStateEnd = _animationStateEnd; }

		void SetHasExitTime(bool val) { hasExitTime = val; }

		void SetExitTime(float val) { exitTime = val; }

		void SetTransitionDuration(float val) { transitionDuration = val; }

		void AddCondition(Condition& condition) { conditions.push_back(condition); }

		bool CheckCondition(std::shared_ptr<AnimatorParmeter>& parameter);
	};
}

