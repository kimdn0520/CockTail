#pragma once

using namespace DirectX;

#define DIRECTION_LIGHT_COUNT 1
#define POINT_LIGHT_COUNT 30
#define SPOT_LIGHT_COUNT 30

namespace GraphicsEngineSpace
{	
	// PBR용 및 추가
	namespace PBR
	{
		__declspec(align(16)) struct DirectionalLight
		{
			DirectionalLight()
				: color(SimpleMath::Vector3::Zero)
				, power(0.0f)
				, direction(SimpleMath::Vector3::Zero)
				, pad(0.0f)
				, lightViewProj(SimpleMath::Matrix::Identity)
			{
				ZeroMemory(this, sizeof(this));
			}

			SimpleMath::Vector3 color;
			float power;

			SimpleMath::Vector3 direction;
			float pad;

			SimpleMath::Matrix lightViewProj;
		};

		__declspec(align(16)) struct PointLight
		{
			PointLight()
				: color(SimpleMath::Vector3::Zero)
				, power(0.0f)
				, position(SimpleMath::Vector3::Zero)
				, range(0.0f)
				, padVector(SimpleMath::Vector3::Zero)
				, isShadow(1.0f)
			{
				ZeroMemory(this, sizeof(this));

				for (int i = 0; i < 6; i++)
				{
					lightViewProj[i] = SimpleMath::Matrix::Identity;
				}
			}

			SimpleMath::Vector3 color;
			float power;

			SimpleMath::Vector3 position;
			float range;

			SimpleMath::Vector3 padVector;
			float isShadow;		

			// 각 면에 따른 6개의 매트릭스가 존재
			SimpleMath::Matrix lightViewProj[6];
		};

		__declspec(align(16)) struct SpotLight
		{
			SpotLight()
				: color(SimpleMath::Vector3::Zero)
				, power(0.0f)
				, direction(SimpleMath::Vector3::Zero)
				, innerAngle(0.0f)
				, padVector(SimpleMath::Vector2::Zero)
				, outerAngle(0.0f)
				, isShadow(1.0f)
				, position(SimpleMath::Vector3::Zero)
				, range(0.0f)
				, lightViewProj(SimpleMath::Matrix::Identity)
			{
				ZeroMemory(this, sizeof(this));
			}

			SimpleMath::Vector3 color;
			float power;

			SimpleMath::Vector3 direction;
			float innerAngle;

			SimpleMath::Vector2 padVector;
			float outerAngle;
			float isShadow;

			SimpleMath::Vector3 position;
			float range;

			SimpleMath::Matrix lightViewProj;
		};
	}

	// 내부에서 사용해줄 constant Buffer.
	__declspec(align(16)) struct cbLight
	{
		PBR::DirectionalLight DirLights[DIRECTION_LIGHT_COUNT];
		PBR::PointLight PointLights[POINT_LIGHT_COUNT];
		PBR::SpotLight SpotLights[SPOT_LIGHT_COUNT];

		SimpleMath::Vector3 EyePosW;

		int DirLightCnt;
		int PointLightCnt;
		int SpotLightCnt;
	};
}
