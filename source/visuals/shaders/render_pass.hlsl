
static const float3 GrayScaleIntensity = { 0.299f, 0.587f, 0.114f };

SamplerState BilinearSampler : register(s14);
Texture2D inputTexture : register(t120);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORDS;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORDS;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.position, 1.0);
    output.position = pos;
    output.uv = input.uv;

    return output;
}

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float4 color = inputTexture.Sample(BilinearSampler, input.uv);
    float intensity = dot(color.rgb, GrayScaleIntensity);

    return float4(intensity.rrr, color.a);
}
