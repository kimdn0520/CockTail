#include "GamePch.h"
#include "RayCollider.h"

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Object/GameObject.h"
#include "Transform.h"
#include <utility>
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	RayCollider::RayCollider(std::weak_ptr<GameObject> gameObj)
		: ColliderBase(gameObj)
		, localCenter(Vector3::Zero)
		, worldCenter(transform->GetWorldPosition())
		, direction(transform->GetLook())
		, hit({})
	{
		// 로컬 센터에서 부터 Dir방향으로 떨어진..
		Vector4 vertices[2] =
		{
			{0.f, 0.f, 0.f, 1.0f},
			{(direction.x * 10.f), (direction.y * 10.f), (direction.z * 10.f), 1.0f}
		};

		WORD indices[2]
		{
			0, 1
		};

		// 그래픽스 엔진 가져와야함.
		/*Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();

		factory->CreateVertexBuffer("RayLineVB", USAGE::DEFAULT, 0, sizeof(Vector), vertices, 2);
		factory->CreateIndexBuffer("RayLineIB", USAGE::DEFAULT, 0, sizeof(WORD), indices, 2);*/
	}

	void RayCollider::Release()
	{
		__super::Release();
	}

	bool RayCollider::CheckForBox(std::shared_ptr<BoxCollider> other)
	{
		// this가 Ray고 other이 Ray가 아닐 때.
		Matrix obbRotInv = other->objWorldRotTM.Invert();

		// 포지션을 other의 스페이스로 변환
		Vector3 rayLocalCenter = Vector3::Transform(worldCenter, obbRotInv);
		// direction도 변환해주기
		Vector3 rayLocalDir = Vector3::Transform(direction, obbRotInv);
		rayLocalDir.Normalize(rayLocalDir);
		rayLocalDir = Vector3{ 1.0f / rayLocalDir.x, 1.0f / rayLocalDir.y, 1.0f / rayLocalDir.z };

		Vector3 boxLocalCenter = Vector3::Transform(other->worldCenter, obbRotInv);

		// Box의 버텍스 중 x, y, z의 최소, 최대 값.
		Vector3 boxMin = { FLT_MAX, FLT_MAX, FLT_MAX };
		Vector3 boxMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		// 상자의 모든 버텍스 좌표를 돌면서 MinMax를 뽑아낸다..
			// extents를 활용해서 구해주자.
		std::vector<Vector3> vertices;
		// 중심에서 각 extents 별로 더해주고 빼주면 vertices 정보가 나온다.
			// extents == 각 축의 half
		Vector3 tempVertex = { boxLocalCenter.x + other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x + other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x + other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x - other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x + other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x - other->extents[0], boxLocalCenter.y + other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x - other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z + other->extents[2] };
		vertices.push_back(tempVertex);
		tempVertex = Vector3{ boxLocalCenter.x - other->extents[0], boxLocalCenter.y - other->extents[1], boxLocalCenter.z - other->extents[2] };
		vertices.push_back(tempVertex);

		for (int i = 0; i < vertices.size(); i++)
		{

			// 버텍스의 각 축 좌표
			float vertexCoord = vertices[i].x;

			if (boxMin.x > vertexCoord)
				boxMin.x = vertexCoord;

			if (boxMax.x < vertexCoord)
				boxMax.x = vertexCoord;

			vertexCoord = vertices[i].y;

			if (boxMin.y > vertexCoord)
				boxMin.y = vertexCoord;

			if (boxMax.y < vertexCoord)
				boxMax.y = vertexCoord;

			vertexCoord = vertices[i].z;

			if (boxMin.z > vertexCoord)
				boxMin.z = vertexCoord;

			if (boxMax.z < vertexCoord)
				boxMax.z = vertexCoord;
		}

		// 레이의 위치로 뽑아낸 벡터
		boxMin = boxMin - rayLocalCenter;
		boxMax = boxMax - rayLocalCenter;

		// 레이에 투영
		Vector3 t1 = boxMin * rayLocalDir;
		Vector3 t2 = boxMax * rayLocalDir;

		// 각 좌표축의 최소 최대값.
		Vector3 tMin;
		Vector3 tMax;

		if (t1.x > t2.x)
		{
			tMin.x = t2.x;
			tMax.x = t1.x;
		}
		else
		{
			tMin.x = t1.x;
			tMax.x = t2.x;
		}

		if (t1.y > t2.y)
		{
			tMin.y = t2.y;
			tMax.y = t1.y;
		}
		else
		{
			tMin.y = t1.y;
			tMax.y = t2.y;
		}

		if (t1.z > t2.z)
		{
			tMin.z = t2.z;
			tMax.z = t1.z;
		}
		else
		{
			tMin.z = t1.z;
			tMax.z = t2.z;
		}

		float maxOfMin = -FLT_MAX;
		float minOfMax = FLT_MAX;

		// Min에서 가장 큰 값과 Max에서 가장 작은 값을 찾는다.
		if (maxOfMin < tMin.x)
			maxOfMin = tMin.x;

		if (minOfMax > tMax.x)
			minOfMax = tMax.x;

		if (maxOfMin < tMin.y)
			maxOfMin = tMin.y;

		if (minOfMax > tMax.y)
			minOfMax = tMax.y;

		if (maxOfMin < tMin.z)
			maxOfMin = tMin.z;

		if (minOfMax > tMax.z)
			minOfMax = tMax.z;


		if (maxOfMin > minOfMax)
			return false;


		if (minOfMax < 0.0f)
			return false;


		//SetHit(other->GetGameObject(), maxOfMin);
		currHitObjVec.push_back(std::make_pair(other->GetGameObject(), maxOfMin));

		return true;
	}

	bool RayCollider::CheckForSphere(std::shared_ptr<SphereCollider> other)
	{
		// 만날 수 있는 점의 위치
		float t0, t1;

		// 원의 중심으로 향하는 벡터
		Vector3 rayToSphere = other->GetWorldCenter() - worldCenter;

		// 해당 벡터를 dir에 사영.
		float projRayToSphere = rayToSphere.Dot(direction);

		// 원의 중심에서 레이 까지 수직인 선분의 길이
		float d = rayToSphere.Dot(rayToSphere) - projRayToSphere * projRayToSphere;

		// 이 길이가 반지름보다 크면. 만나지 않는다.
		if (d > other->radius)
			return false;

		// 중심에서 벡터로 내린 수선에서 원과 부딪히는 부분까지의 거리
		float solDist = sqrtf(other->radius - d);
		t0 = projRayToSphere - solDist;
		t1 = projRayToSphere + solDist;

		// t0이 t1보다 크면 -> 즉 뒤에 있으면.. 스왑한다.
		if (t0 > t1)
			std::swap(t0, t1);

		// 대신 0보다 작으면 t1을 대신 쓴다.
		if (t0 < 0)
		{
			t0 = t1;
			// 그럼에도 0보다 작으면. false를 리턴한다.
			if (t0 < 0)
				return false;
		}

		// 그 거리를 넣어준다.
		//SetHit(other->GetGameObject(), t0);
		currHitObjVec.push_back(std::make_pair(other->GetGameObject(), t0));

		return true;
	}

	void RayCollider::SetHit()
	{
		// 기존에 없다면..
		// 새로 hit 한 녀석의 거리가 더 가까우면 => 제일 앞에 있다.
		/*if ((hit.hitName != TEXT("HitNone") && hit.hitDist < dist) || dist < 0)
			return;

		hit.hitGameObject = gameObj;
		hit.hitTransform = gameObj->GetComponent<Transform>();
		hit.hitDist = dist;
		hit.hitName = gameObj->GetName();*/

		if (currHitObjVec.empty() == true)
			return;

		int idx = 0;
		float min = FLT_MAX;

		for (int i = 0; i < currHitObjVec.size(); i++)
		{
			if (min > currHitObjVec[i].second && currHitObjVec[i].second > 0)
			{
				min = currHitObjVec[i].second;
				idx = i;
			}
		}

		hit.hitGameObject = currHitObjVec[idx].first;
		hit.hitTransform = currHitObjVec[idx].first->GetComponent<Transform>();
		hit.hitDist = currHitObjVec[idx].second;
		hit.hitName = currHitObjVec[idx].first->GetName();
	}

	void RayCollider::ClearHit()
	{
		// Hit 초기화.
			// 길이 벡터 초기화
		currHitObjVec.clear();
	}

	bool RayCollider::CheckCollision(std::shared_ptr<ColliderBase> other)
	{
		// 원과.. 박스만 해준다.
		std::shared_ptr<SphereCollider> otherSphereCol = std::dynamic_pointer_cast<SphereCollider>(other);
		std::shared_ptr<BoxCollider> otherBoxCol = std::dynamic_pointer_cast<BoxCollider>(other);

		if (otherSphereCol != nullptr)
			return CheckForSphere(otherSphereCol);
		if (otherBoxCol != nullptr)
			return CheckForBox(otherBoxCol);

		return false;

	}

	/*void RayCollider::ColliderRender()
	{
		std::shared_ptr<GraphicsEngine> renderer = GraphicsManager::GetInstance()->GetRenderer();

		ResourceManager* resourceManager = renderer->GetResourceManager();

		Matrix lookAt = MatrixLookAt(worldCenter, direction, Vector::UnitY);

		Matrix world = MatrixInverse(lookAt);

		renderer->GraphicsDebugBeginEvent("Ray Line");
		renderer->DrawLine(resourceManager->GetBuffer("RayLineVB"), resourceManager->GetBuffer("RayLineIB"), color, MatrixTranspose(world));
		renderer->GraphicsDebugEndEvent();

	}*/

	void RayCollider::LateUpdate(float tick)
	{
		// 업데이트 처리
		// 월드 센터 업데이트
		worldCenter = localCenter + transform->GetWorldPosition();

		// direction 업뎃
		//direction = transform->GetLook();
	}
}
