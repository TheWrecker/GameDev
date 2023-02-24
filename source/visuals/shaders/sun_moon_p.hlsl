
Texture2D SunTexture;
SamplerState BilinearSampler : register(s14);

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};


float4 main(VS_OUTPUT input) : SV_TARGET
{
    return SunTexture.Sample(BilinearSampler, input.uv);
}