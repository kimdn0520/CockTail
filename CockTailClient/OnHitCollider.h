#pragma once
#include "EventCollider.h"
#include "Scripts/PlayerController.h"
#include "PlayerAttackController.h"

namespace ClientSpace
{
	class OnHitCollider : public EventCollider
	{
	private:
		std::shared_ptr<PlayerController> player;
		std::shared_ptr<PlayerAttackController> attackCon;

	public:
		OnHitCollider(std::weak_ptr<GameObject> _object);
		virtual ~OnHitCollider();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void OnCollisionEnter(std::string name) override;
		virtual void OnCollisionExit(std::string name) override;

		void SetColliderEvent(bool value);
	};

}