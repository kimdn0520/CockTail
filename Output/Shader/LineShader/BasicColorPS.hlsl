// 기본적인 위치와 색만을 가지고 있는 PosColor의 pixel Shader 입니다.
	// LineShader로 이름을 변경했습니다.

// 이후에 필요하다면 추가하겠지만 이 쉐이더 코드는 기본적으로 모델이 있음을 가정합니다
// 즉 상수 버퍼를 받아야함.
#include "BasicColorTypes.hlsli"

// 픽셀 쉐이더
float4 main(VS_Output pin) : SV_Target
{
    return pin.Color;
}