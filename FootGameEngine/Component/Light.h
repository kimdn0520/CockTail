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

	class Light	: public ComponentBase, public std::enable_shared_from_this<Light>
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

		float innerSpotAngle;

		float outerSpotAngle;

		bool isShadow;

	public:
		Light(std::weak_ptr<GameObject> _gameObject);

		virtual ~Light();

		virtual void Awake() override;

		virtual void Release() override;

		virtual void Update(float tick) override;

		virtual void DebugIMGUIRender(float tick) override;

		void ChangeType(LightType _lightType) { lightType = _lightType; }

		void SetColor(SimpleMath::Vector3 _color) { color = _color; }

		void SetDirection(SimpleMath::Vector3 _direction) { direction = _direction; }

		void SetPower(float _power) { power = _power; }

		void SetRange(float _range) { range = _range; }

		void SetInnerSpotAngle(float _innerSpotAngle) { innerSpotAngle = _innerSpotAngle; }

		void SetOuterSpotAngle(float _outerSpotAngle) { outerSpotAngle = _outerSpotAngle; }

		void SetIsShadow(bool _isShadow) { isShadow = _isShadow; }

		bool GetIsShadow() { return isShadow; }
	};
}

