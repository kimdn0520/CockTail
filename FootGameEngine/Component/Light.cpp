#include "GamePch.h"
#include "Light.h"
#include "GraphicsManager.h"
#include "Object/GameObject.h"

namespace GameEngineSpace
{
	Light::Light(std::weak_ptr<GameObject> _gameObject)	:
		ComponentBase(_gameObject, ComponentType::LIGHT)
		, lightType(LightType::Directional)
	{
		// 트랜스폼 캐싱
		transform = GetGameObject()->GetComponent<Transform>();
	}

	Light::~Light()
	{}

	void Light::Release()
	{
		__super::Release();
	}

	// TODO : LightViewProj (섀도우를 위한..)
	void Light::Update(float tick)
	{
		switch (lightType)
		{
			case LightType::Directional:
			{
				GraphicsManager::GetInstance()->PassDirectionalLight(color, direction, power, lightViewProj);
			}
			break;
			case LightType::Point:
			{
				position = transform->GetWorldPosition();
				GraphicsManager::GetInstance()->PassPointLight(color, position, power, range, lightViewProj);
			}
			break;
			case LightType::Spot:
			{
				GraphicsManager::GetInstance()->PassSpotLight(color, direction, power, halfAngle, range, lightViewProj);
			}
			break;
		}
	}

	void Light::DebugIMGUIRender(float tick)
	{
		auto imguiManager = GraphicsEngineSpace::ImGUIManager::GetInstance();
		// 여기서 IMGUI 시작 (Frame)
		// 아마 이제 여기서 조절하기 힘들거임..?
		if (imguiManager->Begin("DirLight"))
		{
			float LightColor[3] = { color.x, color.y, color.z };
			float LightDirection[3] = { direction.x, direction.y, direction.z };

			imguiManager->SliderFloat3("LightColor", LightColor, -1.f, 1.f);
			imguiManager->SliderFloat("LightSpecular", &power, 0.f, 10.f);
			imguiManager->SliderFloat3("LightDir", LightDirection, -1.f, 1.f);

			color.x = LightColor[0];
			color.y = LightColor[1];
			color.z = LightColor[2];

			direction.x = LightDirection[0];
			direction.y = LightDirection[1];
			direction.z = LightDirection[2];
		}
		imguiManager->End();
	}
}