#include "GamePch.h"
#include "SphereCollider.h"

#include "Transform.h"
#include "BoxCollider.h"
#include "Object/GameObject.h"
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	SphereCollider::SphereCollider(std::weak_ptr<GameObject> gameObj)
		: ColliderBase(gameObj)
		, localCenter(Vector3::Zero)
		, worldCenter(transform->GetWorldPosition())
		, radius(1.f)
	{
		// 일단 라인 콜라이더로..
		std::vector<Vector4> vertices;
		std::vector<WORD> indices;

		Vector4 topVertex = { 0.f, radius, 0.f, 1.0f };

		vertices.push_back(topVertex);

		uint32 stackCount = 50;
		uint32 sliceCount = 50;

		float phiStep = GraphicsEngineSpace::MathHelper::Pi / stackCount;
		float thetaStep = 2.0f * GraphicsEngineSpace::MathHelper::Pi / sliceCount;

		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vector4 v;

				v.x = radius * sinf(phi) * cosf(theta);
				v.y = abs(radius * cosf(phi));
				v.z = radius * sinf(phi) * sinf(theta);
				v.w = 1.0f;

				vertices.push_back(v);
			}
		}

		for (UINT i = 1; i <= sliceCount; i++)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}

		UINT baseIndex = 1;

		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}

		/*
		Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		factory->CreateVertexBuffer("SphereLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices.data(), vertices.size());
		factory->CreateIndexBuffer("SphereLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices.data(), indices.size());*/
	}

	SphereCollider::~SphereCollider()
	{
	}

	bool SphereCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		// 구구 충돌은.. 센터사이의 거리가 반지름의 합보다 작으면 충돌한 것..
		Vector3 otherWorldCenter = other->GetWorldCenter();
		float otherRadius = other->radius;

		if (
			((worldCenter - otherWorldCenter).Length()
				<=
				std::abs(radius) + std::abs(otherRadius))
			== true)
			return true;
		else
			return false;
	}

	// Comet Engine 참고
	bool SphereCollider::CheckForBox(std::shared_ptr<BoxCollider> other)
	{
		Matrix obbRotInv = other->objWorldRotTM.Invert();

		//Matrix translate = MatrixTranslationFromVector(worldCenter);

		// OBB 회전량의 역변환을 해서. AABB 스페이스로 만들어준다.
			// 의미상으로 Vector3 Transform과 같다.
		Vector3 sphereLocalCenter = Vector3::Transform(worldCenter, obbRotInv);

		Vector3 boxLocalCenter = Vector3::Transform(other->worldCenter, obbRotInv);

		// AABB와 구 충돌.
		// Box의 MinMax와 구의 중심 사이의 거리를 확인한다.
		// 박스의 센터에서.. 사이즈(즉 박스의 한 변의 길이)의 절반을 빼거나 더하면 최소 최대 값이다.
		Vector3 boxMin = std::move(boxLocalCenter - other->size * 0.5f);
		Vector3 boxMax = std::move(boxLocalCenter + other->size * 0.5f);

		// 해당 최대 최소 값을 이용해서 x, y, z축의 min정보를 구해준다..
			// 박스와 가장 가까운.. 친구.
		float minX = std::max<float>(boxMin.x, std::min<float>(sphereLocalCenter.x, boxMax.x));
		float minY = std::max<float>(boxMin.y, std::min<float>(sphereLocalCenter.y, boxMax.y));
		float minZ = std::max<float>(boxMin.z, std::min<float>(sphereLocalCenter.z, boxMax.z));

		Vector3 minPos = Vector3{ minX, minY, minZ };

		float minDist = (minPos - sphereLocalCenter).Length();

		if (minDist <= radius)
			return true;
		else
			return false;
	}

	void SphereCollider::Release()
	{
		__super::Release();
	}

	bool SphereCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// other를 자식 포인터로 하향 캐스팅 해서.. 확인해본다..
		std::shared_ptr<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(other);
		std::shared_ptr<BoxCollider> otherBoxCol = std::dynamic_pointer_cast<BoxCollider>(other);

		// 얘가 널이 아니면.. 구 충돌 체크를 한다.
		if (otherSphereCol != nullptr)
			return CheckForSphere(otherSphereCol);
		if (otherBoxCol != nullptr)
			return CheckForBox(otherBoxCol);

		return false;
	}

	/*void SphereCollider::ColliderRender()
	{
		std::shared_ptr<GraphicsEngine> renderer = GraphicsManager::GetInstance()->GetRenderer();

		ResourceManager* resourceManager = renderer->GetResourceManager();

		Matrix worldPos = MatrixTranslationFromVector(worldCenter);
		Matrix worldScale = MatrixScalingFromVector({ radius, radius, radius });
		Matrix world = worldScale * worldPos;

		renderer->GraphicsDebugBeginEvent("Sphere Line");
		renderer->DrawLine(resourceManager->GetBuffer("SphereLineVB"), resourceManager->GetBuffer("SphereLineIB"), color, MatrixTranspose(world));
		renderer->GraphicsDebugEndEvent();
	}*/

	void SphereCollider::LateUpdate(float tick)
	{
		// 오브젝트가 움직였으면 해당 움직임을 반영해준다.
		worldCenter = localCenter + transform->GetWorldPosition();
	}
}
