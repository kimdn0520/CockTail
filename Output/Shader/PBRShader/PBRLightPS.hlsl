// Deferred Rendering을 통해 빛 연산을 하기 위한 Light Shader 입니다.
#include "PBRLightUtils.hlsli"

struct LightPixelIn
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};

cbuffer cbLight : register(b0)
{
    DirectionalLight DirLights[DIRECTION_LIGHT_COUNT];
    PointLight PointLights[POINT_LIGHT_COUNT];
    SpotLight SpotLights[SPOT_LIGHT_COUNT];

    float3 EyePosW; // CameraPos

    int DirLightCnt;
    int PointLightCnt;
    int SpotLightCnt;
}

// RenderTarget Texture

Texture2D Albedo : register(t0);

Texture2D DMRAO : register(t1); // depth, metallic, roughness, ambient occlusion

Texture2D Normal : register(t2);

Texture2D Position : register(t3);

Texture2D Emissive : register(t4);

SamplerState Sampler : register(s0);
SamplerComparisonState samLinearPointBoarder : register(s1);

float4 main(LightPixelIn lpin) : SV_Target
{
    float depth = DMRAO.Sample(Sampler, lpin.Tex).x;

    float3 normal = Normal.Sample(Sampler, lpin.Tex).xyz;

    float4 position = Position.Sample(Sampler, lpin.Tex).xyzw;

    float4 albedo = Albedo.Sample(Sampler, lpin.Tex).xyzw;

    float3 emissive = Emissive.Sample(Sampler, lpin.Tex).xyz;

    float shadowVal = 1.0f;

    // TODO : Shadow와 SSAO

    float ambientAccess = 1.f;

    // 금속성
    float metallic = DMRAO.Sample(Sampler, lpin.Tex).y;

	// 거칠기
    float roughness = DMRAO.Sample(Sampler, lpin.Tex).z;

	// 차폐도
    float ambientOcclusion = DMRAO.Sample(Sampler, lpin.Tex).w * ambientAccess;

    float3 toEye = normalize(EyePosW - position.xyz);

    bool isLight = position.w;

    float3 litColor = float3(0.0f, 0.0f, 0.0f);

    // 감마 Correction
    litColor = pow(litColor, 2.2f);

    if (isLight)
    {
        litColor = PBR_DirectionalLight(toEye, normal, DirLights[0],
			albedo.xyz, ambientOcclusion, roughness, metallic, shadowVal);

        litColor += PBR_PointLight(toEye, normal, PointLights, PointLightCnt, position.xyz,
			albedo.xyz, ambientOcclusion, roughness, metallic);

        litColor += PBR_SpotLight(toEye, normal, SpotLights, SpotLightCnt, position.xyz,
			albedo.xyz, ambientOcclusion, roughness, metallic);

       litColor += emissive;
    }
    else
    {
        litColor += albedo.xyz;
    }

    float3 ambientLighting = albedo * (1.1f - roughness) * 0.3f;

    litColor = pow(litColor, 1 / 2.2f);

    return float4(litColor + ambientLighting, 1.0f);
}