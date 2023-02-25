// PBR 빛 연산에서 필요한 각종 함수들을 모아뒀습니다.

// 레퍼런스 : https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl

// Specular coefficiant - fixed reflectance value for non-metals
#define F_ZERO float3(0.04f, 0.04f, 0.04f)
#define PI          3.14159265359f
#define EPSILON 0.000001f

#define DIRECTION_LIGHT_COUNT	1
#define POINT_LIGHT_COUNT		30
#define SPOT_LIGHT_COUNT		30

// Unity Spec 성분을 가지고 오기 위한 노력..
#define kDielectricSpec float4(0.04f, 0.04f, 0.04f, 1.0f - 0.04f) // standard dielectric reflectivity coef at incident angle (= 4%)

// Light Informations For PBR
struct DirectionalLight
{
    float3 color; // 빛의 색
    float power; // 빛의 세기

    float3 direction; // 방향
    float pad;

    float4x4 lightViewProj;
};

struct PointLight
{
    float3 color;
    float power;

    float3 position; // 위치
    float range; // 범위

    float3 pad;
    float isShadow;   // 그림자 여부

	// 각 면에 따른 6개의 매트릭스가 존재
    float4x4 lightViewProj[6];
};

struct SpotLight
{
    float3 color;
    float power;

    float3 direction; // 방향
    float innerAngle; // 내부적으로 빛의 감쇠를 넣어줄 수치

    float2 pad;
    float outerAngle; // Spot Angle => 스포트 라이트가 퍼지는 범위
    float isShadow;   // 그림자 여부 1.0f 있음 0.f 없음

    float3 position; // 위치
    float range; // 반지름 범위

    float4x4 lightViewProj;
};

/// Point Light 그림자를 위한 함수 전방 선언
float CalcDepthInShadow(const in float3 fragPos, float range);
float CalcPointShadowFactor(SamplerComparisonState samShadow, int index, TextureCubeArray cubeShadowMap, float4 shadowPosH, float4 textureInfo, float range);
float3 SRGBToLinear(float3 color);
float4 SRGBToLinear(float4 color);

// Diffuse BRDF
// Burley B. "Physically Based Shading at Disney"
// SIGGRAPH 2012 Course: Practical Physically Based Shading in Film and Game Production, 2012.
float3 Disney_Diffuse(in float roughnessPercent, in float3 diffuseColor, in float NdotL, in float NdotV, in float LdotH)
{
    float energyBias = lerp(0.0f, 0.5f, roughnessPercent);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughnessPercent);

    float fd90 = energyBias + 2.0f * roughnessPercent * LdotH * LdotH;

    float lightScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotL, 5.0f);
    float viewScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotV, 5.0f);

    return diffuseColor * lightScatter * viewScatter * energyFactor;
}

//float D_GGX(float roughness, float NoH, const float3 NxH)
//{
//    float a = NoH * roughness;
//    float k = roughness / (dot(NxH, NxH) + a * a);
//    float d = k * k * (1.0 / PI);
//    return min(d, 65504.0);
//}

// GGX Specular D (normal distribution)
// https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
float D_GGX(in float roughness2, in float NdotH)
{
    const float alpha = roughness2 * roughness2;
    const float NdotH2 = NdotH * NdotH; // NdotH2 = NdotH^2

    const float lower = (NdotH2 * (alpha - 1.0f)) + 1.0f;
    //const float lower = NdotH2 * alpha + (1.0f - NdotH2);
    return alpha / (PI * lower * lower);
}

// Shlick's approximation of Fresnel By Unity Engine - Specular_F_Fresnel_Shlick
float3 F_Shlick(in float3 specularColor, in float HdotV)
{
    float FC = pow(1.0f - HdotV, 5.0f);
    return specularColor + (float3(1.0f, 1.0f, 1.0f) - specularColor) * FC;
}

// Schlick-Smith specular G (visibility) By Unity Version
float G_Smith(float roughness2, float NdotV, float NdotL)
{
    float SmithV = NdotL * sqrt(NdotV * (NdotV - NdotV * roughness2) + roughness2);
    float SmithL = NdotV * sqrt(NdotL * (NdotL - NdotL * roughness2) + roughness2);

    return 0.5f / max(SmithV + SmithL, 1e-5f);
}

float GGX_Geometry(float cosThetaN, float roughness4)
{
    float cosTheta_sqr = saturate(cosThetaN * cosThetaN);
    float tan2 = (1.0f - cosTheta_sqr) / cosTheta_sqr;

    return 2.0f / (1.0f + sqrt(1.0f + roughness4 * tan2));
}

