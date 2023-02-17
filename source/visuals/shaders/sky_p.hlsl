
Texture2D SkyboxTexture;
SamplerState BilinearSampler;

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TextureCoordinates : TEXCOORD;
};


float4 main(VS_OUTPUT IN) : SV_TARGET
{
    return SkyboxTexture.Sample(BilinearSampler, IN.TextureCoordinates.rg);
}