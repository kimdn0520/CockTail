#pragma once
#include "Scripts/Trigger.h"

namespace ClientSpace
{
	class PlayerController;
	class PlayerAttackController;

class EnemyHitTrigger : public Trigger
{
private:
	std::shared_ptr<PlayerController> player;
	std::shared_ptr<PlayerAttackController> attackCon;
	std::shared_ptr<Transform> transform;

public:
	EnemyHitTrigger(std::weak_ptr<GameObject> _object);
	virtual ~EnemyHitTrigger();

	virtual void Awake() override;
	virtual void Start() override;
	virtual void OnTriggerEnter(std::string name) override;
	virtual void OnTriggerExit(std::string name) override;

	void SetTriggerEvent(bool value);
};

}