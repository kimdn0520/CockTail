#pragma once
#include "Object/ComponentBase.h"

namespace GameEngineSpace
{
	class Camera;
	class Transform;

	class BasicCameraController final : public ComponentBase
	{
	private:
		float moveSpeed;
		// 현재 카메라의 위치 캐싱
		std::shared_ptr<Transform> transform;
		// 현재 카메라 캐싱
		std::shared_ptr<Camera> camera;

		Vector2 prevMousePos;

	public:
		BasicCameraController(std::weak_ptr<GameObject> gameObj);
		virtual ~BasicCameraController();

		void Release() override;

		void Awake() override;
		void PreUpdate(float tick) override;
		void LateUpdate(float tick) override;
	};

}
