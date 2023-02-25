#include "GamePch.h"
#include "CapsuleCollider.h"

#include "Object/GameObject.h"
#include "GraphicsManager.h"

#include "SceneManager.h"
#include "SceneBase.h"
#include "Camera.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	CapsuleCollider::CapsuleCollider(std::weak_ptr<GameObject> _object)
		: Collider(_object)
		, radius(0.5)
		, height(1)
		, bDebug(true)

	{
		colliderType = MoolyEngine::GeometryType::CAPSULE;
		center = { 0, radius * 2 + height / 2, 0 };
	}

	CapsuleCollider::CapsuleCollider(std::weak_ptr<GameObject> _object, Vector3 _pivot, float _radius, float _height)
		: Collider(_object)
		, radius(_radius)
		, height(_height)
	{
		colliderType = MoolyEngine::GeometryType::CAPSULE;
		center = _pivot;
	}

	CapsuleCollider::~CapsuleCollider()
	{

	}

	void CapsuleCollider::InitRenderObj()
	{
		renderObj = Factory::GetInstance()->CreateDXObject<LineObj>(
			BuilderManger::GetInstance()->GetBuilder("LineBuilder"), "Capsule");

		GraphicsManager::GetInstance()->GetRenderer()->InitObject(renderObj);
	}

	void CapsuleCollider::ColliderRender()
	{
		if (bDebug != true)
			return;

		Matrix worldPos = XMMatrixTranslation(transform->GetWorldPosition().x + center.x, transform->GetWorldPosition().y + center.y, transform->GetWorldPosition().z + center.z);
		Matrix worldScale = XMMatrixScaling(radius * 2, height, radius * 2);
		Matrix worldRot = XMMatrixRotationQuaternion(transform->GetWorldRotationQuat());

		Matrix worldTM = worldScale * worldRot * worldPos;

		auto cam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		renderObj->Update(worldTM, cam->GetViewMatrix(), cam->GetProjMatrix());
		renderObj->Render();
	}
}