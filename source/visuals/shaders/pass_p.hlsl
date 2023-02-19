
static const float3 GrayScaleIntensity = { 0.299f, 0.587f, 0.114f };

Texture2D inputTexture : register(t127);

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TextureCoordinates : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = inputTexture.Sample(TrilinearSampler, input.TextureCoordinates);
    float intensity = dot(color.rgb, GrayScaleIntensity);

    return float4(intensity.rrr, color.a);

}