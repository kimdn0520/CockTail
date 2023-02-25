#include "GraphicsPch.h"
#include "AnimatorTransition.h"
#include "AnimatorParmeter.h"

namespace GraphicsEngineSpace
{
	AnimatorTransition::AnimatorTransition()
		: transitionState("")
		, animationStateStart("")
		, animationStateEnd("")
		, hasExitTime(false)
		, exitTime(0.f)
		, transitionDuration(0.f)
	{}

	AnimatorTransition::~AnimatorTransition()
	{}

	void AnimatorTransition::SetTransitionState(std::string stateStart, std::string stateEnd)
	{
		transitionState = stateStart + "->" + stateEnd;
	}

	bool AnimatorTransition::CheckCondition(std::shared_ptr<AnimatorParmeter>& parameter)
	{
		for (const auto& condition : conditions)
		{
			auto& param = parameter->parameters[condition.parameterName];

			switch (param.parameterType)
			{
			case ParameterType::Int:
			{
				if (condition.type == ConditionType::Equals && param.intValue == condition.intVal)
					continue;
				else if (condition.type == ConditionType::NotEqual && param.intValue != condition.intVal)
					continue;
				else if (condition.type == ConditionType::Greater && param.intValue > condition.intVal)
					continue;
				else if (condition.type == ConditionType::Less && param.intValue < condition.intVal)
					continue;
				else
					return false;
			}
			break;
			case ParameterType::Float:
			{
				if (condition.type == ConditionType::Greater && param.floatValue > condition.floatVal)
					continue;
				else if (condition.type == ConditionType::Less && param.floatValue < condition.floatVal)
					continue;
				else
					return false;
			}
			break;
			case ParameterType::Bool:
			{
				if (param.boolValue == condition.boolVal)
					continue;
				else if (param.boolValue != condition.boolVal)
					continue;
				else
					return false;
			}
			break;
			case ParameterType::Trigger:
			{
				if (param.isTrigger)
				{
					param.isTrigger = false;
					continue;
				}
				else
					return false;
			}
			break;
			default:
				break;
			}
		}

		return true;
	}
}