float G_GGX(in float roughness2, in float NdotV, in float NdotL)
{
    float alpha = roughness2 * roughness2;

    return GGX_Geometry(NdotV, alpha) * GGX_Geometry(NdotL, alpha);
}

float G_SmithShlick(in float roughness2, in float NdotV, in float NdotL)
{
    float r = sqrt(roughness2) + 1.0f;
    float k = (r * r) / 8.0f;

    float SmithV = NdotV / (NdotV * (1.0 - k) + k);
    float SmithL = NdotL / (NdotL * (1.0 - k) + k);

    return SmithV * SmithL;
}


float3 BRDF(in float roughness2, in float metallic, in float3 diffuseColor, in float3 specularColor, in float NdotH, in float NdotV, in float NdotL, in float HdotV)
{
    // Distribution & Geometry & Fresnel
    /*
    */
    float D = D_GGX(roughness2, NdotH); // 미세면 분포 함수
    float G = G_GGX(roughness2, NdotV, NdotL); // 기하 감쇠 함수
    float3 F = F_Shlick(specularColor, HdotV); // 프레넬 함수

    /*float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= 1.0 - metallic;*/

    float3 kD = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);

    // Diffuse & Specular factors
    float denom = max(4.0f * NdotV * NdotL, 0.001f); // 0.001f just in case product is 0 , 일반적으로 스페큘러는 4로 나누는 쪽이 많은듯
    float3 specular_factor = saturate((D * F * G) / denom);
    float3 diffuse_factor = kD * diffuseColor;
    
    return (diffuse_factor + specular_factor) * NdotL;
}

// Unity의 BRDF Specular Term을 가져와 보았습니다.
	// HoV == Half dot View => Light dot Half와 같은 결과
float DirectBRDFSpecular(in float roughness2, in float3 NoH, in float3 HoV)
{
    const float roughness2MinusOne = roughness2 - 1.0f;
    // 제곱한 값을 보내주기 때문에.. 음수가 될 수 없다.
    const float normalizationTerm = sqrt(roughness2) * 4.0f + 2.0f;

    // 아래는 Unity의 주석입니다..
    // 기본적으로 GGX를 사용하지만..
    // GGX Distribution multiplied by combined approximation of Visibility and Fresnel
    // BRDFspec = (D * V * F) / 4.0
    // D = roughness^2 / ( NoH^2 * (roughness^2 - 1) + 1 )^2
    // V * F = 1.0 / ( LoH^2 * (roughness + 0.5) )
    // See "Optimizing PBR for Mobile" from Siggraph 2015 moving mobile graphics course
    // https://community.arm.com/events/1155

    // Final BRDFspec = roughness^2 / ( NoH^2 * (roughness^2 - 1) + 1 )^2 * (LoH^2 * (roughness + 0.5) * 4.0)
    // We further optimize a few light invariant terms
    // brdfData.normalizationTerm = (roughness + 0.5) * 4.0 rewritten as roughness * 4.0 + 2.0 to a fit a MAD.
    float d = NoH * NoH * roughness2MinusOne + 1.00001f;
    float d2 = d * d;

    float HoV2 = HoV * HoV;

    return roughness2 / (d2 * max(0.1f, HoV2) * normalizationTerm);
}


// Unity의 요소들을 가져온 새로운 BRDF 함수입니다.
float3 BRDF(in float roughness2, in float metallic, in float3 diffuseColor, in float3 specularColor, in float NdotH, in float NdotL, in float HdotV)
{
    // 위의 식을 이용한 Specular 계산.
    float3 specular_factor = DirectBRDFSpecular(roughness2, NdotH, HdotV);
    
    // metallic을 이용한 diffuse Factor 계산.. in unity
    float oneMinusDielectricSpec = kDielectricSpec.a;
    float oneMinusReflectivity = oneMinusDielectricSpec - metallic * oneMinusDielectricSpec;
    float3 diffuse_factor = diffuseColor * oneMinusReflectivity;

    return (diffuse_factor + specular_factor * specularColor) * NdotL;
}


float3 PBR_DirectionalLight(
    in float3 V, in float3 N, in DirectionalLight light,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    // Burley roughness bias
    const float roughness2 = max(roughness * roughness, 0.01f);

    // Blend base colors
    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
    const float3 c_spec = lerp(kDielectricSpec.rgb, albedo, metallic) * ambientOcclusion;

    // Calculate Directional Light
    const float3 L = normalize(-light.direction);
    const float3 H = normalize(V + L);

    // products
    const float NdotL = max(dot(N, L), EPSILON);
    const float NdotV = abs(dot(N, V)) + EPSILON;
    const float NdotH = max(dot(N, H), EPSILON);
    const float HdotV = max(dot(H, V), EPSILON);

    // BRDF
    //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
    float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);

    // Directional light
    light.color = SRGBToLinear(light.color);
    acc_color += light.color.rgb * light.power * shadow * brdf_factor;

    return acc_color;
}

