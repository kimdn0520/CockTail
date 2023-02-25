// Deferred 이후 마지막 화면에 그림을 그려주기 위한 사각형  Vertex Shader 입니다.

// 버텍스 쉐이더에 들어오는 값
struct VS_Input
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct VS_Output
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

// 버텍스 쉐이더
VS_Output main(VS_Input vin)
{
    VS_Output vout;
    vout.PosH = float4(vin.PosL, 1.0f).xyww;
	vout.Tex = vin.Tex;

    return vout;
}