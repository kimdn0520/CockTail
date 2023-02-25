#pragma once
#include "Collider.h"

namespace GameEngineSpace
{
	// physx로 리뉴얼

	class BoxCollider : public Collider
	{
	protected:
		Vector3 boxSize = Vector3::One;
		bool bDebug;

	public:
		BoxCollider(std::weak_ptr<GameObject> _object);
		BoxCollider(std::weak_ptr<GameObject> _object ,Vector3 _pivot, Vector3 _size);
		virtual ~BoxCollider();

		//void Update(float tick) override;

		Vector3 GetSize() const { return boxSize; }
		void SetSize(Vector3 _value) { boxSize = _value; }

		void SetDebugRender(bool _value) { bDebug = _value; }
		void DrawDebugRender();

		void InitRenderObj() override;
		void ColliderRender() override;
	};
}