float3 PBR_PointLight(
    in float3 V, in float3 N, in PointLight lights[POINT_LIGHT_COUNT], in uint lightCount, in float3 position,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    PointLight light;

    [unroll]
    for (uint i = 0; i < lightCount; i++)
    {
        light = lights[i];

        // Light vector (to light)
        float3 lightVec = light.position - position;
        float distance = length(lightVec);

        if (distance > light.range)
            continue;

        const float3 L = normalize(lightVec);
        const float3 H = normalize(V + L);

        // products
        const float NdotL = max(dot(N, L), EPSILON);
        const float NdotV = max(dot(N, V), EPSILON);
        const float NdotH = max(dot(N, H), EPSILON);
        const float HdotV = max(dot(H, V), EPSILON);

        // Attenuation

        /// 기존 PBR Point Light Attenuation
        //float DistToLightNorm = 1.0 - saturate(distance * (1.0f / light.range));
        //float Attn = DistToLightNorm * DistToLightNorm;

        /// Unity 커스텀 SRP 블로그에서 가져온 Attenuation
        float distSqr = max(dot(lightVec, lightVec), 0.00001f);
        float reciRSqr = 1.f / max((light.range * light.range), 0.00001f);
        float rangeAttn = pow(saturate(1.0f - pow((distSqr * reciRSqr), 2.0f)), 2.0f);
        float Attn = rangeAttn / distSqr;

        /// Unity URP 내부의 hlsl에서 가져온 Attenuation
        //float distSqr = max(dot(lightVec, lightVec), 0.00001f);
        //float rSqr = max((light.range * light.range), 0.00001f);
        //float lightAttn = rcp(distSqr);
        //float fadeDistance = 0.8f * 0.8f * rSqr;        
        //half smoothFactor = half(saturate((rSqr - distSqr) / (rSqr - fadeDistance)));
        //float Attn = lightAttn * smoothFactor;

        float3 radiance = Attn * light.power;

        // Burley roughness bias
        const float roughness2 = max(roughness * roughness, 0.01f);

        // Blend base colors
        const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
        const float3 c_spec = lerp(F_ZERO, albedo, metallic) * ambientOcclusion;

        // BRDF
        //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
        float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);

        // Point light
        light.color = SRGBToLinear(light.color);
        // 쉐도우 팩터를 계산해줍니다.
        acc_color += light.color.rgb * radiance * shadow * brdf_factor;
    }

    return acc_color;
}

float3 PBR_OnePointLight(in float3 V, in float3 N, in PointLight light, in float3 position,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    // Light vector (to light)
    float3 lightVec = light.position - position;
    float distance = length(lightVec);

    if (distance > light.range)
        return float3(0.f, 0.f, 0.f);

    const float3 L = normalize(lightVec);
    const float3 H = normalize(V + L);

    // products
    const float NdotL = max(dot(N, L), EPSILON);
    const float NdotV = max(dot(N, V), EPSILON);
    const float NdotH = max(dot(N, H), EPSILON);
    const float HdotV = max(dot(H, V), EPSILON);

    // Attenuation

    /// Unity 커스텀 SRP 블로그에서 가져온 Attenuation
    float distSqr = max(dot(lightVec, lightVec), 0.00001f);
    float reciRSqr = 1.f / max((light.range * light.range), 0.00001f);
    float rangeAttn = pow(saturate(1.0f - pow((distSqr * reciRSqr), 2.0f)), 2.0f);
    float Attn = rangeAttn / distSqr;

    //float distSqr = max(dot(lightVec, lightVec), 0.00001f);
    //float rSqr = max((light.range * light.range), 0.00001f);
    //float lightAttn = rcp(distSqr);
    //float fadeDistance = 0.8f * 0.8f * rSqr;        
    //half smoothFactor = half(saturate((rSqr - distSqr) / (rSqr - fadeDistance)));
    //float Attn = lightAttn * smoothFactor;

    float3 radiance = Attn * light.power;

    // Burley roughness bias
    const float roughness2 = max(roughness * roughness, 0.01f);

    // Blend base colors
    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
    const float3 c_spec = lerp(kDielectricSpec.rgb, albedo, metallic) * ambientOcclusion;

    // BRDF
    //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
    float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);

    // Point light
    light.color = SRGBToLinear(light.color);
    // 쉐도우 팩터를 계산해줍니다.
    return light.color.rgb * radiance * shadow * brdf_factor;
}

