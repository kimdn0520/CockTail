#include "GamePch.h"
#include "BoxCollider.h"

#include "../../FootGraphicsEngine/GraphicsEngineFramework.h"

#include "Object/GameObject.h"
#include "Transform.h"
#include "SphereCollider.h"
#include <utility>

#include "Component/Camera.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "SceneBase.h"

namespace GameEngineSpace
{
	BoxCollider::BoxCollider(std::weak_ptr<GameObject> gameObj)
		: ColliderBase(gameObj)
		, size(Vector3{ 1.f, 1.f, 1.f })
		, localCenter(Vector3::Zero)
		, worldCenter(transform->GetWorldPosition())
		, colliderRenderObj(nullptr)
	{
		objWorldRotTM = Matrix::Identity;
		worldNormals = std::vector<Vector3>(3);
		worldNormals[0] = Vector3{ 1.f, 0.f, 0.f };	// Vector3::Right
		worldNormals[1] = Vector3{ 0.f, 1.f, 0.f };	// Vector3::Up
		worldNormals[2] = Vector3{ 0.f, 0.f, 1.f };	// Vector3::BackWard;

		extents = std::vector<float>(3, size.x * 0.5f);

		// 기본적인 버텍스 버퍼를 만듭니다
			// 해당 버퍼의 사이즈 등은 이후 TM에서 관리해줍니다.
			// 여기서는 단순한 1.f 1.f 1.f의 상자를 그려줍니다.
			// 기본적으로 8개의 버텍스 (텍스쳐가 들어가지 않음 -> 쪼개기의 필요가 없음..
		/*Vector4 vertices[8] =
		{
			// upSide
			{-extents[0], extents[1], extents[2], 1.0f},
			{extents[0], extents[1], extents[2], 1.0f},
			{extents[0], extents[1], -extents[2], 1.0f},
			{-extents[0], extents[1], -extents[2], 1.0f},

			// downside
			{-extents[0], -extents[1], extents[2], 1.0f},
			{extents[0], -extents[1], extents[2], 1.0f},
			{extents[0], -extents[1], -extents[2], 1.0f},
			{-extents[0], -extents[1], -extents[2], 1.0f}
		};

		WORD indices[24] =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			0, 4,
			1, 5,
			2, 6,
			3, 7,
			4, 5,
			5, 6,
			6, 7,
			7, 4
		};*/

		// 그래픽스 엔진에서 버퍼 생성
		/*Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		factory->CreateVertexBuffer("BoxLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices, 8);
		factory->CreateIndexBuffer("BoxLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices, 24);*/
	}

	BoxCollider::~BoxCollider()
	{
	}

	void BoxCollider::Release()
	{
		__super::Release();
	}

