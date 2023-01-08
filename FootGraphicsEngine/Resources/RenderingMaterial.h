#pragma once
#include "Resource.h"

namespace GraphicsEngineSpace
{
	class Texture;

	class RenderingMaterial : public GraphicsEngineSpace::Resource
	{
	public:
		RenderingMaterial()
			: Resource(GraphicsEngineSpace::RESOURCE_TYPE::MATERIAL)
			, albedoTex(nullptr)
			, normalTex(nullptr)
			, metallicTex(nullptr)
			, roughnessTex(nullptr)
			, aoTex(nullptr)
			, emissiveTex(nullptr)
			, cubeTex(nullptr)
		{}
		~RenderingMaterial() {};

	public:
		std::shared_ptr<Texture> albedoTex;
		std::shared_ptr<Texture> normalTex;
		std::shared_ptr<Texture> metallicTex;
		std::shared_ptr<Texture> roughnessTex;
		std::shared_ptr<Texture> aoTex;
		std::shared_ptr<Texture> emissiveTex;
		std::shared_ptr<Texture> cubeTex;

		DirectX::SimpleMath::Vector4 material_Ambient = { 0.f, 0.f, 0.f, 0.f };
		DirectX::SimpleMath::Vector4 material_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::SimpleMath::Vector4 material_Specular = { 0.f, 0.f, 0.f, 0.f };
		DirectX::SimpleMath::Vector4 material_Emissive = { 0.f, 0.f, 0.f, 0.f };

		float emissionFactor = 0.f;			// 방출 크기

		float material_Transparency = 0.f;	// 투명도

		float material_Reflectivity = 0.f;	// 반사율

		float metallic = 0.1f;

		float roughness = 0.f;
	};
}

