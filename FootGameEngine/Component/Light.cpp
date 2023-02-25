#include "GamePch.h"
#include "Light.h"
#include "GraphicsManager.h"
#include "SceneBase.h"
#include "SceneManager.h"

#include "Object/GameObject.h"

namespace GameEngineSpace
{
	Light::Light(std::weak_ptr<GameObject> _gameObject) :
		ComponentBase(_gameObject, ComponentType::LIGHT)
		, lightType(LightType::Directional)
		, isShadow(true)
	{
		// 트랜스폼 캐싱
		transform = GetGameObject()->GetComponent<Transform>();
	}

	Light::~Light()
	{}

	void Light::Awake()
	{
		auto currentScene = SceneManager::GetInstance()->GetCurrentScene();

		if (currentScene == nullptr)
		{
			printf("NOT FOUND SCENE\n");
			return;
		}

		// Type에 따라서 씬에 추가해줍니다.
		switch (lightType)
		{
		case LightType::Point:
		{
			currentScene->AddPointLight(this->shared_from_this());
			break;
		}

		case LightType::Spot:
		{
			currentScene->AddSpotLight(this->shared_from_this());
			break;
		}

		case LightType::Directional:
		default:
			break;
		}
	}

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
			// view 생성
			auto lightPos = -2.0f * 50.f * direction;

			if (lightPos != Vector3::Zero)
			{
				Matrix view = XMMatrixLookAtLH(lightPos, Vector3{ 0.5f, -0.5f, 0.5f }, Vector3{ 0.0f, 1.0f, 0.0f });

				Vector3 newCenter = XMVector3TransformCoord(Vector3{ 0.5f, -0.5f, 0.5f }, view);

				float l = newCenter.x - 50.f;
				float b = newCenter.y - 50.f;
				float n = newCenter.z - 50.f;
				float r = newCenter.x + 50.f;
				float t = newCenter.y + 50.f;
				float f = newCenter.z + 50.f;

				Matrix proj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

				lightViewProj = view * proj;
			}

