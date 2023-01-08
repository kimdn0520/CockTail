// SkyBox를 그리기 위한 픽셀 셰이더 코드 입니다.

#include "SpriteEffectTypes.hlsli"

// 큐브맵
Texture2D EffectMap : register( t0 );

// 샘플러
SamplerState Sampler : register( s0 );

// RasterizerState와 DepthStencilState의 경우 밖에서 지정해준다..

// 픽셀 쉐이더
float4 main(VS_Output pin) : SV_Target
{
    // 텍스쳐를 샘플해서 바로 내보내면 된다.
    return EffectMap.Sample(Sampler, pin.Tex);
}