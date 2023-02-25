#include "GraphicsPch.h"
#include "AnimatorParmeter.h"

namespace GraphicsEngineSpace
{
	AnimatorParmeter::AnimatorParmeter()
	{}

	AnimatorParmeter::~AnimatorParmeter()
	{}

	void AnimatorParmeter::SetFloat(const std::string& name, float value)
	{
		parameters[name].floatValue = value;
	}

	void AnimatorParmeter::SetInteger(const std::string& name, int value)
	{
		parameters[name].intValue = value;
	}

	void AnimatorParmeter::SetBool(const std::string& name, bool value)
	{
		parameters[name].boolValue = value;
	}

	void AnimatorParmeter::SetTrigger(const std::string& name)
	{
		parameters[name].isTrigger = true;
	}

	void AnimatorParmeter::AddFloat(const std::string& name, float value)
	{
		parameters.insert({ name, { name, ParameterType::Float , value, 0, false, false } });
	}

	void AnimatorParmeter::AddInteger(const std::string& name, int value)
	{
		parameters.insert({ name, { name, ParameterType::Int, 0.0f, value, false, false } });
	}

	void AnimatorParmeter::AddBool(const std::string& name, bool value)
	{
		parameters.insert({ name, { name, ParameterType::Bool, 0.0f, 0, value, false } });
	}

	void AnimatorParmeter::AddTrigger(const std::string& name)
	{
		parameters.insert({ name, { name, ParameterType::Trigger, 0.0f, 0, false, false } });
	}
}