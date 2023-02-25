// Deferred Rendering을 통해 빛 연산을 하기 위한 Light Shader 입니다.
#include "PBRLightUtils.hlsli"

struct LightPixelIn
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
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

// 텍스쳐에 대한 정보를 담고 있는 texture 상수버퍼
	// 텍셀에 크기에 대한 정보를 담고 있다.
cbuffer cbTexture : register(b1)
{
    float4 textureInfo;
}

// AmbientColor에 대한 정보를 담고있는 Color 상수 버퍼
cbuffer cbColor : register(b2)
{
    float4 environmentColor;
}

// RenderTarget Texture

Texture2D Albedo : register(t0);

Texture2D DMRAO : register(t1); // depth, metallic, roughness, ambient occlusion

Texture2D Normal : register(t2);

Texture2D Position : register(t3);

Texture2D Emissive : register(t4);

Texture2D Shadow : register(t5);

// PointLight(CubeMap) Shadow
TextureCubeArray PointShadow : register(t6);

// SpotLight (TextureArray) Shadow
Texture2DArray SpotShadow : register(t7);

SamplerState Sampler : register(s0);
SamplerComparisonState samLinearPointBoarder : register(s1);

// Linear Space 값을 Gamma Space로
// hlsli에 있는 함수를 찾지 못해서 여기로..
float3 LinearToSRGB(float3 color)
{
    float3 sRGBLo = color * 12.92f;
    float3 sRGBHi = (pow(color, 1 / 2.4f) * 1.055f) - 0.055f;
    float3 sRGB = (color <= 0.0031308f) ? sRGBLo : sRGBHi;

    return sRGB;
}

float4 LinearToSRGB(float4 color)
{
    return float4(LinearToSRGB(color.rgb), color.a);
}

float4 main(LightPixelIn lpin) : SV_Target
{
    float depth = DMRAO.Sample(Sampler, lpin.Tex).x;

    float3 normal = Normal.Sample(Sampler, lpin.Tex).xyz;

    float4 position = Position.Sample(Sampler, lpin.Tex).xyzw;

    float4 albedo = Albedo.Sample(Sampler, lpin.Tex).xyzw;

    float3 emissive = Emissive.Sample(Sampler, lpin.Tex).xyz;

    float shadowVal = 1.0f;

    // TODO : Shadow와 SSAO
#ifdef IsShadow
    // 받아온 shadow Texture를 이용해준다.
    // 투영
    // Shadow 위치 정보를 다시 계산해준다.
        // Shadow VS의 내용과 동일하다.
    float4 shadow = mul(float4(position.xyz, 1.0f), DirLights[0].lightViewProj);

    shadow.xyz /= shadow.w;

    // NDC 공간에서 텍스쳐 공간으로 변환
    shadow.x = 0.5f * shadow.x + 0.5f;
    shadow.y = -0.5f * shadow.y + 0.5f;

    // 실제 그림자 계수 계산
    shadowVal = CalcShadowFactor(samLinearPointBoarder, Shadow, shadow, textureInfo);
#endif

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

    //albedo = pow(albedo, 2.2f);

    if (isLight)
    {
        litColor = PBR_DirectionalLight(toEye, normal, DirLights[0],
			albedo.xyz, ambientOcclusion, roughness, metallic, shadowVal);
        
        //litColor += PBR_PointLight(toEye, normal, PointLights, PointLightCnt, position.xyz,
		//	albedo.xyz, ambientOcclusion, roughness, metallic, shadowVal);

        /// Point Light
        float3 acc_color = float3(0.f, 0.f, 0.f);
        PointLight light;
        float4 fragToLight = float4(0.f, 0.f, 0.f, 0.f);

        // 기본적으로 위의 함수 내부의 내용을 밖으로 빼낸 것.
        [unroll]
        for (int i = 0; i < PointLightCnt; i++)
        {
            light = PointLights[i];

#ifdef IsPointShadow
			fragToLight = float4((position.xyz - light.position), 1.0f);

            if(light.isShadow != true)
                shadowVal = 1.0f;
            else
                shadowVal = CalcPointShadowFactor(samLinearPointBoarder, i, PointShadow, fragToLight, textureInfo, light.range);
#endif
            
            acc_color += PBR_OnePointLight(toEye, normal, light, position.xyz, albedo.xyz,
            ambientOcclusion, roughness, metallic, shadowVal);
        }

        litColor += acc_color;

        //litColor += PBR_SpotLight(toEye, normal, SpotLights, SpotLightCnt, position.xyz,
		//	albedo.xyz, ambientOcclusion, roughness, metallic);

        /// Spot Light
        acc_color = float3(0.f, 0.f, 0.f);
        SpotLight sLight;

        // 그림자 자체는 dir Light와 같게 계산해줍니다.
        [unroll]
        for (int j = 0; j < SpotLightCnt; j++)
        {
            sLight = SpotLights[j];
            
#ifdef IsSpotShadow
            float4 shadow = mul(float4(position.xyz, 1.0f), sLight.lightViewProj);

            shadow.xyz /= shadow.w;

		// NDC 공간에서 텍스쳐 공간으로 변환
            shadow.x = 0.5f * shadow.x + 0.5f;
            shadow.y = -0.5f * shadow.y + 0.5f;

		// 실제 그림자 계수 계산
        	if(sLight.isShadow != true)
                shadowVal = 1.0f;
            else
				shadowVal = CalcShadowFactor(samLinearPointBoarder, SpotShadow, shadow, textureInfo, j);
#endif
            acc_color += PBR_OneSpotLight(toEye, normal, sLight, position.xyz, albedo.xyz,
            ambientOcclusion, roughness, metallic, shadowVal);
        }

        litColor += acc_color;

    	litColor += emissive;
    }
    else
    {
        litColor += albedo.xyz;

        // 감마 Correction
			// PostProcessing을 위해 빼줍니다.
        //litColor = LinearToSRGB(litColor);

        return float4(litColor, 1.0f);
    }

    // 해당 부분을 하드코딩 해봅시다..
	//float3 ambientLighting = albedo.xyz * (1.1f - roughness) * 0.3f;
    float3 ambientLighting = (albedo.xyz * SRGBToLinear(float3(environmentColor.xyz))) * (1.1f - roughness) * 0.7f;

    // 감마 Correction
		// PostProcessing을 위해 빼줍니다.
    //litColor = LinearToSRGB(litColor);

    return float4(litColor + ambientLighting, 1.0f);
}