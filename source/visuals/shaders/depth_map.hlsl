
cbuffer PerFrame : register(b12)
{
    float4x4 view_projection_matrix;
}

cbuffer PerObject : register(b1)
{
    float4x4 world_matrix;
}

struct VS_INPUT
{
    float3 position : POSITION;
};

float4 vs_main(VS_INPUT vertex) : SV_Position
{ 
    return mul(mul(float4(vertex.position, 1.0), world_matrix), view_projection_matrix);
}
