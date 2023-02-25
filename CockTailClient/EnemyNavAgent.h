#pragma once
#include "Util/BehaviorTree.h"

namespace ClientSpace
{
	class PlayerController;
	class EnemyController;

	class EnemyNavAgent : public ComponentBase
	{
	private:
		std::shared_ptr<GameObject> player;
		std::shared_ptr<Transform> targetTransform;
		std::shared_ptr<PlayerController> playerController;
		std::shared_ptr<Transform> transform;
		std::shared_ptr<MeshRenderer> meshRenderer;
		std::shared_ptr<NavMeshAgent> navMeshAgent;
		std::shared_ptr<EnemyController> controller;

		float moveSpeed;
		bool bSet;

		std::shared_ptr<SequenceNode> rootNode;

	public:
		EnemyNavAgent(std::weak_ptr<GameObject> _object);
		virtual ~EnemyNavAgent();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void PreUpdate(float tick) override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

		void SetOn(bool value) { bSet = value; }
		bool GetSet() { return bSet; }

		void SetTrackingStatus(bool value);

		void Chasetarget(float tick);

		std::shared_ptr<Transform> GetTargetTransform() { return targetTransform; }
	};
}