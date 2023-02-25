#include "pch.h"
#include "EnemyController.h"
#include "EnemyNavAgent.h"
#include "Component/CapsuleCollider.h"
#include "EnemyAttackRange.h"
#include "EnemySightRange.h"
#include "Component/BoxCollider.h"
#include "Manager/PhysicsManager.h"


namespace ClientSpace
{

	EnemyController::EnemyController(std::weak_ptr<GameObject> _object)
		: EventCollider(_object)
		, bDamaged(false)
		, timer(0)
		, hp(10)
		, damage(1)
		, armor(0)
		, cooldown(2)
		, isAim(false)
		, aimTimer(0)
		, aimTime(1)
		, isAttack(false)
		, attackTimer(0)
		, attackTime(1)
		, damagedTime(1)
		, damagedTimer(0)
		, rotateSpeed(5)
		, bBackDash(false)
		, backDashCoolDown(3)
		, backDashTimer(0)
		, backDashTime(1)
	{
		std::string enemyLayer = "Enemy";
		status = EnemyStatus::IDLE;
		targetPos = Vector3::Zero;
		direction = Vector3::Zero;
		GetGameObject()->AddComponent<EnemyNavAgent>();
		GetGameObject()->AddComponent<Rigidbody>();
		GetGameObject()->AddComponent<CapsuleCollider>()->InitRenderObj();
		GetGameObject()->GetComponent<CapsuleCollider>()->SetCenter({ 0.f, 1.0f, 0.f });
		collider = GetGameObject()->GetComponent<CapsuleCollider>();
		GetGameObject()->GetComponent<Collider>()->SetLayer(enemyLayer);
		GetGameObject()->GetComponent<Rigidbody>()->InitRigidBody();
		GetGameObject()->GetComponent<Rigidbody>()->SetPhysAxisLock(false, false, false, true, true, true);
		GetGameObject()->AddComponent<EnemyAttackRange>();

		attackBox = SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		attackBox->AddComponent<BoxCollider>();
		attackCollider = attackBox->GetComponent<Collider>();
		GetGameObject()->SetChild(attackBox);
		attackBox->GetComponent<Transform>()->SetPosition({ 0.f, 1.0f, 1.0f, }, Space::LOCAL);
		attackCollider->SetTrigger(true);
		attackCollider->InitRenderObj();

		GetGameObject()->GetComponent<CapsuleCollider>()->AddColliderEvent(this);

		rigid = GetGameObject()->GetComponent<Rigidbody>();
		transform = GetGameObject()->GetComponent<Transform>();
		navAgent = GetGameObject()->GetComponent<EnemyNavAgent>();
		attackRange = GetGameObject()->GetComponent<EnemyAttackRange>();
	}

	EnemyController::~EnemyController()
	{

	}

	void EnemyController::Awake()
	{
		attackBox->SetGameObjectEnable(false);
		PhysicsManager::GetInstance()->DisableObject(attackCollider.get());

		//rigid->SetKinematic(true);
		rigid->SetMass(1.0f);
	}

	void EnemyController::Start()
	{

	}

	void EnemyController::Update(float tick)
	{
		if (status == EnemyStatus::DIE)
		{
			status = EnemyStatus::IDLE;
			hp = 10;
			objectPool->ReturnGameObject(GetGameObject());
			return;
		}

		CheckHP();

		if (status == EnemyStatus::DIE)
			return;
		
		DamageUpdate(tick);

		if (status == EnemyStatus::HIT)
			return;

		AttackUpdate(tick);
		NavAgentUpdate(tick);
		RotateUpdate(tick);
		BackDashCoolDown(tick);
	}

	void EnemyController::LateUpdate(float tick)
	{
		AddForce();
	}

	void EnemyController::OnCollisionEnter(std::string name)
	{
		/*	std::shared_ptr<GameObject> collider = SceneManager::GetInstance()->FindObject(name);
			if (collider != nullptr)
			{
				if (collider->GetComponent<Collider>()->GetLayer() == "Player")
				{
					if (bBackDash == false)
					{
						isAim = false;
						bBackDash = true;
						status = EnemyStatus::BACK;
						backDashTimer = 0.0f;
						direction = transform->GetLook() * -5.0f;
						rigid->AddForce({ direction.x * 20, direction.y, direction.z * 20 });

						if (isAttack)
						{
							isAttack = false;
							attackBox->SetGameObjectEnable(false);
							PhysicsManager::GetInstance()->DisableObject(attackCollider.get());
						}
					}
				}
			}*/
	}

