// Shadow Map을 위한 Vertex Shader 입니다.
	// 기본적으로 들어온 버텍스를 그림자 맵으로 만들어준다. => 광원 시점으로 렌더링.

// 버텍스 쉐이더에 들어오는 값
	// PBR Object에 대해서만 그림자를 만들어 줄 것이다.
	// 기본적으로 line, Effect, SkyBox를 제외한 대부분의 물체가 PBR shader를 통해 렌더링 된다.

// 기본적인 상수 버퍼
cbuffer cbPerObject : register(b0)
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4x4 WorldInvTranspose;
}

// 그림자 맵 생성에 필요한 Light의 View Proj   
cbuffer cbLightViewProj : register(b1)
{
    float4x4 LightViewProj;
}

// Bone Matrix
cbuffer cbBoneMatrix : register(b2)
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

// 나갈 때는 포지션 값만 나가면 된다.
struct VS_Output
{
    float4 PosH : SV_POSITION;
};

// 버텍스 쉐이더
VS_Output main(VS_Input vin)
{
    VS_Output vout;

#ifdef Skinned
	// Skin Vertex에서는 Bone TM에 따라 이동된 버텍스의 위치를 뽑아내 준다.
    float3 pos = float3(0.f, 0.f, 0.f);

    float totalWeight = (float)0.f;

    for (int k = 0; k < 4; k++)
    {
        totalWeight += vin.Weight1[k];

        totalWeight += vin.Weight2[k];
    }

    // 정점 혼합
    for (int i = 0; i < 4; i++)
    {
        if(vin.BoneIndex1[i] != -1)
            pos += (vin.Weight1[i] / totalWeight) * mul(float4(vin.PosL, 1.0f), BoneMatrix[vin.BoneIndex1[i]]).xyz;
    }

    for (int j = 0; j < 4; j++)
    {
        if(vin.BoneIndex2[j] != -1)
			pos += (vin.Weight2[j] / totalWeight) * mul(float4(vin.PosL, 1.0f), BoneMatrix[vin.BoneIndex2[j]]).xyz;
    }

   
	vout.PosH = mul(float4(pos, 1.0f), World);
#else

  		// 우선 월드 좌표계로 포지션을 이동 후
    vout.PosH = mul(float4(vin.PosL, 1.0f), World);
#endif

    // 빛의 종류에 따라 매크로를 분리해준다.
		// DirLight 일 때는 LightViewProj를 곱해주고
    	// 아닐 때는 따로 곱하지 않는다 => 기하 쉐이더에서 각 면에 곱해서 shadow Map을 만들어준다.
#ifdef DirLightShadow
		// Light의 viewProj로 정점을 이동시킨다.
    vout.PosH = mul(vout.PosH, LightViewProj);
#endif

    return vout;
}