#pragma once
#include "Manager/PhysicsManager.h"

namespace ClientSpace
{
	enum class PlayerStatus
	{
		IDLE,
		MOVE,
		DASH,
		ATTACK,
	};

	class PlayerController : public ComponentBase
	{
	private:
		std::shared_ptr<IDXObject> renderObj;

		bool bMoveToTarget;
		Vector3 moveTarget;
		PlayerStatus status;

		std::shared_ptr<Transform> transform;
		std::shared_ptr<Rigidbody> rigid;
		std::shared_ptr<MeshRenderer> meshRenderer;
		std::shared_ptr<InputManager> input;
		std::shared_ptr<Collider> collider;
		std::shared_ptr<GameObject> blocker;

		Vector3 direction;
		Vector3 prevDirection;
		Vector3 dashDirection;

		Vector3 gravity;

		float moveSpeed;
		float rotateSpeed;
		float dashTime;
		float currentTimer;
		float dashSpeed;

		bool isGrounded;
		bool isDash;
		bool isStop;
		bool bStop;
		bool isWall;

		RaycastHit hitSlope;
		float slopeAngle;
		float rayDistance;
		float maxSlopeAngle;

	public:
		PlayerController(std::weak_ptr<GameObject> _object);
		~PlayerController();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void FixedUpdate(float tick) override;
		virtual void PreUpdate(float tick) override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;
		virtual void DebugIMGUIRender(float tick) override;

		void SetTarget(Vector3 pos) { moveTarget = pos; }
		void SetBool(bool value) { bMoveToTarget = value; }

		// JW 2.11(토) 테스트 중..
	private:
		void InputKey();
		void UpdateIdle();
		void UpdateMove(float tick);
		void UpdateDash(float tick);
		void Move();
		void Turn(float tick);
		bool IsGround();
		bool IsSlope();
		bool isBorder();

		void UpdateKinematic(float tick);
		void MoveToTarget(Vector3 target);

		//void InitAttackBox(std::shared_ptr<GameObject> _box);
		/*void MoveUpdate();
		void StopCharacter(float delta);
		void DashInputUpdate();
		void InputUpdate();
		void DashUpdate(float deltaTime);
		void RaycastUpdate();
		void RotateUpdate(float deltaTime);*/
		//std::shared_ptr<GameObject> GetAttackBox(int num);

		Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal);
	};
}
