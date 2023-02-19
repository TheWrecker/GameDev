
struct VS_INPUT
{
    float4 position : POSITION;
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
    
    output.position = input.position;
    output.uv = input.uv;

    return output;
}
