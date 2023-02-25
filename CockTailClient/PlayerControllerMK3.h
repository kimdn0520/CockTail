#pragma once
#include "Manager/PhysicsManager.h"

namespace ClientSpace
{
	class PlayerControllerMK3 : public ComponentBase
	{
	private:
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Rigidbody> rigid;
		std::shared_ptr<MeshRenderer> meshRenderer;
		std::shared_ptr<Collider> collider;

	public:
		void CreatePlayer();
		void Move();
		void InputUpdate();

		virtual void Awake() override;
		virtual void Update(float tick) override;

	};


}