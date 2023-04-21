
cbuffer HudData : register(b10)
{
    float4x4 hud_matrix;
}

cbuffer DevData : register(b01)
{
    float4x4 dev_matrix;
}

SamplerState BilinearSampler : register(s14);

Texture2D inputTexture : register(t121);

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
    output.position = mul(pos, hud_matrix);
    output.uv = input.uv;

    return output;
}


float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float4 color = inputTexture.Sample(BilinearSampler, input.uv);
    return color;
}