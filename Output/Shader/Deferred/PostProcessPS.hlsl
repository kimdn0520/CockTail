// Deferred 이후 들어온 각각의 Pxiel들에 대한 PostProcessing을 수행하는 Pixel Shader 입니다.

// 픽셀 쉐이더에 들어오는 값 => QuadVS의 OutPut값
	// 실제로 사용하는 값은 Texture 값만 사용해 줍니다..
struct PostPixelIn
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

// light 연산을 모두 끝낸 RenderTarget의 Texture
	// 해당 텍스쳐에서 샘플한 뒤, 후처리를 해줍니다.
Texture2D EndRenderTarget : register(t0);

SamplerState Sampler : register(s0);

// Unity의 Neutral Tonemapping 함수를 가져왔습니다.
	// Hable, Heji, Frostbite의 공식을 사용하는 것 같습니다.
// Input 값은 Linear RGB여야 하므로, light Pass의 마지막 감마 인코딩을 제거해줍니다.
// Unity의 real을 float로 취급해주었습니다.
float3 NeutralCurve(float3 x, float a, float b, float c, float d, float e, float f)
{
    return ((x * (a * x + c * b) + d * e) / (x * (a * x + b) + d * f)) - e / f;
}

float3 NeutralTonemap(float3 pixelColor)
{
	// ToneMap 수치들
    const float a = 0.2f;
    const float b = 0.29f;
    const float c = 0.24f;
    const float d = 0.272f;
    const float e = 0.02f;
    const float f = 0.3f;
    const float whiteLevel = 5.3f;
    const float whiteClip = 1.0f;

    float3 whiteScale = (1.0f).xxx / NeutralCurve(whiteLevel, a, b, c, d, e, f);
    pixelColor = NeutralCurve(pixelColor * whiteScale, a, b, c, d, e, f);
    pixelColor *= whiteScale;

    pixelColor /= whiteClip.xxx;

    return pixelColor;
}

// Unity의 Vignetting 함수.
float3 Vignette(float3 cin, float2 uv, float intensity, float smoothness)
{
	// center 0.5, color black 고정입니다..
    float2 center = float2(0.5f, 0.5f);
    // 검은색..(외곽의 색깔.)
    float3 color = float3(0.f, 0.f, 0.f);
    float2 dist = abs(uv - center) * intensity;

    float vfactor = pow(saturate(1.0f - dot(dist, dist)), smoothness);

    return cin * lerp(color, (1.0f).xxx, vfactor);
}


// Linear Space 값을 Gamma Space로
// hlsli에 있는 함수를 찾지 못해서 여기로..
float3 LinearToSRGB(float3 color)
{
    float3 sRGBLo = color * 12.92f;
    float3 sRGBHi = (pow(color, 1 / 2.4f) * 1.055f) - 0.055f;
    float3 sRGB = (color <= 0.0031308f) ? sRGBLo : sRGBHi;

    return sRGB;
}

float4 LinearToSRGB(float4 color)
{
    return float4(LinearToSRGB(color.rgb), color.a);
}


// 버텍스 쉐이더
float4 main(PostPixelIn ppin) : SV_Target
{
    // 후처리를 받기 전의 색상
    float3 retColor = EndRenderTarget.Sample(Sampler, ppin.Tex).xyz;


    // ToneMapping을 해줍니다.
#ifdef UseToneMapping
    retColor = NeutralTonemap(retColor);
    saturate(retColor);
#endif

#ifdef UseVignette
    // 현재는 기본값을 세팅해줍니다. 필요하면 cBuffer로 전달해줍니다.
    retColor = Vignette(retColor, ppin.Tex, 0.25f, 0.4f);
#endif

    retColor = LinearToSRGB(retColor);

	return float4(retColor, 1.0f);
}