#pragma once
#include "Scripts/Trigger.h"

namespace ClientSpace
{
	class GameEngineSpace::InputManager;

	class InteractionTrigger : public Trigger
	{
	private:
		bool isInteracted;
		bool isSelected;
		std::shared_ptr<Collider> collider;
		std::shared_ptr<InputManager> input;

	public:
		InteractionTrigger(std::weak_ptr<GameObject> _obj);
		virtual ~InteractionTrigger();

		virtual void Awake();
		virtual void Update(float tick) override;
		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		bool IsInteracted() { return isInteracted; }
		bool IsSelected() { return isSelected; }

	private:
		void InputUpdate();
	};
}