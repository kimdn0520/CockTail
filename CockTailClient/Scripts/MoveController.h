#pragma once

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

		std::shared_ptr<Transform> targetTransform;
		std::shared_ptr<MeshRenderer> meshRenderer;
		std::shared_ptr<InputManager> input;

		float moveSpeed;

	public:
		MoveController(std::weak_ptr<GameObject> obj);
		~MoveController();

		virtual void Awake() override;
		virtual void Start() override;

		virtual void PreUpdate(float tick) override;
	};
}