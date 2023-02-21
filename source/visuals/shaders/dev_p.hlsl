
SamplerState BilinearSampler : register(s14);
texture2D inputTexture;

struct PS_INPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORDS;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = inputTexture.Sample(BilinearSampler, input.uv);
    return color;
}