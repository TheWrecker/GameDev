
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
    float3 position : POSITION;
    float2 uv : TEXCOORDS;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT vertex)
{
    VS_OUTPUT output;
    
    float4 pos = float4(vertex.position, 1.0);
    output.position = mul(pos, translation_matrix);
    output.position = mul(output.position, view_projection_matrix);
    output.uvw = vertex.position.rgb;
    
    return output;
}
