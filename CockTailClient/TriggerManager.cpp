#include "pch.h"
#include "TriggerManager.h"

namespace ClientSpace
{
	std::shared_ptr<TriggerManager> TriggerManager::instance = nullptr;

	std::shared_ptr<TriggerManager> TriggerManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<TriggerManager>();

		return instance;
	}
}