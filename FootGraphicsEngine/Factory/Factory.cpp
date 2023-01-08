#include "GraphicsPch.h"
#include "Factory.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<Factory> Factory::instance = nullptr;

	void Factory::DeleteFactory()
	{
		if (instance != nullptr)
		{
			SafeReset(instance)
		}
	}
}
