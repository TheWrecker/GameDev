
cbuffer HudData : register(b01)
{
    float4x4 translation_matrix;
}

SamplerState PointSampler : register(s13);

Texture2D inputTexture : register(t02);

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
    output.position = mul(pos, translation_matrix);
    output.uv = input.uv;

    return output;
}

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float4 color = inputTexture.Sample(PointSampler, input.uv);
    return float4(color.r, color.g, color.b, 1.0f);
}
