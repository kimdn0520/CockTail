#include "GamePch.h"
#include "SphereCollider.h"

#include "Object/GameObject.h"
#include "GraphicsManager.h"

#include "SceneManager.h"
#include "SceneBase.h"
#include "Camera.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	SphereCollider::SphereCollider(std::weak_ptr<GameObject> _object) :
		Collider(_object), bDebug(true)
	{
		colliderType = MoolyEngine::GeometryType::SPHERE;
	}

	SphereCollider::SphereCollider(std::weak_ptr<GameObject> _object, Vector3 _pivot, float _radius) :
		Collider(_object)
	{
		colliderType = MoolyEngine::GeometryType::SPHERE;
		center = _pivot;
		radius = _radius;
	}

	SphereCollider::~SphereCollider()
	{
	}

	
	void SphereCollider::Update(float tick)
	{
		if (bDebug)
			DrawDebugRender();
	}

	void SphereCollider::DrawDebugRender()
	{

	}

	void SphereCollider::InitRenderObj()
	{
		renderObj = Factory::GetInstance()->CreateDXObject<LineObj>(
			BuilderManger::GetInstance()->GetBuilder("LineBuilder"), "WireSphere");

		GraphicsManager::GetInstance()->GetRenderer()->InitObject(renderObj);
	}

	void SphereCollider::ColliderRender()
	{
		if (bDebug != true)
			return;

		Matrix worldPos = XMMatrixTranslation(transform->GetWorldPosition().x, transform->GetWorldPosition().y, transform->GetWorldPosition().z);
		Matrix worldScale = XMMatrixScaling(radius, radius, radius);
		Matrix worldRot = XMMatrixRotationQuaternion(transform->GetWorldRotationQuat());

		Matrix worldTM = worldScale * worldRot * worldPos;

		auto cam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		renderObj->Update(worldTM, cam->GetViewMatrix(), cam->GetProjMatrix());
		renderObj->Render();
	}
}
