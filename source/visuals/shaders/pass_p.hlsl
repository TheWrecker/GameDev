
static const float3 GrayScaleIntensity = { 0.299f, 0.587f, 0.114f };

SamplerState BilinearSampler : register(s14);
texture2D inputTexture : register(t120);


struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 TextureCoordinates : TEXCOORDS;
};

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = inputTexture.Sample(BilinearSampler, input.TextureCoordinates);
    float intensity = dot(color.rgb, GrayScaleIntensity);

    return float4(intensity.rrr, color.a);

}