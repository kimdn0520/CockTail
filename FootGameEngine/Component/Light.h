#pragma once
#include "Object/ComponentBase.h"

#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	class GameObject;
	class Transform;

	enum class LightType
	{
		Directional,
		Point,
		Spot
	};

	class Light	: public ComponentBase
	{
	private:
		std::shared_ptr<Transform> transform;

	private:
		LightType lightType;

		SimpleMath::Vector3 color;

		SimpleMath::Vector3 direction;

		SimpleMath::Matrix lightViewProj;
	
		SimpleMath::Vector3 position;
		
		float power;

		float range;

		float halfAngle;

	public:
		Light(std::weak_ptr<GameObject> _gameObject);

		virtual ~Light();

		virtual void Release() override;

		virtual void Update(float tick) override;

		virtual void DebugIMGUIRender(float tick) override;

		void ChangeType(LightType _lightType) { lightType = _lightType; }

		void SetColor(SimpleMath::Vector3 _color) { color = _color; }

		void SetDirection(SimpleMath::Vector3 _direction) { direction = _direction; }

		void SetPower(float _power) { power = _power; }

		void SetRange(float _range) { range = _range; }

		void SetHalfAngle(float _halfAngle) { halfAngle = _halfAngle; }
	};
}

