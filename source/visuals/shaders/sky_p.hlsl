
TextureCube SkyTexture : register(t122);
SamplerState BilinearSampler : register(s14);

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORDS;
};


float4 main(VS_OUTPUT input) : SV_TARGET
{
    return SkyTexture.Sample(BilinearSampler, input.uvw);
}