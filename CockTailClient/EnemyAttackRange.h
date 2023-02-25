#pragma once
#include "Scripts/Trigger.h"

namespace ClientSpace
{
class EnemyAttackRange : public Trigger
{
private:
	std::shared_ptr<GameObject> attackRangeObject;
	float radius;
	bool isCollide;

	void SetRadiusToCollider();

public:
	EnemyAttackRange(std::weak_ptr<GameObject> _obj);
	virtual ~EnemyAttackRange();

	virtual void Awake() override;
	virtual void OnTriggerEnter(std::string name) override;
	virtual void OnTriggerExit(std::string name) override;

	void SetRadius(float value);
	bool GetIsCollide() { return isCollide; }
};

}