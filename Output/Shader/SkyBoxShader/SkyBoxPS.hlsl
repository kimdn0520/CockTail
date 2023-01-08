// SkyBox를 그리기 위한 픽셀 셰이더 코드 입니다.

#include "SkyBoxTypes.hlsli"

// 큐브맵
TextureCube CubeMap : register( t0 );

// 샘플러
SamplerState samTriLinearSam : register( s0 );

// RasterizerState와 DepthStencilState의 경우 밖에서 지정해준다..

// 픽셀 쉐이더
float4 main(VS_Output pin) : SV_Target
{
    return CubeMap.Sample(samTriLinearSam, pin.PosL);
}