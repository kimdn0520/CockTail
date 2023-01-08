#include "GamePch.h"
#include "PhysicsManager.h"

#include "Object/GameObject.h"
#include "Component/ColliderBase.h"
#include "Component/RayCollider.h"

namespace GameEngineSpace
{
	std::shared_ptr<PhysicsManager> PhysicsManager::instance = nullptr;

	std::shared_ptr<PhysicsManager> PhysicsManager::GetInstance()
	{
		if(instance == nullptr)
			instance = std::make_shared<PhysicsManager>();

		return instance;
	}

	void PhysicsManager::Init()
	{
		// TODO 구획 분리 나눠서 처리.
	}

	void PhysicsManager::Update(float deltaTime, std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// 업데이트 전. 지난 프레임을 초기화.
		ResetColliders(colliderList);

		// 충돌 체크
		CheckCollisionAll(colliderList);

		// 이후 체크된 것을 브로드캐스트
		BroadCastTriggerEvents(colliderList);
	}

	void PhysicsManager::Release()
	{
		// TODO 나눈 구획 릴리즈.
	}

	void PhysicsManager::CheckCollisionAll(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// 모든 영역을 돌면서 충돌 처리 해준다.
		// 지금은 아무 생각 없이 n^2지만 공간 분할과 충돌 처리 가능한 것들만 처리해줄 필요가 있다.
		// TODO 공간 분할, 최적화 반드시 필요.

		for(size_t i = 0; i < colliderList.size(); i++)
		{
			for(size_t j = i + 1; j < colliderList.size(); j++)
			{
				std::shared_ptr<ColliderBase> collider = colliderList[i];
				std::shared_ptr<ColliderBase> otherCollider = colliderList[j];

				// 콜라이더가 사용가능하지 않으면.
				if(collider->GetIsEnable() == false || otherCollider->GetIsEnable() == false)
					continue;

				// 두 콜라이더가 같은 게임오브젝트 안에 있으면.. Continue
				if(collider->GetGameObject() == otherCollider->GetGameObject())
					continue;

				// 충돌한 게 부모면 continue;
					// 내가 상대방의 부모면 or 상대방이 나의 부모면..
				if(collider->GetGameObject() == otherCollider->GetGameObject()->GetParent()
				|| collider->GetGameObject()->GetParent() == otherCollider->GetGameObject())
					continue;
					
				if(collider->CheckCollision(otherCollider))
				{
					// 충돌 했으면.. 각각의 콜라이더의 충돌한 물체 벡터에 other을 넣어줍니다.
					collider->currColliedColList.push_back(otherCollider);
					otherCollider->currColliedColList.push_back(collider);

					// 디버그용 코드 => 현재 충돌 하고 있는가..
					collider->SetColor({1.0f, 0.f, 0.f});
					otherCollider->SetColor({1.0f, 0.f, 0.f});

					/*TCHAR buffer[256] = {};*/

					//_stprintf_s<256>(buffer, _T("Colided : %s <-> %s\n"), collider->GetGameObject()->GetName().c_str(), otherCollider->GetGameObject()->GetName().c_str());
					//OutputDebugString(buffer);
				}
			}
		}
	}

	void PhysicsManager::ResetColliders(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		for(auto& collider : colliderList)
		{
			// 돌면서 정리
			// 현재 프레임에 있던 콜라이더 들을 이전 프레임으로 넘겨주고.
			collider->prevColliedColList = collider->currColliedColList;

			// 현재 프레임은 클리어해준다.
			collider->ClearCurrColliedColList();

			std::shared_ptr<RayCollider> ray = std::dynamic_pointer_cast<RayCollider>(collider);
			// 레이면 Hit도 클리어.
			if (ray != nullptr)
				ray->ClearHit();
		}
	}

	void PhysicsManager::BroadCastTriggerEvents(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// 콜라이더 전체를 돌면서.. 트리거를 체크해줍니다.
		for(auto& collider : colliderList)
		{
			// 현재 콜라이더와 이번 프레임에 충돌한 콜라이더
			std::vector<std::shared_ptr<ColliderBase>>& currColList = collider->currColliedColList;

			// 콜라이더가 ray면 continue
			std::shared_ptr<RayCollider> ray = std::dynamic_pointer_cast<RayCollider>(collider);

			// 현재 충돌한게 없을 때.. 색 변경
			if (currColList.size() == 0)
			{
				collider->SetColor({ 1.0f, 1.0f, 1.0f });
			}

			if(ray != nullptr)
			{
				// 레이면 여기서 SetHit
				ray->SetHit();
				continue;
			}

			// 현재 콜라이더와 지난번 프레임에 충돌한 콜라이더
			std::vector<std::shared_ptr<ColliderBase>>& prevColList = collider->prevColliedColList;

			std::shared_ptr<GameObject> gameObject = collider->GetGameObject();

			// 두개를 비교해 가면서..
				// 현재 프레임에 충돌한 콜라이더
			for(auto& currCollider : currColList)
			{
				// 콜라이더가 ray면 continue
				ray = std::dynamic_pointer_cast<RayCollider>(currCollider);

				if (ray != nullptr)
					continue;

				// 현재 프레임에 충돌이 있는데 과거게 없으면 Enter. 둘다 있으면 stay
				if(std::find(prevColList.begin(), prevColList.end(), currCollider) != prevColList.end())
					gameObject->OnTriggerStay(currCollider);
				else
					gameObject->OnTriggerEnter(currCollider);
			}

			// 반대로 현재에 없는데 과거에 있으면..? Exit
			for(auto& prevCollider : prevColList)
			{
				// 콜라이더가 ray면 continue
				ray = std::dynamic_pointer_cast<RayCollider>(prevCollider);

				if (ray != nullptr)
					continue;

				// 현재 콜라이더 리스트를 시작부터 끝까지 뒤져서.. 지난번 콜라이더 리스트에 있는 애가 없으면..!
					// 얘가 이번 프레임에 나갔구나..!
				if(std::find(currColList.begin(), currColList.end(), prevCollider) == currColList.end())
					gameObject->OnTriggerExit(prevCollider);
			}
		}
	}
}
