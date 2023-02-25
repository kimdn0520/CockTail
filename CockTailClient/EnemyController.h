#pragma once
#include "EventCollider.h"

namespace ClientSpace
{
	enum class EnemyStatus
	{
		IDLE,
		MOVE,
		AIM,
		ATTACK,
		HIT,
		DIE,
		BACK
	};

	class EnemyNavAgent;
	class EnemyAttackRange;
	class EnemySightRange;

	class EnemyController : public EventCollider
	{
	private:
		std::shared_ptr<Collider> collider;
		std::shared_ptr<Rigidbody> rigid;
		std::shared_ptr<Transform> transform;
		std::shared_ptr<EnemyNavAgent> navAgent;
		std::shared_ptr<EnemyAttackRange> attackRange;
		std::shared_ptr<Collider> attackCollider;
		std::shared_ptr<GameObject> attackBox;
		std::shared_ptr<GameObject> blocker;

		std::shared_ptr<ObjectPool> objectPool;

		EnemyStatus status;

		Vector3 targetPos;
		bool bDamaged;
		float timer;

		int hp;
		int damage;
		int armor;
		float cooldown;

		bool isAim;
		float aimTime;
		float aimTimer;

		bool isAttack;
		float attackTimer;
		float attackTime;

		float damagedTime;
		float damagedTimer;

		float power;

		float rotateSpeed;

		Vector3 direction;
		bool bBackDash;
		float backDashCoolDown;
		float backDashTimer;
		float backDashTime;

	public:
		EnemyController(std::weak_ptr<GameObject> _object);
		virtual ~EnemyController();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

		virtual void OnCollisionEnter(std::string name) override;
		virtual void DebugIMGUIRender(float tick) override;

		void AttackUpdate(float delta);
		void NavAgentUpdate(float delta);
		void RotateUpdate(float delta);

		void DamageUpdate(float delta);

		void CheckHP();
		void SetHP(int dmg);
		void Aiming(float delta);
		void Attacking(float delta);

		void BackDashCoolDown(float delta);

		void SetStatus(EnemyStatus stat) { status = stat; }
		EnemyStatus GetStatus() { return status; }

		void SetDirection(Vector3 _dir) { targetPos = _dir; }
		void SetOn() { bDamaged = true; }
		void SetTimer(float value) { timer = value; }
		void SetDamagedTime(float value) { damagedTimer = value; }
		void SetPower(float value) { power = value; }

		void AddForce();

		void SetObjectPool(std::shared_ptr<ObjectPool> _objectPool) { objectPool = _objectPool; }
	};
}
