#pragma once
#include "Trigger.h"
#include "Scripts/PlayerController.h"
#include "PlayerAttackController.h"

namespace ClientSpace
{
	class OnHitTrigger : public Trigger
	{
	private:
		std::shared_ptr<PlayerController> player;
		std::shared_ptr<PlayerAttackController> attackCon;
		std::shared_ptr<Transform> transform;

	public:
		OnHitTrigger(std::weak_ptr<GameObject> _object);
		virtual ~OnHitTrigger();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;

		void SetTriggerEvent(bool value);
	};
}