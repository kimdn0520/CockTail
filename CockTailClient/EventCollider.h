#pragma once
#include "MoolyInfo.h"

namespace ClientSpace
{
	class EventCollider : public ComponentBase, public MoolyEngine::IEventCollider
	{
	protected:
		bool isCollide = false;

	public:
		EventCollider(std::weak_ptr<GameObject> _object);
		virtual ~EventCollider() {}

		virtual void Awake() override;
		virtual void Start() override;

		virtual void OnCollisionEnter(std::string name) override;
		virtual void OnCollisionPersist(std::string name) override;
		virtual void OnCollisionExit(std::string name) override;

		bool GetIsCollide() const { return isCollide; }
		void SetIsCollide(bool value) { isCollide = value; }
	};
}