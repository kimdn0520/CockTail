// PosColor 에서 필요한 기본적인 타입을 나타냅니다.
// 일단은 버텍스 셰이더 이후 픽셀 셰이더가 알아야하는 구조체를 지정합니다.
// == vout을 만듭니다.

struct VS_Output
{
    float4 Pos : SV_POSITION;
};