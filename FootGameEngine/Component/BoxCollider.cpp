#include "GamePch.h"
#include "BoxCollider.h"

#include "Object/GameObject.h"
#include "GraphicsManager.h"

#include "SceneManager.h"
#include "SceneBase.h"
#include "Camera.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	BoxCollider::BoxCollider(std::weak_ptr<GameObject> _object) 
		: Collider(_object), bDebug(true)
	{
		colliderType = MoolyEngine::GeometryType::BOX;
	}

	BoxCollider::BoxCollider(std::weak_ptr<GameObject> _object, Vector3 _pivot, Vector3 _size) :
		Collider(_object), bDebug(false)
	{
		colliderType = MoolyEngine::GeometryType::BOX;
		center = _pivot;
		boxSize = _size;
	}

	BoxCollider::~BoxCollider()
	{
	}

	void BoxCollider::DrawDebugRender()
	{
		// TODO 콜라이더렌더
	}

	void BoxCollider::InitRenderObj()
	{
		renderObj = Factory::GetInstance()->CreateDXObject<LineObj>(
			BuilderManger::GetInstance()->GetBuilder("LineBuilder"), "WireCube");

		GraphicsManager::GetInstance()->GetRenderer()->InitObject(renderObj);
	}

	void BoxCollider::ColliderRender()
	{
		if (bDebug != true)
			return;

		Matrix worldPos = XMMatrixTranslation(transform->GetWorldPosition().x + center.x, transform->GetWorldPosition().y + center.y, transform->GetWorldPosition().z + center.z);
		Matrix worldScale = XMMatrixScaling(boxSize.x, boxSize.y, boxSize.z);
		Matrix worldRot = XMMatrixRotationQuaternion(transform->GetWorldRotationQuat());

		Matrix worldTM = worldScale * worldRot * worldPos;

		auto cam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		renderObj->Update(worldTM, cam->GetViewMatrix(), cam->GetProjMatrix());
		renderObj->Render();		
	}
}
