
static const float4 ColorBlack = { 0.0f, 0.0f, 0.0f , 1.0f };
static const float4 ColorWhite = { 1.0f, 1.0f, 1.0f , 1.0f };

static const float DepthBias = 0.005f;

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

cbuffer DevData : register(b2)
{
    float4
			ambient_color, //new vec
			light_color, //new vec
			specular_color; //new vec

    float3 light_position; //new vec
    float light_radius;
    
    float3 camera_position; //new vec
    float specular_power;

    float4x4 projector_matrix; //new vec x 4
}

cbuffer LightData : register(b11)
{
    float4 light_direction;
    float4 test_light_color;
    float4 test_ambient_color;
    float4 UNUSED;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORDS;
    float3 normals : NORMALS;
    float atlas_slice : ATLAS_SLICE;
};

struct VS_FIRST_OUTPUT
{
    float4 position : SV_Position;
};

struct VS_SECOND_OUTPUT
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORDS;
    float3 normal : NORMALS;
    float4 proj_coord : PROJCOORD;
};

VS_FIRST_OUTPUT vs_first_pass(VS_INPUT vertex)
{
    VS_FIRST_OUTPUT output;
    float4 _pos = float4(vertex.position, 1.0);
    output.position = mul(_pos, world_matrix);
    output.position = mul(output.position, view_projection_matrix);
    
    return output;
}

VS_OUTPUT vs_second_pass(VS_INPUT vertex)
{
    VS_OUTPUT output;
    float4 _pos = float4(vertex.position, 1.0);
    output.position = mul(_pos, world_matrix);
    output.position = mul(output.position, view_projection_matrix);
    output.normal = mul(vertex.normals, world_matrix);
    output.normal = normalize(output.normal);
    output.uvw = float3(vertex.uv, vertex.atlas_slice);
    output.proj_coord = mul(_pos, projector_matrix);
    
    return output;
}

float4 ps_main(VS_OUTPUT input) : SV_TARGET
{
    float4 texture_color = inputTexture.Sample(BilinearSampler, input.uvw);
    float4 ambient = texture_color * test_ambient_color * test_ambient_color.a;
    float n_l = dot(input.normal, light_direction.rgb);
    float4 diffuse = n_l > 0 ? texture_color * n_l * light_color * light_color.a : (float4) 0;
    
    float4 shadow_color = ColorWhite;
    
    if (input.proj_coord.w > 0.0f)
    {
        input.proj_coord.xyz /= input.proj_coord.w;
        
        float _pixel_Depth = input.proj_coord.z;
        //float _sample_Depth = depthMap.SampleCmpLevelZero(DepthSampler, input.proj_coord.xy, _pixel_Depth).x;
        float _sample_Depth = depthMap.Sample(DepthSampler, input.proj_coord.xy).x + DepthBias;
        
        shadow_color = (_pixel_Depth > _sample_Depth ? ColorBlack : ColorWhite);
    }
    
    return saturate(ambient + diffuse) * shadow_color;
}