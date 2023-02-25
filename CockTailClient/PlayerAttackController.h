#pragma once
#include "Scripts/Trigger.h"

namespace ClientSpace
{
	class PlayerController;

	struct AttackBoxObject
	{
		int num;
		int dmg = 1;
		float power = 1.0f;
		float delay = 0;
		Vector3 size = { 1,1,1 };
		Vector3 position = { 0,0,0 };
		Vector3 center = { 0,0,0 };
		std::shared_ptr<GameObject> object;
		std::shared_ptr<Collider> collider;
	};

	class PlayerAttackController : public Trigger
	{
	private:
		std::shared_ptr<PlayerController> player;
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Rigidbody> rigid;
		std::shared_ptr<InputManager> input;
		std::shared_ptr<Collider> collider;
		std::shared_ptr<Camera> camera;

		AttackBoxObject attack_1;
		AttackBoxObject attack_2;
		AttackBoxObject attack_3;

		//std::vector<std::shared_ptr<GameObject>> attackVec;
		// µð¹ö±ë¿ë ray
		std::vector<SimpleMath::Vector3> ray;

		int comboCounter;
		float timer;

		bool bAttack;

	private:
		void SetBox(AttackBoxObject object);
		void ReSizeBox(AttackBoxObject _object, Vector3 _size);
		void RePositionBox(AttackBoxObject _object, Vector3 _pos);
		void ReCenter(AttackBoxObject _object, Vector3 _center);
		void ReDelay(AttackBoxObject _object, float _delay);

	public:
		PlayerAttackController(std::weak_ptr<GameObject> _object);
		~PlayerAttackController();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void PreUpdate(float tick) override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;
		virtual void DebugIMGUIRender(float tick) override;

		virtual void OnTriggerEnter(std::string name) override;
		virtual void OnTriggerExit(std::string name) override;
		void SetTriggerEvent(bool value);

		void AddColliders(std::shared_ptr<GameObject> _box1, std::shared_ptr<GameObject> _box2, std::shared_ptr<GameObject> _box3);
		std::shared_ptr<GameObject> GetAttackBox(int num);

		void SetAttakDelayTime(int num, float value);

		void AttackUpdate(float tick);

		AttackBoxObject FindEnableAttack();
	};
}