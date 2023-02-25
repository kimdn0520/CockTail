#pragma once
#include "Scripts/Trigger.h"

namespace ClientSpace
{
	class PlayerRellyPointTrigger : public Trigger
	{
	private:
		std::shared_ptr<Collider> collider;

	public:
		PlayerRellyPointTrigger(std::weak_ptr<GameObject> _obj);
		virtual ~PlayerRellyPointTrigger();

		virtual void Awake() override;
		virtual void OnTriggerEnter(std::string name) override;
	};
}