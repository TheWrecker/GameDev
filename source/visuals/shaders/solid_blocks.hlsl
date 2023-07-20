
static const float4 ColorBlack = { 0.0f, 0.0f, 0.0f , 1.0f };
static const float4 ColorWhite = { 1.0f, 1.0f, 1.0f , 1.0f };
static const float4 ShadowCoefficient = { 0.3f, 0.3f, 0.3f, 1.0f };

static const float DepthBias = 0.001f;

SamplerState BilinearSampler : register(s14);
Texture2DArray inputTexture : register(t126);

Texture2D depthMap : register(t03);
SamplerState DepthSampler : register(s11); //Point

cbuffer PerFrame : register(b12)
{
    float4x4 view_projection_matrix;
}

cbuffer PerObject : register(b1)
{
    float4x4 world_matrix;
}

cbuffer SunDirection : register(b2)
{
    float4x4 projector_matrix;
}

cbuffer BiasMatrix : register(b3)
{
    float4x4 bias_matrix;
}

cbuffer LightData : register(b11)
{
    float4 light_direction;
    float4 light_color;
    float4 ambient_color;
    float4 UNUSED;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORDS;
    float3 normals : NORMALS;
    float atlas_slice : ATLAS_SLICE;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORDS;
    float3 normal : NORMALS;
    float4 proj_coord : TEXCOORD1;
};

float4 vs_first_pass(VS_INPUT vertex) : SV_Position
{
    float4 _pos = float4(vertex.position, 1.0);
    _pos = mul(_pos, world_matrix);
    _pos = mul(_pos, projector_matrix);
    
    return _pos;
}

float4 ps_first_pass(float4 vertex : SV_Position) : SV_Target
{
    //vertex.z /= vertex.w;
    return float4(vertex.zzz, 1.0f);
}

VS_OUTPUT vs_second_pass(VS_INPUT vertex)
{
    VS_OUTPUT output;
    float4 _pos = float4(vertex.position, 1.0);
    output.position = mul(_pos, world_matrix);
    output.proj_coord = mul(output.position, projector_matrix);
    //output.proj_coord = mul(output.proj_coord, bias_matrix);
    output.position = mul(output.position, view_projection_matrix);
    output.normal = mul(vertex.normals, world_matrix);
    output.normal = normalize(output.normal);
    output.uvw = float3(vertex.uv, vertex.atlas_slice);
    
    return output;
}

float4 ps_second_pass(VS_OUTPUT input) : SV_TARGET
{
    float4 texture_color = inputTexture.Sample(BilinearSampler, input.uvw);
    
    //float4 ambient = texture_color * ambient_color * ambient_color.a;
    //float n_l = dot(input.normal, light_direction.rgb);
    //float4 diffuse = n_l > 0 ? texture_color * n_l * light_color * light_color.a : (float4) 0;
    
    //float4 shadow_color = ColorWhite;
    
    input.proj_coord.xyz /= input.proj_coord.w;
    
    input.proj_coord.x = input.proj_coord.x/2 + 0.5f;
    input.proj_coord.y = input.proj_coord.y/-2 + 0.5f;
    
    float visibility = 1.0;
    float _sampled_depth = depthMap.Sample(DepthSampler, input.proj_coord.xy) + DepthBias;
 
    if (_sampled_depth < input.proj_coord.z)
    {
        visibility = 0.5;
    }
    
    //if (input.proj_coord.w > 0.0f)
    //{
    //    input.proj_coord.xyz /= input.proj_coord.w;
        
    //    float _pixel_Depth = input.proj_coord.z;
    //    //float _sample_Depth = depthMap.SampleCmpLevelZero(DepthSampler, input.proj_coord.xy, _pixel_Depth).x;
    //    float _sample_Depth = depthMap.Sample(DepthSampler, input.proj_coord.xy).x + DepthBias;
        
    //    shadow_color = (_pixel_Depth > _sample_Depth ? ShadowCoefficient : ColorWhite);
    //}
    
    //return saturate(ambient + diffuse) * shadow_color;
    
    return texture_color * visibility;
}