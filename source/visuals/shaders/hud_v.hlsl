
cbuffer HudData : register(b10)
{
    float4x4 hud_matrix;
}

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

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.position, 1.0);
    output.position = mul(pos, hud_matrix);
    output.uv = input.uv;

    return output;
}
