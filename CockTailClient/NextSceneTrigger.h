#pragma once
#include "Scripts/Trigger.h"

namespace ClientSpace
{
	class NextSceneTrigger : public Trigger
	{
	private:
		std::shared_ptr<Collider> collider;
		tstring nextScene;

	public:
		NextSceneTrigger(std::weak_ptr<GameObject> _obj);
		virtual ~NextSceneTrigger();

		virtual void Awake() override;
		virtual void OnTriggerEnter(std::string name) override;

		void SetNextScene(tstring value) { nextScene = value; }
		tstring GetNextScene() { return nextScene; }
	};
}