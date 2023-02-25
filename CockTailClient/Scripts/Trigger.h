#pragma once
#include "MoolyInfo.h"

class Trigger : public ComponentBase, public MoolyEngine::IEventTrigger
{
protected:
	bool isTrigger = false;

public:
	Trigger(std::weak_ptr<GameObject> _object);
	virtual ~Trigger();

	virtual void Awake() override;
	virtual void Start() override;

	virtual void OnTriggerEnter(std::string name) override;
	virtual void OnTriggerPersist(std::string name) override;
	virtual void OnTriggerExit(std::string name) override;

	bool GetIsTrigger() const { return isTrigger; }
	void SetIsTrigger(bool value) { isTrigger = value; }
};