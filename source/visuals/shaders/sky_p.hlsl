
TextureCube SkyTexture;
SamplerState BilinearSampler;

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORD;
};


float4 main(VS_OUTPUT input) : SV_TARGET
{
    return SkyTexture.Sample(BilinearSampler, input.uvw);
}