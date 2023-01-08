#include "pch.h"
#include "Export/IMoolyEngine.h"
#include "MoolyEngine/MoolyEngineX.h"

MOOLY_ENGINE std::shared_ptr<MoolyEngine::IMoolyEngine> MoolyEngine::MoolyEngineInstance()
{
	if (MoolyEngine::g_MoolyEngine == nullptr)
	{
		g_MoolyEngine = std::make_shared<MoolyEngine::MoolyEngineX>();
		return g_MoolyEngine;
	}

	return g_MoolyEngine;
}

MOOLY_ENGINE void MoolyEngine::MoolyEngineRelease()
{
	g_MoolyEngine.reset();
}
