// SkyBox을 화면에 그리기 위한 쉐이더 코드입니다.

#include "SpriteEffectTypes.hlsli"

// 기본적인 상수 버퍼
cbuffer cbPerObject : register(b0)
{
    float4x4 WorldViewProj;
};

cbuffer cbUVOffset : register(b1)
{
    float2 UVOffset;
    float2 pad;
};

// 버텍스 쉐이더에 들어오는 값
struct VS_Input
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

// 버텍스 쉐이더
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output) 0;

    // 기본적인 포지션 변경을 해준다.
    vout.Pos = mul(float4(vin.PosL, 1.0f), WorldViewProj);

    // Offset을 더해준 UV값을 반환해준다.
    vout.Tex.x = vin.Tex.x + UVOffset.x;
    vout.Tex.y = vin.Tex.y + UVOffset.y;

    return vout;
}