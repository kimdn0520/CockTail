#include "pch.h"
#include "Trigger.h"

Trigger::Trigger(std::weak_ptr<GameObject> _object)
	: ComponentBase(_object)
{

}

Trigger::~Trigger()
{

}

void Trigger::Awake()
{

}

void Trigger::Start()
{

}

void Trigger::OnTriggerEnter(std::string name)
{

}

void Trigger::OnTriggerPersist(std::string name)
{

}

void Trigger::OnTriggerExit(std::string name)
{

}
