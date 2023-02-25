#include "GamePch.h"
#include "BehaviorTree.h"

namespace GameEngineSpace
{
	bool SequenceNode::run()
	{
		for (auto& child : getChildren())
		{
			if (child->run())
				return true;
		}

		return false;
	}

	bool SelectorNode::run()
	{
		for (auto& child : getChildren())
		{
			if (!child->run())
				return false;
		}

		return true;
	}

	bool ConditionNode::run()
	{
		if (condition()) 
		{
			return true;
		}

		return false;
	}
}