float3 PBR_SpotLight(
    in float3 V, in float3 N, in SpotLight lights[SPOT_LIGHT_COUNT], in uint lightCount, in float3 position,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    SpotLight light;

    const float rad2Deg = (PI * 2.0f) / 360.f;

    [unroll]
    for (uint i = 0; i < lightCount; i++)
    {
        light = lights[i];

        float3 lightVec = light.position - position;
        float distance = length(lightVec);

        if (distance > light.range)
            continue;

        const float3 L = normalize(lightVec);
        const float3 H = normalize(V + L);

        // products
        const float NdotL = max(dot(N, L), EPSILON);
        const float NdotV = max(dot(N, V), EPSILON);
        const float NdotH = max(dot(N, H), EPSILON);
        const float HdotV = max(dot(H, V), EPSILON);

        /// 원본 Cone 및 거리 Attenuation
        // Cone attenuation
        /*float attStart = light.range;
        float attRange = cos(degrees(0.5f * light.angle) * attStart) - cos(degrees(0.5f * light.angle));
        attStart = cos(degrees(0.5f * light.angle));
        float cosAng = dot(-light.direction, L);
        float conAtt = saturate((cosAng - attStart) / attRange);
        conAtt *= conAtt;*/

        // Attenuation
        /*float DistToLightNorm = 1.0 - saturate(distance * (1.0f / light.range));
        float Attn = DistToLightNorm * DistToLightNorm;*/

        /// Unity 커스텀 SRP 블로그에서 가져온 Attenuation
        // 기본적으로 거리 Attenuation은 Point와 같다.
			// 거기에 방향성 및 각도 Attenuation만 연산해준 것..
        float distSqr = max(dot(lightVec, lightVec), 0.00001f);
        float reciRSqr = 1.f / max((light.range * light.range), 0.00001f);
        float rangeAttn = pow(saturate(1.0f - pow((distSqr * reciRSqr), 2.0f)), 2.0f);

        // Angle 관련 계산
			// 내각(Unity Light의 innerSpotAngle)
        float innerCos = cos(rad2Deg * 0.5f * light.innerAngle);
			// 실제 spotAngle(외각)
        float outerCos = cos(rad2Deg * 0.5f * light.outerAngle);
        float angleRangeInv = 1.f / max(innerCos - outerCos, 0.0001f);
        float angleRangeInv2 = -outerCos * angleRangeInv;

        // 0도에서 90도 까지의.. Spot Attenuation (위의 코드와 다를바가 거의 없다)
        float spotAttn = pow(saturate(dot(-light.direction.xyz, L) * angleRangeInv + angleRangeInv2), 2.0f);
        // 위의 것을 수정해볼까?

        float Attn = spotAttn * rangeAttn / distSqr;

        float3 radiance = Attn * light.power;

        // Burley roughness bias
        const float roughness2 = max(roughness * roughness, 0.01f);

        // Blend base colors
        const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
        const float3 c_spec = lerp(kDielectricSpec.rgb, albedo, metallic) * ambientOcclusion;

        // BRDF
        //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
        float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);


        // unity 연산을 고려한 감마 코렉션
        light.color = SRGBToLinear(light.color);
        acc_color += light.color.rgb * radiance * brdf_factor;
    }

    return acc_color;
}