			GraphicsManager::GetInstance()->PassDirectionalLight(color, direction, power, lightViewProj);
		}
		break;

		case LightType::Point:
		{
			position = transform->GetWorldPosition();

			// light view proj 벡터 생성
			std::vector<Matrix> lightViewProjMatrix(6);

			// 우선은 임시 변수 기입
				// pointLight의 원점에서 월드 x, -x, y, -y, z, -z 방향으로 향하는 절두체를 그린다고 생각하면 된다.
			if (position != Vector3::Zero)
			{
				// 바라보는 방향을 lightPos 기준으로 정해진다.
				// X를 바라보는 방향 + 월드의 UP은 같다.
				Matrix viewX = XMMatrixLookAtLH(position, position + Vector3{ 1.0f, 0.f, 0.f }, Vector3{ 0.f, 1.0f, 0.f });
				// -X를 바라보는 방향
				Matrix viewXReci = XMMatrixLookAtLH(position, position + Vector3{ -1.0f, 0.f, 0.f }, Vector3{ 0.f, 1.0f, 0.f });
				// Z를 바라보는 방향
				Matrix viewZ = XMMatrixLookAtLH(position, position + Vector3{ 0.0f, 0.f, 1.f }, Vector3{ 0.f, 1.0f, 0.f });
				// -Z를 바라보는 방향
				Matrix viewZReci = XMMatrixLookAtLH(position, position + Vector3{ 0.0f, 0.f, -1.f }, Vector3{ 0.f, 1.0f, 0.f });
				// Y를 바라보는 방향 + 월드 UP의 경우 오류가 있으면 변경해야함.
				Matrix viewY = XMMatrixLookAtLH(position, position + Vector3{ 0.f, 1.f, 0.f }, Vector3{ 0.f, 0.f, -1.f });
				// -Y를 바라보는 방향
				Matrix viewYReci = XMMatrixLookAtLH(position, position + Vector3{ 0.f, -1.f, 0.f }, Vector3{ 0.f, 0.f, 1.f });

				// projection의 경우, far를 range 만큼 잡으면 괜찮지 않을까..?

				// 높이와 길이가 같다고 가정한다.
				float n = 1.f;
				float f = range;
				if (f < 1.0f)
					f = 1.0001f;
				// 내부의 시야각은 90도로 해줍니다 => 90도를 라디안으로 (Pi / 2)
				Matrix proj = XMMatrixPerspectiveFovLH(0.5f * FootMath::Pi, 1.f, n, f);

				// 각각을 곱해서 vector에 넣어본다.
				// X
				lightViewProjMatrix[0] = viewX * proj;
				// -X
				lightViewProjMatrix[1] = viewXReci * proj;
				// Y
				lightViewProjMatrix[2] = viewY * proj;
				// -Y
				lightViewProjMatrix[3] = viewYReci * proj;
				// Z
				lightViewProjMatrix[4] = viewZ * proj;
				// -Z
				lightViewProjMatrix[5] = viewZReci * proj;
			}

			GraphicsManager::GetInstance()->PassPointLight(color, position, power, range, isShadow, lightViewProjMatrix);
		}
		break;

		case LightType::Spot:
		{
			position = transform->GetWorldPosition();

			// Spot Light의 Light View Proj의 경우, position과 방향 벡터와 앵글을 통해 만들어 줍니다.
			if (position != Vector3::Zero)
			{
				// 우선 View.. => 포지션에서 Direction 방향을 보는..
				Vector3 focusDir = XMVector3Normalize(direction);

				// Direction이 GetLook을 통해 가져와지기 때문에..
				// 해당 조명의 Up Vector는 GetUp을 통해 가져올 수 있을 것이다.
				Matrix view = XMMatrixLookAtLH(position, position + focusDir, transform->GetUp());

				// Perspective 형식의 투영 행렬을 만들어줍니다.
					// 역시 높이와 길이가 같다고 가정해줍니다. (빛으로 생성되는 원이 타원이 아니라 가정)
				float n = 1.0f;
				float f = range;
				if (f < 1.f)
					f = 1.0001f;
				// 라디안으로 바꿔준 빛의 영역(각도)
				float degToRad = 2.0f * FootMath::Pi * (outerSpotAngle / 360.f);
				if (degToRad == 0.f)
					degToRad = 0.0001f;
				Matrix proj = XMMatrixPerspectiveFovLH(degToRad, 1.0f, n, f);

				lightViewProj = view * proj;
			}

			GraphicsManager::GetInstance()->PassSpotLight(color, position, direction, power, innerSpotAngle, outerSpotAngle, range, isShadow, lightViewProj);
		}
		break;
		}
	}

	void Light::DebugIMGUIRender(float tick)
	{
		auto imguiManager = GraphicsEngineSpace::ImGUIManager::GetInstance();
		// 여기서 IMGUI 시작 (Frame)
		// 아마 이제 여기서 조절하기 힘들거임..?

		if (this->lightType == LightType::Directional)
		{
			if (imguiManager->TreeNode("Light In Scene", "DirLight" + std::to_string(GetObjectID())))
			{
				float LightColor[3] = { color.x, color.y, color.z };
				float LightDirection[3] = { direction.x, direction.y, direction.z };

				auto pos = transform->GetWorldPosition();

				float LightPos[3] = { pos.x, pos.y, pos.z };

				imguiManager->SliderFloat3("LightColor", LightColor, 0.f, 1.f);
				imguiManager->DragFloat("LightSpecular", &power, 0.f, 10.f);
				imguiManager->SliderFloat3("LightDir", LightDirection, -1.f, 1.f);
				imguiManager->DragFloat3("LightPos", LightPos);

				color.x = LightColor[0];
				color.y = LightColor[1];
				color.z = LightColor[2];

				direction.x = LightDirection[0];
				direction.y = LightDirection[1];
				direction.z = LightDirection[2];

				pos.x = LightPos[0];
				pos.y = LightPos[1];
				pos.z = LightPos[2];

				transform->SetPosition(pos);

				imguiManager->TreePop();
				imguiManager->Separator();
			}
		}

		if (this->lightType == LightType::Point)
		{
			if (imguiManager->TreeNode("Light In Scene", "PointLight" + std::to_string(GetObjectID())))
			{
				float LightColor[3] = { color.x, color.y, color.z };
				float LightDirection[3] = { direction.x, direction.y, direction.z };

				auto pos = transform->GetWorldPosition();

				float LightPos[3] = { pos.x, pos.y, pos.z };

				imguiManager->SliderFloat3("LightColor", LightColor, 0.f, 1.f);
				imguiManager->DragFloat("LightSpecular", &power);
				imguiManager->SliderFloat("LightRange", &range, 0.f, 100.f);
				imguiManager->SliderFloat3("LightDir", LightDirection, -1.f, 1.f);
				imguiManager->DragFloat3("LightPos", LightPos);

				color.x = LightColor[0];
				color.y = LightColor[1];
				color.z = LightColor[2];

				direction.x = LightDirection[0];
				direction.y = LightDirection[1];
				direction.z = LightDirection[2];

				pos.x = LightPos[0];
				pos.y = LightPos[1];
				pos.z = LightPos[2];

				transform->SetPosition(pos);

				imguiManager->TreePop();
				imguiManager->Separator();
			}
		}

		if (this->lightType == LightType::Spot)
		{
			if (imguiManager->TreeNode("Light In Scene", "SpotLight" + std::to_string(GetObjectID())))
			{
				float LightColor[3] = { color.x, color.y, color.z };
				float LightDirection[3] = { direction.x, direction.y, direction.z };

				auto pos = transform->GetWorldPosition();

				float LightPos[3] = { pos.x, pos.y, pos.z };

				imguiManager->SliderFloat3("LightColor", LightColor, 0.f, 1.f);
				imguiManager->DragFloat("LightSpecular", &power);
				imguiManager->SliderFloat("LightRange", &range, 0.f, 90.f);
				imguiManager->SliderFloat("LightInnerAngle", &innerSpotAngle, 0.f, outerSpotAngle);
				imguiManager->SliderFloat("LightOuterAngle", &outerSpotAngle, innerSpotAngle, 90.f);
				imguiManager->DragFloat3("LightDir", LightDirection, -1.f, 1.f);
				imguiManager->DragFloat3("LightPos", LightPos);

				color.x = LightColor[0];
				color.y = LightColor[1];
				color.z = LightColor[2];

				direction.x = LightDirection[0];
				direction.y = LightDirection[1];
				direction.z = LightDirection[2];

				pos.x = LightPos[0];
				pos.y = LightPos[1];
				pos.z = LightPos[2];

				transform->SetPosition(pos);

				imguiManager->TreePop();
				imguiManager->Separator();
			}
		}

	}
}