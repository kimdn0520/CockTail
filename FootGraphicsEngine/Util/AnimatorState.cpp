#include "GraphicsPch.h"
#include "AnimatorState.h"
#include "AnimatorTransition.h"
#include "AnimatorParmeter.h"

namespace GraphicsEngineSpace
{
	AnimatorState::AnimatorState()
		: motion("")
		, speed(1.0f)
		, isLoop(false)
	{}

	AnimatorState::~AnimatorState()
	{}

	std::shared_ptr<AnimatorTransition> AnimatorState::GetTransition(const std::string& name)
	{
		auto it = find_if(transitions.begin(), transitions.end(), [&name](const std::shared_ptr<AnimatorTransition>& a) 
					{ return (a->GetTransitionName() == name); });
	
		return *it;
	}

	std::string AnimatorState::UpdateState(std::shared_ptr<AnimatorParmeter> parameter)
	{
		for (const auto& transition : transitions)
		{
			// State 상태가 바뀐다.
			if (transition->CheckCondition(parameter))
			{
				operateTransition = transition;

				return transition->GetAnimationStateEndName();
			}
		}

		return stateName;
	}
}