float3 PBR_OneSpotLight(in float3 V, in float3 N, in SpotLight light, in float3 position,
	in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    const float rad2Deg = (PI * 2.0f) / 360.f;

    float3 lightVec = light.position - position;
    float distance = length(lightVec);

    if (distance > light.range)
        return float3(0.f, 0.f, 0.f);

    const float3 L = normalize(lightVec);
    const float3 H = normalize(V + L);

    // products
    const float NdotL = max(dot(N, L), EPSILON);
    const float NdotV = max(dot(N, V), EPSILON);
    const float NdotH = max(dot(N, H), EPSILON);
    const float HdotV = max(dot(H, V), EPSILON);

    /// Unity 커스텀 SRP 블로그에서 가져온 Attenuation
    // 기본적으로 거리 Attenuation은 Point와 같다.
		// 거기에 방향성 및 각도 Attenuation만 연산해준 것..
    float distSqr = max(dot(lightVec, lightVec), 0.00001f);
    float reciRSqr = 1.f / max((light.range * light.range), 0.00001f);
    float rangeAttn = pow(saturate(1.0f - pow((distSqr * reciRSqr), 2.0f)), 2.0f);

        // Angle 관련 계산
			// 내각(Unity Light의 innerSpotAngle)
    float innerCos = cos(rad2Deg * 0.5f * light.innerAngle);
			// 실제 spotAngle(외각)
    float outerCos = cos(rad2Deg * 0.5f * light.outerAngle);
    float angleRangeInv = 1.f / max(innerCos - outerCos, 0.0001f);
    float angleRangeInv2 = -outerCos * angleRangeInv;

    // 0도에서 90도 까지의.. Spot Attenuation (위의 코드와 다를바가 거의 없다)
    float spotAttn = pow(saturate(dot(-light.direction.xyz, L) * angleRangeInv + angleRangeInv2), 2.0f);

    float Attn = spotAttn * rangeAttn / distSqr;

    float3 radiance = Attn * light.power;

        // Burley roughness bias
    const float roughness2 = max(roughness * roughness, 0.01f);

        // Blend base colors
    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
    const float3 c_spec = lerp(kDielectricSpec.rgb, albedo, metallic) * ambientOcclusion;

        // BRDF
    //float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);
    float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotL, HdotV);

        // unity 연산을 고려한 감마 코렉션
    light.color = SRGBToLinear(light.color);
    return light.color.rgb * radiance * shadow * brdf_factor;
}

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    // Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    // Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    //float3 T = tangentW;
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    // Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}

// Shadow Factor 계산
float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH, float4 textureInfo)
{
	// NDC 기준 깊이값
    float depth = shadowPosH.z;

    // 텍셀의 크기 => 상수버퍼에서 받아온다.
    const float dx = textureInfo.z;
    const float dy = textureInfo.w;

    // 필터
    float shadowFactor = 0.f;

    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dy), float2(0.0f, dy), float2(dx, dy)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        shadowFactor += shadowMap.SampleCmpLevelZero(samShadow, shadowPosH.xy + offsets[i], depth).r;
    }

    // 결과들의 평균 반환
    return shadowFactor /= 9.0f;
}

float CalcShadowFactor(SamplerComparisonState samShadow, Texture2DArray shadowMap, float4 shadowPosH, float4 textureInfo, int idx)
{
	// NDC 기준 깊이값
    float depth = shadowPosH.z;

    // 텍셀의 크기 => 상수버퍼에서 받아온다.
    const float dx = textureInfo.z;
    const float dy = textureInfo.w;

    // 필터
    float shadowFactor = 0.f;

    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dy), float2(0.0f, dy), float2(dx, dy)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        shadowFactor += shadowMap.SampleCmpLevelZero(samShadow, float3(shadowPosH.xy + offsets[i], idx), depth).r;
    }

    // 결과들의 평균 반환
    return shadowFactor /= 9.0f;
}

float CalcDepthInShadow(const in float3 fragPos, float range)
{
    const float c1 = range / (range - 1.0f);
    const float c0 = -1.0f * range / (range - 1.0f);
    const float3 m = abs(fragPos).xyz;

    // 어느 축이 가장 진한가..?
    const float major = max(m.x, max(m.y, m.z));

    return (c1 * major + c0) / major;

}

// Point Light Shadow 계산
float CalcPointShadowFactor(SamplerComparisonState samShadow, int index, TextureCubeArray cubeShadowMap, float4 shadowPosH, float4 textureInfo, float range)
{
    // 깊이값
    float depth = CalcDepthInShadow(shadowPosH.xyz, range);

    shadowPosH = normalize(shadowPosH);

    // 텍셀의 크기 => 상수버퍼에서 받아온다.
    const float dx = textureInfo.z;
    const float dy = textureInfo.w;

     // 필터
    float shadowFactor = 0.f;

    /*const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dy), float2(0.0f, dy), float2(dx, dy)
    };*/

    shadowPosH.w = index;

    /*[unroll]
    for (int i = 0; i < 9; i++)
    {
        shadowFactor += cubeShadowMap.SampleCmpLevelZero(samShadow, shadowPosH, depth).r;
    }*/

    shadowFactor = cubeShadowMap.SampleCmpLevelZero(samShadow, shadowPosH, depth).r;
    
    return shadowFactor;
}

float3 SRGBToLinear(float3 color)
{
    float3 linearRGBLo = color / 12.92f;
    float3 linearRGBHi = pow((color + 0.055f) / 1.055f, 2.4f);
    float3 linearRGB = (color <= 0.04045f) ? linearRGBLo : linearRGBHi;
    return linearRGB;
}

float4 SRGBToLinear(float4 color)
{
    return float4(SRGBToLinear(color.rgb), color.a);
}