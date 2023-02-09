
cbuffer PerFrame : register(b0)
{
    float4x4 view_projection_matrix;
}

cbuffer PerObject : register(b1)
{
    float4x4 world_matrix;
}

struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};


VS_OUTPUT main(VS_INPUT vertex)
{
    VS_OUTPUT output;
    
    output.position = mul(vertex.position, world_matrix);
    output.position = mul(output.position, view_projection_matrix);
    output.color = vertex.color;
    
    return output;
}