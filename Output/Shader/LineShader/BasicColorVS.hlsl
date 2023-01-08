// 기본적인 위치와 색만을 가지고 있는 PosColor의 vertex Shader 입니다.
	// 기본적으로 Line 모델을 그릴 때만 사용 예정입니다.

// 이후에 필요하다면 추가하겠지만 이 쉐이더 코드는 기본적으로 모델이 있음을 가정합니다
// 즉 상수 버퍼를 받아야함.
#include "BasicColorTypes.hlsli"

// 월드와 View를 나누는 의미가..
	// 월드는 오브젝트자체의 좌표
	// View나 proj은 카메라에 따라 변경될 수 있음..
// 일단 지금은 이렇게 해두고, 나중에 필요에 따라 변경하는 것으로 한다.

// 기본적인 상수 버퍼
cbuffer cbPerFrame : register(b0)
{
    float4x4 WorldViewProj;
};

// 버텍스 쉐이더에 들어오는 값
struct VS_Input
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

// 버텍스 쉐이더
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;
    vout.Pos = mul(float4(vin.PosL, 1.0f), WorldViewProj);
    vout.Color = vin.Color;

    return vout;
}