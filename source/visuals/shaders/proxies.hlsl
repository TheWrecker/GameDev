
cbuffer PerFrame : register(b12)
{
    float4x4 view_projection_matrix;
}

cbuffer PerObject : register(b01)
{
    float4x4 world_matrix;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VS_OUTPUT vs_main(VS_INPUT vertex)
{
    VS_OUTPUT output;
    float4 pos = float4(vertex.position, 1.0);
    output.position = mul(pos, world_matrix);
    output.position = mul(output.position, view_projection_matrix);
    output.color = vertex.color;
    
    return output;
}

float4 ps_main(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}