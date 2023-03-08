
SamplerState BilinearSampler : register(s14);
Texture2DArray inputTexture : register(t126);

cbuffer LightData : register(b11)
{
    float4 light_direction;
    float4 light_color;
    float4 ambient_color;
    float4 UNUSED;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORDS;
    float3 normal : NORMALS;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 texture_color = inputTexture.Sample(BilinearSampler, input.uvw);
    float4 ambient = texture_color * ambient_color * ambient_color.a;
    float n_l = dot(input.normal, light_direction.rgb);
    float4 diffuse = n_l > 0 ? texture_color * n_l * light_color * light_color.a : (float4) 0;
    return saturate(ambient + diffuse);
}