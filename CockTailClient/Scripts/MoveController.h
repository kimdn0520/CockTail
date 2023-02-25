#pragma once
#include "Util/BehaviorTree.h"

namespace ClientSpace
{	
	enum class PlayerState
	{
		IDLE,
		RUN,
	};

	class MoveController : public ComponentBase
	{
	private:
		PlayerState playerState = PlayerState::IDLE;

		std::shared_ptr<Transform> transform;
		std::shared_ptr<Transform> targetTransform;
		std::shared_ptr<MeshRenderer> meshRenderer;
		std::shared_ptr<NavMeshAgent> navMeshAgent;
		std::shared_ptr<InputManager> input;
		std::shared_ptr<Camera> camera;

		float moveSpeed;

		std::shared_ptr<SequenceNode> rootNode;

		bool isDebugPath = true;

	public:
		MoveController(std::weak_ptr<GameObject> obj);
		~MoveController();

		virtual void Awake() override;
		virtual void Start() override;

		virtual void PreUpdate(float tick) override;
		virtual void Update(float tick) override;

	public:
		void SetTarget(shared_ptr<Transform> _targetTransform) { targetTransform = _targetTransform; }
	};
}