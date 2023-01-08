// SkyBox을 화면에 그리기 위한 쉐이더 코드입니다.
	

#include "SkyBoxTypes.hlsli"

// 기본적인 상수 버퍼
cbuffer cbPerFrame : register( b0 )
{
    float4x4 WorldViewProj;
};


// 버텍스 쉐이더에 들어오는 값
struct VS_Input
{
    float3 PosL : POSITION;
};

// 버텍스 쉐이더
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;

    // z/w = 1이 되도록 (하늘 돔이 평면에 있도록) z = w로 설정한다.
    vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj).xyww;

    // 로컬 버텍스의 위치를 텍스쳐의 조회 벡터로 사용한다.
    vout.PosL = vin.PosL;

    return vout;
}