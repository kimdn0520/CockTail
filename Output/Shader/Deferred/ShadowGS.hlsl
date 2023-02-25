// PointLight Shadow Map을 위한 Geometry Shader 입니다.
	// 기본적으로 들어온 버텍스를 그림자 맵으로 만들어준다. => 광원 시점으로 렌더링.
	// 포인트 라이트에서는 들어온 폴리곤을 그림자 큐브맵에 각각 그려준다
	// => 3개의 정점을 각 면(6개)에 렌더 => 18개의 정점으로

// 버텍스 쉐이더에 들어오는 값
	// PBR Object에 대해서만 그림자를 만들어 줄 것이다.
	// 기본적으로 line, Effect, SkyBox를 제외한 대부분의 물체가 PBR shader를 통해 렌더링 된다.

// 큐브맵의 각 면에 해당하는 shadowMatrix
cbuffer cbLightMatrix : register(b1)
{
    float4x4 LightViewProj[6];
}

// 기하 쉐이더의 Input / Output => 일단은 TexCoord를 제외하고 만들어본다.
struct GS_Input
{
	float4 PosH : SV_POSITION;
};

struct GS_Output
{
    float4 FragPos : FRAGPOS;
    float4 PosH : SV_POSITION;
    uint RTIndex : SV_RENDERTARGETARRAYINDEX;
};

// 기하 쉐이더 => 결과 값은 큐브맵으로 이루어진 그림자 맵이 된다.
[maxvertexcount(18)]
void main(
	triangle GS_Input gin[3],
	inout TriangleStream<GS_Output> gout
)
{
	// 모든 면에 대해서 그림자 깊이만을 넣어준다..
    for (int face = 0; face < 6; ++face)
    {
        GS_Output element;
        element.RTIndex = face;

        // 폴리곤을 구성하는 각 정점을 각 면으로 투영한다(PointLight 각 면의 ViewProj을 사용한다.)
        for (uint i = 0; i < 3; i++)
        {
            element.FragPos = gin[i].PosH;
            element.PosH = mul(gin[i].PosH, LightViewProj[face]);
            gout.Append(element);
        }
        gout.RestartStrip();
    }

}