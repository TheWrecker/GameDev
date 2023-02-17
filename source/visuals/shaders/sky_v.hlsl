
cbuffer PerFrame : register(b12)
{
    float4x4 view_projection_matrix;
}

cbuffer CBufferTranslation : register(b1)
{
    float4x4 translation_matrix : WORLDVIEWPROJECTION;
}

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORDS;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 uvw : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT vertex)
{
    VS_OUTPUT output;
    
    output.position = mul(vertex.position, translation_matrix);
    output.position = mul(output.position, view_projection_matrix);
    output.uvw = vertex.uv.rg;
    
    return output;
}
