#include "pch.h"
#include "PhysX/EventCallback.h"

MoolyEngine::EventCallback::~EventCallback()
{
	contactEvents.clear();

	triggerEvents.clear();
	triggerPersistEvents.clear();
}

void MoolyEngine::EventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair& contactPair = pairs[i];

		if (pairHeader.actors[0]->userData == nullptr || pairHeader.actors[1]->userData == nullptr)
		{
			return;
		}

		auto foundEvent0 = contactEvents.find(((UserData*)pairHeader.actors[0]->userData)->m_name);
		auto foundEvent1 = contactEvents.find(((UserData*)pairHeader.actors[1]->userData)->m_name);
		bool foundEventTrigger0 = false;
		bool foundEventTrigger1 = false;

		if (foundEvent0 != contactEvents.end())
		{
			foundEventTrigger0 = true;
		}

		if (foundEvent1 != contactEvents.end())
		{
			foundEventTrigger1 = true;
		}

		if (foundEventTrigger0 == false && foundEventTrigger1 == false)
		{
			continue;
		}

		if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (foundEventTrigger0)
			{
				for (auto events : foundEvent0->second)
				{
					events->OnCollisionEnter(((UserData*)pairHeader.actors[1]->userData)->m_name);
				}
			}

			if (foundEventTrigger1)
			{
				for (auto events : foundEvent1->second)
				{
					events->OnCollisionEnter(((UserData*)pairHeader.actors[0]->userData)->m_name);
				}
			}
		}

		if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			if (foundEventTrigger0)
			{
				for (auto events : foundEvent0->second)
				{
					events->OnCollisionPersist(((UserData*)pairHeader.actors[1]->userData)->m_name);
				}
			}

			if (foundEventTrigger1)
			{
				for (auto events : foundEvent1->second)
				{
					events->OnCollisionPersist(((UserData*)pairHeader.actors[0]->userData)->m_name);
				}
			}
		}

		if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (foundEventTrigger0)
			{
				for (auto events : foundEvent0->second)
				{
					events->OnCollisionExit(((UserData*)pairHeader.actors[1]->userData)->m_name);
				}
			}
			if (foundEventTrigger1)
			{
				for (auto events : foundEvent1->second)
				{
					events->OnCollisionExit(((UserData*)pairHeader.actors[0]->userData)->m_name);
				}
			}
		}
	}
}


void MoolyEngine::EventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
		{
			continue;
		}

		if (pairs[i].triggerActor->userData == nullptr || pairs[i].otherActor->userData == nullptr)
		{
			continue;
		}

		auto foundEvent = triggerEvents.find(((UserData*)pairs[i].triggerActor->userData)->m_name);

		std::string triggerObject = ((UserData*)pairs[i].triggerActor->userData)->m_name;
		std::string hitObject = ((UserData*)pairs[i].otherActor->userData)->m_name;

		if (triggerEvents.end() == foundEvent)
		{
			continue;
		}

		else
		{
			for (auto triggerEvents : foundEvent->second)
			{
				if (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND == pairs->status)
				{
					triggerEvents->OnTriggerEnter(hitObject);
					auto foundPersistEvent = triggerPersistEvents.find(triggerObject);
					foundPersistEvent->second.push_back(hitObject);
				}

				if (physx::PxPairFlag::eNOTIFY_TOUCH_LOST == pairs->status)
				{
					triggerEvents->OnTriggerExit(hitObject);
					auto foundPersistEvent = triggerPersistEvents.find(triggerObject);
					foundPersistEvent->second.remove(hitObject);
				}
			}
		}
	}
}

bool MoolyEngine::EventCallback::AddColliderEvent(std::string eventHolder, IEventCollider* callBackClass)
{
	auto foundEvent = contactEvents.find(eventHolder);
	if (contactEvents.end() == foundEvent)
	{
		contactEvents.emplace(eventHolder, std::vector<IEventCollider*>{callBackClass});
		return true;
	}

	foundEvent->second.emplace_back(callBackClass);
	return true;

}

bool MoolyEngine::EventCallback::AddTriggerEvent(std::string eventHolder, IEventTrigger* callBackClass)
{
	auto foundEvent = triggerEvents.find(eventHolder);
	if (triggerEvents.end() == foundEvent)
	{
		triggerEvents.emplace(eventHolder, std::vector<IEventTrigger*>{callBackClass});
		triggerPersistEvents.emplace(eventHolder, std::list<std::string>());
		return true;
	}

	foundEvent->second.emplace_back(callBackClass);
	return true;
}

void MoolyEngine::EventCallback::DeleteEvents(std::string eventHolder)
{
	UINT foundTriggerEventCnt = 0;
	UINT foundContactEventCnt = 0;

	contactEvents.erase(eventHolder);
	triggerEvents.erase(eventHolder);
	triggerPersistEvents.erase(eventHolder);
}

void MoolyEngine::EventCallback::CallOnCollisionEnter()
{

}

void MoolyEngine::EventCallback::CallonTriggerPersist()
{
	for (auto& _trigger : triggerPersistEvents)
	{
		if (_trigger.second.empty() == false)
		{
			auto _foundedEvent = triggerEvents.find(_trigger.first);

			if (triggerEvents.end() == _foundedEvent)
			{
				continue;
			}

			else
			{
				for (auto& _event : _foundedEvent->second)
				{
					for (auto& _hitObj : _trigger.second)
					{
						_event->OnTriggerPersist(_hitObj);
					}
				}
			}
		}
	}
}
