#pragma once
#include "EventCollider.h"
#include "Scripts/PlayerController.h"
#include "PlayerAttackController.h"

namespace ClientSpace
{
	class AttackBoxColliderEvent : public EventCollider
	{
	private:
		std::shared_ptr<PlayerController> player;
		std::shared_ptr<PlayerAttackController> attackCon;

	public:
		AttackBoxColliderEvent(std::weak_ptr<GameObject> _object);
		virtual ~AttackBoxColliderEvent();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void OnCollisionEnter(std::string name) override;
		virtual void OnCollisionPersist(std::string name) override;
		virtual void OnCollisionExit(std::string name) override;
	};

}