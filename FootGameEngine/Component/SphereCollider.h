#pragma once
#include "Collider.h"

namespace GameEngineSpace
{
	class SphereCollider : public Collider
	{
	protected:
		float radius = 1.0f;
		bool bDebug = false;

	public:
		SphereCollider(std::weak_ptr<GameObject> _object);
		SphereCollider(std::weak_ptr<GameObject> _object, Vector3 _pivot, float _radius);
		virtual ~SphereCollider();

		void Update(float tick) override;

		float GetRadius() const { return radius; }
		void SetRadius(float _value) { radius = _value; }

		void SetDebugRender(bool _value) { bDebug = _value; }
		void DrawDebugRender();

		void InitRenderObj() override;
		void ColliderRender() override;
	};

}
