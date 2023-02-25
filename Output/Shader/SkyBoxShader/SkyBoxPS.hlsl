// SkyBox를 그리기 위한 픽셀 셰이더 코드 입니다.

#include "SkyBoxTypes.hlsli"

// 큐브맵
TextureCube CubeMap : register( t0 );

// 샘플러
SamplerState samTriLinearSam : register( s0 );

// 픽셀 쉐이더
float4 main(VS_Output pin) : SV_Target
{
    float4 color = CubeMap.Sample(samTriLinearSam, pin.PosL);

    // Gamma Correction
    color = pow(color, 2.2f);

    return color;
}