	void EnemyController::DebugIMGUIRender(float tick)
	{
		auto imgui = GraphicsEngineSpace::ImGUIManager::GetInstance();

		if (imgui->CollapsingHeader("Enemy Status"))
		{
			switch (status)
			{
				case EnemyStatus::IDLE:
					imgui->Text("Status : IDLE");
					break;

				case EnemyStatus::MOVE:
					imgui->Text("Status : MOVE");
					break;

				case EnemyStatus::AIM:
					imgui->Text("Status : AIM");
					break;

				case EnemyStatus::ATTACK:
					imgui->Text("Status : ATTACK");
					break;

				case EnemyStatus::HIT:
					imgui->Text("Status : HIT");
					break;

				case EnemyStatus::DIE:
					imgui->Text("Status : DIE");
					break;

				case EnemyStatus::BACK:
					imgui->Text("Status : BACK");
					break;
			}
		}
		imgui->Spacing();

		if (imgui->CollapsingHeader("Enemy HP"))
		{
			imgui->Text("HP : " + std::to_string(hp));
		}
		imgui->Spacing();
	}

	void EnemyController::CheckHP()
	{
		if (0 >= hp && status != EnemyStatus::DIE)
			status = EnemyStatus::DIE;
	}

	void EnemyController::SetHP(int dmg)
	{
		hp -= dmg;
		CheckHP();
	}

	void EnemyController::AttackUpdate(float delta)
	{
		if (attackRange->GetIsCollide())
		{
			if (status == EnemyStatus::IDLE || status == EnemyStatus::MOVE)
			{
				isAim = true;
				status = EnemyStatus::AIM;
				aimTimer = 0.0f;
			}
		}

		Aiming(delta);
		Attacking(delta);
	}

	void EnemyController::NavAgentUpdate(float delta)
	{
		if (status == EnemyStatus::IDLE || status == EnemyStatus::MOVE)
			navAgent->SetOn(true);

		else
		{
			navAgent->SetOn(false);

			if (status != EnemyStatus::BACK && status != EnemyStatus::HIT)
				rigid->SetLinearVelocity({ 0.f,0.f,0.f });
		}
	}

	void EnemyController::RotateUpdate(float delta)
	{
		if (status == EnemyStatus::AIM)
		{
			Vector3 direction = navAgent->GetTargetTransform()->GetWorldPosition() - transform->GetWorldPosition();
			Vector3 _result;
			_result = Vector3::Lerp(transform->GetLook(), direction, delta * rotateSpeed);
			transform->SetLook(_result);
		}
	}

	void EnemyController::DamageUpdate(float delta)
	{
		if (status == EnemyStatus::HIT)
		{
			if (damagedTime > damagedTimer)
				damagedTimer += delta;

			else if (damagedTimer >= damagedTime)
			{
				status = EnemyStatus::IDLE;
				rigid->SetLinearVelocity({ 0.f,0.f,0.f });
			}
		}
	}

	void EnemyController::Aiming(float delta)
	{
		if (isAim == false)
			return;

		if (aimTime > aimTimer)
		{
			aimTimer += delta;
		}

		if (aimTimer >= aimTime)
		{
			isAim = false;
			isAttack = true;
			attackTimer = 0.0f;
			status = EnemyStatus::ATTACK;
			attackBox->SetGameObjectEnable(true);
			PhysicsManager::GetInstance()->EnableObject(attackCollider.get());

		}
	}

	void EnemyController::Attacking(float delta)
	{
		if (isAttack == false)
			return;

		if (attackTime > attackTimer)
			attackTimer += delta;

		if (attackTimer >= attackTime)
		{
			isAttack = false;
			attackBox->SetGameObjectEnable(false);
			PhysicsManager::GetInstance()->DisableObject(attackCollider.get());
			status = EnemyStatus::IDLE;
		}
	}

	void EnemyController::BackDashCoolDown(float delta)
	{

		if (bBackDash == true)
		{

			if (backDashTimer >= backDashTime)
				status = EnemyStatus::IDLE;

			if (backDashCoolDown >= backDashTimer)
			{
				backDashTimer += delta;
			}

			else
			{
				bBackDash = false;
				backDashTimer = 0.0f;
			}
		}
	}

	void EnemyController::AddForce()
	{
		if (bDamaged == true)
		{
			if (targetPos == Vector3::Zero)
				return;

			navAgent->SetOn(false);
			status = EnemyStatus::HIT;
			damagedTimer = 0.0f;

			isAim = false;
			Vector3 _result = transform->GetWorldPosition() - targetPos;
			_result.Normalize();
			_result *= 1000 * power;
			rigid->AddForce({ _result.x, 0.0f, _result.z });
			bDamaged = false;
			targetPos = Vector3::Zero;
		}
	}

}
