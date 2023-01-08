// PBRModel 에서 필요한 기본적인 타입을 나타냅니다.
// 버텍스 셰이더 이후 픽셀 셰이더가 알아야하는 구조체를 지정합니다.
// == vout을 만듭니다.

struct VS_Output
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR0;

    float metallic : METALLIC;
    float roughness : ROUGHNESS;
};