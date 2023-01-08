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
		struct DirectionalLight
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

		struct PointLight
		{
			PointLight()
				: color(SimpleMath::Vector3::Zero)
				, power(0.0f)
				, position(SimpleMath::Vector3::Zero)
				, range(0.0f)
				, lightViewProj(SimpleMath::Matrix::Identity)
			{
				ZeroMemory(this, sizeof(this));
			}

			SimpleMath::Vector3 color;
			float power;

			SimpleMath::Vector3 position;
			float range;

			SimpleMath::Matrix lightViewProj;
		};

		struct SpotLight
		{
			SpotLight()
				: color(SimpleMath::Vector3::Zero)
				, power(0.0f)
				, direction(SimpleMath::Vector3::Zero)
				, halfAngle(0.0f)
				, position(SimpleMath::Vector3::Zero)
				, range(0.0f)
				, lightViewProj(SimpleMath::Matrix::Identity)
			{
				ZeroMemory(this, sizeof(this));
			}

			SimpleMath::Vector3 color;
			float power;

			SimpleMath::Vector3 direction;
			float halfAngle;

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
