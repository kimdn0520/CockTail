// Ref.by https://github.com/CrustPizza/LevelUpEngine/blob/main/OutPut/Shader/PBRModel/PBRModelVS.hlsl
// Ref.by https://github.com/kimdn0520/GraphicsEngine_DX11/blob/main/Output/Data/Shader/Model_PBR_VS.hlsl

// PBR으로 빛 연산을 하는 PBR Model을 만들기 위한 Vertex Shader입니다.
	// 내부에 Skinning과 관련된 정점 구조역시 들어가 있습니다.
#include "PBRModelTypes.hlsli"

// 기본적인 상수 버퍼
cbuffer cbPerObject : register( b0 )
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4x4 WorldInvTranspose;
}

// PBR과 관련된 파라메터.
cbuffer cbMaterial : register(b1)
{
    float Metallic;
    float Roughness;
    float2 pad1;

    float3 AddColor;
    float pad2;

    float3 EmissiveColor;
    float pad3;
	
    bool IsLight;
}

// Bone Matrix
cbuffer cbBoneMatrix : register( b2 )
{
    matrix BoneMatrix[100];
}

// TODO : Shadow를 위한 LightViewProj 추가
#ifdef Skinned
struct VS_Input
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float3 TangentL : TANGENT;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;

	float4 Weight1 : WEIGHT;
	float4 Weight2 : WEIGHTS;

	uint4 BoneIndex1 : BONEINDEX;
	uint4 BoneIndex2 : BONEINDICES;
};

// 버텍스 쉐이더에 들어오는 값
// 위치, 노말, Tangent 값
#else
struct VS_Input
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};
#endif

// 버텍스 쉐이더
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;

#ifdef Skinned
    float3 pos = float3(0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);

    // Total Weight 계산
    float totalWeight = (float)0.f;

    for (int k = 0; k < 4; k++)
	{
		totalWeight += vin.Weight1[k];

		totalWeight += vin.Weight2[k];
	}

	// Vertex Blending
	for (int i = 0; i < 4; i++)
	{
		// 노말 변환시 변환 행렬에 비균등 비례가 없다고 가정한다.
        // 역전치 행렬이 아닌 변환행렬을 그대로 사용.
        
        // 포지션에는 웨이트 값만큼을 곱해준다. 여기서 트랜스폼 값이 없으면 (영향울 안받는다면?) 0이 더해질 것.
		if (vin.BoneIndex1[i] != -1)
		{
			pos += (vin.Weight1[i] / totalWeight) * mul(float4(vin.PosL, 1.0f), BoneMatrix[vin.BoneIndex1[i]]).xyz;
			normal += (vin.Weight1[i] / totalWeight) * mul(vin.NormalL, (float3x3)BoneMatrix[vin.BoneIndex1[i]]);
		}
	}

	for (int j = 0; j < 4; j++)
	{
		// 논유니폼 스케일이 없다고 가정하므로, 노말값을 위한 역행렬의 전치행렬이 필요없다.
		// 두 번째 본들의 blending => 한 정점에 영향을 미치는 본이 8개로 가정
		if (vin.BoneIndex2[j] != -1)
		{
			pos += (vin.Weight2[j] / totalWeight) * mul(float4(vin.PosL, 1.0f), BoneMatrix[vin.BoneIndex2[j]]).xyz;
			normal += (vin.Weight2[j] / totalWeight) * mul(vin.NormalL, (float3x3)BoneMatrix[vin.BoneIndex2[j]]);
		}
	}

	vout.PosW = mul(float4(pos, 1.0f), World).xyz;

	vout.PosH = mul(float4(pos, 1.0f), WorldViewProj);

	// 접선벡터와 법선벡터가 여전히 직교가되는 법선 벡터 변환행렬인 역전치행렬을 노말(법선벡터)에 곱해준다.
	vout.NormalW = normalize(mul(normal, (float3x3)WorldInvTranspose));
#else
    vout.PosW = mul(float4(vin.PosL, 1.0f), World).xyz;

    vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj);

    vout.NormalW = normalize(mul(vin.NormalL, (float3x3) WorldInvTranspose));
#endif
    vout.Tex = vin.Tex;

    vout.TangentW = mul(vin.TangentL, (float3x3) World);

    vout.Color = vin.Color;

    vout.metallic = Metallic;
    vout.roughness = Roughness;

    return vout;
}