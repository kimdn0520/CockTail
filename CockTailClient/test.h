#pragma once
#include "Manager/PhysicsManager.h"

namespace ClientSpace
{
	class test : public ComponentBase
	{
	private:
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Rigidbody> rigid;
		std::shared_ptr<InputManager> input;
		std::shared_ptr<Collider> collider;

		Vector3 direction;
		Vector3 gravity;

		float moveSpeed;
		
	public:
		test(std::weak_ptr<GameObject> obj);
		~test();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;
		virtual void DebugIMGUIRender(float tick) override;

		void InputKey();
		void UpdateMove(float tick);
		void Move();

	};

}