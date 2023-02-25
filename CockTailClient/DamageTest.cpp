#include "pch.h"
#include "DamageTest.h"
#include "MoolyInfo.h"

ClientSpace::DamageTest::DamageTest(std::weak_ptr<GameObject> _object)
	: ComponentBase(_object)
{
	targetPos = Vector3::Zero;
	bDamaged = false;
}

ClientSpace::DamageTest::~DamageTest()
{

}

void ClientSpace::DamageTest::Awake()
{
	rigid = GetGameObject()->GetComponent<Rigidbody>();
	transform = GetGameObject()->GetComponent<Transform>();
	std::string enemyLayer = "Enemy";
	GetGameObject()->GetComponent<Collider>()->SetLayer(enemyLayer);
}

void ClientSpace::DamageTest::Start()
{

}

void ClientSpace::DamageTest::Update(float tick)
{

}

void ClientSpace::DamageTest::LateUpdate(float tick)
{
	AddForce();
}

void ClientSpace::DamageTest::AddForce()
{
	if (bDamaged == true)
	{
		if (targetPos == Vector3::Zero)
			return;

		Vector3 _result = transform->GetWorldPosition() - targetPos;
		_result.Normalize();
		_result *= 1000;
		rigid->AddForce({ _result.x, _result.y, _result.z });
		bDamaged = false;
		targetPos = Vector3::Zero;
	}
}