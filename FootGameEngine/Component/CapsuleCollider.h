#pragma once
#include "Collider.h"

namespace GameEngineSpace
{
	class CapsuleCollider : public Collider
	{
	protected:
		float radius;
		float height;

		bool bDebug = false;

	public:
		CapsuleCollider(std::weak_ptr<GameObject> _object);
		CapsuleCollider(std::weak_ptr<GameObject> _object, Vector3 _pivot, float _radius, float _height);
		virtual ~CapsuleCollider();

		float GetRadius() { return radius; }
		float GetHeight() { return height; }
		void SetRadius(float value) { radius = value; }
		void SetHeight(float value) { height = value; }

		void InitRenderObj() override;
		void ColliderRender() override;
	};
}