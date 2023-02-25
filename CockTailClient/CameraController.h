#pragma once

namespace ClientSpace
{
	class PlayerController;

	class CameraController : public ComponentBase
	{
	private:
		std::shared_ptr<GameObject> player;
		std::shared_ptr<Transform> playerTransform;
		std::shared_ptr<Transform> transform;
		Vector3 offset;
		float smoothSpeed = 0.125f;

	public:
		CameraController(std::weak_ptr<GameObject> _object);
		virtual ~CameraController();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void PreUpdate(float tick) override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;
	};

}