#pragma once

namespace ClientSpace
{
	class DamageTest : public ComponentBase
	{
	private:
		std::shared_ptr<Rigidbody> rigid;
		std::shared_ptr<Transform> transform;
		Vector3 targetPos;
		bool bDamaged;

	public:
		DamageTest(std::weak_ptr<GameObject> _object);
		virtual ~DamageTest();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

		void SetDirection(Vector3 _dir) { targetPos = _dir; }
		void SetOn() { bDamaged = true; }
		void AddForce();
	};
}