	// OBB를 이용한다. by Comet
	bool BoxCollider::CheckForBox(std::shared_ptr<BoxCollider> other)
	{
		const Vector3& otherCenter = other->worldCenter;
		// 각 면까지의 거리를 받아온다.
		const std::vector<float>& otherExtents = other->extents;
		// 검증에 필요한 축들을 받아온다.
		const std::vector<Vector3>& otherNormals = other->worldNormals;

		// 내적 연산의 결과를 보관하는 변수들.
		float otherCDotPlane = 0.f;
		float myCDotPlane = 0.f;
		float otherDotRadius = 0.f;
		float myDotRadius = 0.f;
		float otherDotMin = 0.f;
		float otherDotMax = 0.f;
		float myDotMin = 0.f;
		float myDotMax = 0.f;

		// 내 평면들에 대한 검사.
			// 직육면체는 평행하지 않은 면이 세 개이기 때문에 세 번만 해주면 된다.
		for (size_t i = 0; i < 3; i++)
		{
			const Vector3& targetNormal = worldNormals[i];

			// 중심에서 판별 축을 향한 내적.
			otherCDotPlane = otherCenter.Dot(targetNormal);
			myCDotPlane = worldCenter.Dot(targetNormal);

			otherDotRadius = 0.f;

			// 분리축 반지름을 찾는 과정
				// TODO 해당 내용을 이해하자..
				// 각 축의 노말에 길이를 곱해서나온 녀석을 모두 더해준 Radius..
				// 결국 판별축에서의 길이를 구해주는 과정이다.
			for (size_t j = 0; j < 3; j++)
				otherDotRadius += fabsf(otherNormals[j].Dot(targetNormal)) * otherExtents[j];

			myDotRadius = extents[i];

			otherDotMin = otherCDotPlane - otherDotRadius;
			otherDotMax = otherCDotPlane + otherDotRadius;

			myDotMin = myCDotPlane - myDotRadius;
			myDotMax = myCDotPlane + myDotRadius;

			// 분리축 발견.
				// 해당 축에서 겹치는 게 없으면..!
			if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
				return false;
		}

		// 상대의 평면에 대한 검사
		for (size_t i = 0; i < 3; i++)
		{
			const Vector3& targetNormal = otherNormals[i];

			otherCDotPlane = otherCenter.Dot(targetNormal);
			myCDotPlane = worldCenter.Dot(targetNormal);

			myDotRadius = 0.f;

			for (size_t j = 0; j < 3; j++)
				myDotRadius += fabsf(worldNormals[j].Dot(targetNormal)) * extents[j];

			otherDotRadius = otherExtents[i];

			otherDotMin = otherCDotPlane - otherDotRadius;
			otherDotMax = otherCDotPlane + otherDotRadius;

			myDotMin = myCDotPlane - myDotRadius;
			myDotMax = myCDotPlane + myDotRadius;

			// 분리축 발견.
				// 해당 축에서 겹치는 게 없으면..!
			if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
				return false;
		}

		// 각 모서리의 외적으로 생성되는 평면에 대한 검사.
			// 면으로만 투영했을시 처리가 안되는 충돌 연산을 검사하기 위해.
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				Vector3 targetNormal;
				worldNormals[i].Cross(otherNormals[j], targetNormal);

				if (targetNormal == Vector3::Zero)
					continue;

				targetNormal.Normalize(targetNormal);

				otherCDotPlane = otherCenter.Dot(targetNormal);
				myCDotPlane = worldCenter.Dot(targetNormal);

				myDotRadius = 0.f;

				for (size_t k = 0; k < 3; k++)
					myDotRadius += fabsf(worldNormals[k].Dot(targetNormal)) * extents[k];

				otherDotRadius = 0.f;

				for (size_t k = 0; k < 3; k++)
					otherDotRadius += fabsf(otherNormals[k].Dot(targetNormal)) * otherExtents[k];

				otherDotMin = otherCDotPlane - otherDotRadius;
				otherDotMax = otherCDotPlane + otherDotRadius;

				myDotMin = myCDotPlane - myDotRadius;
				myDotMax = myCDotPlane + myDotRadius;

				if ((myDotMax < otherDotMin) || (myDotMin > otherDotMax))
					return false;
			}
		}

		//DoNotCheckEdge:

		// 모든 경우에 분리축을 찾지 못했다면? 이건 충돌이다!
		return true;
	}

	bool BoxCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		// 구에서 구현 한 것과 마찬가지로.. obb를 AABB 스페이스로 옮긴 뒤. 충돌 체크한다.
		Matrix obbRotInv = objWorldRotTM.Invert();

		Vector3 sphereLocalCenter = Vector3::Transform(other->worldCenter, obbRotInv);
		Vector3 boxLocalCenter = Vector3::Transform(worldCenter, obbRotInv);

		Vector3 boxMin = std::move(boxLocalCenter - size * 0.5);
		Vector3 boxMax = std::move(boxLocalCenter + size * 0.5);

		float minX = std::max<float>(boxMin.x, std::min<float>(sphereLocalCenter.x, boxMax.x));
		float minY = std::max<float>(boxMin.y, std::min<float>(sphereLocalCenter.y, boxMax.y));
		float minZ = std::max<float>(boxMin.z, std::min<float>(sphereLocalCenter.z, boxMax.z));

		Vector3 minPos = Vector3{ minX, minY, minZ };

		float minDist = (minPos - sphereLocalCenter).Length();

		if (minDist <= other->radius)
			return true;
		else
			return false;
	}

	void BoxCollider::SetSize(const Vector3& newSize)
	{
		size = newSize;
		tmpSize = newSize;

		extents[0] = size.x * 0.5;
		extents[1] = size.y * 0.5;
		extents[2] = size.z * 0.5;

	}

	bool BoxCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// 하향 캐스팅 이후..

		std::shared_ptr<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(other);
		std::shared_ptr<BoxCollider> otherBoxCol = std::dynamic_pointer_cast<BoxCollider>(other);

		if (otherSphereCol != nullptr)
			return CheckForSphere(otherSphereCol);
		if (otherBoxCol != nullptr)
			return CheckForBox(otherBoxCol);

		return false;
	}

	void BoxCollider::InitRenderObj(std::string _objName)
	{
		colliderRenderObj = Factory::GetInstance()->CreateDXObject<LineObj>(
		BuilderManger::GetInstance()->GetBuilder("LineBuilder"), _objName);

		// 해당 Obj는 게임엔진 내부에서 Debug용으로 껐다 켰다를 할 예정이므로 따로 렌더큐에 넣어주지 않는다.
		auto renderer = GraphicsManager::GetInstance()->GetRenderer();
		renderer->InitObject(colliderRenderObj);
	}

	void BoxCollider::ColliderRender()
	{
		auto renderer = GraphicsManager::GetInstance()->GetRenderer();

		Matrix worldPos = XMMatrixTranslation(worldCenter.x, worldCenter.y, worldCenter.z);
		Matrix worldScale = XMMatrixScaling(size.x, size.y, size.z);
		Matrix worldTM = worldScale * objWorldRotTM * worldPos;

		auto cam = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();

		colliderRenderObj->Update(worldTM, cam->GetViewMatrix(), cam->GetProjMatrix());
		renderer->GraphicsDebugBeginEvent("Box Line");
		colliderRenderObj->Render();
		renderer->GraphicsDebugEndEvent();
	}

	void BoxCollider::LateUpdate(float tick)
	{
		// 월드 센터 업데이트
		worldCenter = localCenter + transform->GetWorldPosition();

		size = tmpSize * transform->GetWorldScale();

		Quaternion tmpRotQuat = XMQuaternionRotationRollPitchYawFromVector(transform->GetWorldRotation());

		Matrix objRotTM = XMMatrixRotationQuaternion(transform->EulerToQuat(transform->GetWorldRotation()));

		// 지난 번 프레임과 비교해서 이번 프레임에 회전 값의 변화가 있을 경우.
		if (objWorldRotTM == objRotTM)
			return;

		// 충돌 검사에 필요한 정보를 캐싱
		objWorldRotTM = objRotTM;

		// Transform
		worldNormals[0] = Vector3{ 1.f, 0.f, 0.f };
		worldNormals[0] = Vector3::Transform(worldNormals[0], objRotTM);
		worldNormals[0].Normalize(worldNormals[0]);

		worldNormals[1] = Vector3{ 0.f, 1.f, 0.f };
		worldNormals[1] = Vector3::Transform(worldNormals[1], objRotTM);
		worldNormals[1].Normalize(worldNormals[1]);

		worldNormals[2] = Vector3{ 0.f, 0.f, 1.f };
		worldNormals[2] = Vector3::Transform(worldNormals[2], objRotTM);
		worldNormals[2].Normalize(worldNormals[2]);
	}
}
