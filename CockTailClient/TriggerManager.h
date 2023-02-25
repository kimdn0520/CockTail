#pragma once

namespace ClientSpace
{
	class TriggerManager
	{
	private:
		static std::shared_ptr<TriggerManager> instance;

	public:
		TriggerManager() = default;
		~TriggerManager() = default;
		
		static std::shared_ptr<TriggerManager> GetInstance();
	};

}