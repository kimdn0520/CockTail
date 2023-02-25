#pragma once
#include "Manager/PhysicsManager.h"

namespace ClientSpace
{
	enum class PlayerStatusMK2
	{
		IDLE,
		MOVE,
		DASH,
		ATTACK
	};

	class PlayerControllerMK2 : public ComponentBase
	{
	private:
		PlayerStatusMK2 status;
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Rigidbody> rigid;
		std::shared_ptr<MeshRenderer> meshRenderer;
		std::shared_ptr<InputManager> input;
		std::shared_ptr<Collider> collider;

		Vector3 direction;
		Vector3 prevDirection;
		Vector3 dashDirection;

		float moveSpeed;
		float rotateSpeed;
		float dashTime;
		float dashSpeed;
		float currentTimer;
	
		bool isGrounded;
		bool isDash;
		bool isStop;
		bool bStop;

		RaycastHit hit;
		float rayDistance;

	public:
		PlayerControllerMK2(std::weak_ptr<GameObject> _object);
		virtual ~PlayerControllerMK2();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void PreUpdate(float tick) override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

		void StopCharacter(float delta);
		void RaycastUpdate();
		void InputUpdate();
		void MoveUpdate();
		void RotateUpdate(float deltaTime);
	};

}