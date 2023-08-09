
static const float4 ColorBlack = { 0.0f, 0.0f, 0.0f , 1.0f };
static const float4 ColorWhite = { 1.0f, 1.0f, 1.0f , 1.0f };
static const float4 ShadowCoefficient = { 0.3f, 0.3f, 0.3f, 1.0f };

static const float DepthBias = 0.0005f;

SamplerState BilinearSampler : register(s14);
Texture2DArray inputTexture : register(t126);

Texture2D depthMap : register(t03);
SamplerState DepthSampler : register(s11); //Point

SamplerComparisonState cmpSampler : register(s10);

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

cbuffer LightData : register(b4)
{
    float4 light_direction;
    float4 light_color;
    float4 ambient_color;
    float4 UNUSED;
};

struct VS_DEPTH_INPUT
{
    float3 position : POSITION;
};

struct VS_FULL_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORDS;
    float3 normals : NORMALS;
    float atlas_slice : ATLAS_SLICE;
};

struct VS_SHADOWMAP_OUTPUT
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORDS;
    float3 normal : NORMALS;
    float4 proj_coord : TEXCOORD1;
};

struct VS_DIRECTIONAL_OUTPUT
{
    float4 position : SV_Position;
    float3 uvw : TEXCOORDS;
    float3 normal : NORMALS;
};

float4 vs_shadowmap_first_pass(VS_DEPTH_INPUT vertex) : SV_Position
{
    float4 _pos = float4(vertex.position, 1.0);
    _pos = mul(_pos, world_matrix);
    _pos = mul(_pos, projector_matrix);
    
    return _pos;
}

float4 ps_shadowmap_first_pass(float4 vertex : SV_Position) : SV_Target
{
    //vertex.z /= vertex.w;
    return float4(vertex.zzz, 1.0f);
}

VS_SHADOWMAP_OUTPUT vs_shadowmap_second_pass(VS_FULL_INPUT vertex)
{
    VS_SHADOWMAP_OUTPUT output;
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

//TODO: pass shadowmap size via variables
float2 texOffset(int u, int v)
{
    return float2(u * 1.0f / 8192, v * 1.0f / 6144);
}

float4 ps_shadowmap_second_pass(VS_SHADOWMAP_OUTPUT input) : SV_TARGET
{
    float4 texture_color = inputTexture.Sample(BilinearSampler, input.uvw);    
    float4 ambient = texture_color * ambient_color;
    
    float n_l = dot(input.normal, light_direction.xyz);
    
    if (n_l < 0)
        return ambient;
        
    input.proj_coord.xyz /= input.proj_coord.w;
    
    //if (input.proj_coord.x < -1.0f || input.proj_coord.x > 1.0f ||
    //    input.proj_coord.y < -1.0f || input.proj_coord.y > 1.0f ||
    //    input.proj_coord.z < 0.0f || input.proj_coord.z > 1.0f) return ambient;
    
    input.proj_coord.x = input.proj_coord.x / 2 + 0.5f;
    input.proj_coord.y = input.proj_coord.y / -2 + 0.5f;
    
    input.proj_coord.z -= DepthBias;
    
    //float shadowFactor = depthMap.SampleCmpLevelZero(cmpSampler, input.proj_coord.xy, input.proj_coord.z);
    
    //PCF sampling for shadow map
    float sum = 0;
    float x, y;
    
    for (y = -1.5; y <= 1.5; y += 1.0)
    {
        for (x = -1.5; x <= 1.5; x += 1.0)
        {
            sum += depthMap.SampleCmpLevelZero(cmpSampler, input.proj_coord.xy + texOffset(x, y), input.proj_coord.z);
        }
    }
    
    //float _sampled_depth = depthMap.Sample(DepthSampler, input.proj_coord.xy) + DepthBias;
 
    //if (_sampled_depth < input.proj_coord.z)
    //{
    //    return ambient;
    //}
    
    float shadowFactor = sum / 16.0;
    

    float4 diffuse = n_l > 0 ? texture_color * n_l * light_color : 0;
    
    return ambient + shadowFactor * texture_color * n_l;
    
    //return saturate(ambient + diffuse);
}

VS_DIRECTIONAL_OUTPUT vs_simple_directional_lighting(VS_FULL_INPUT vertex)
{
    VS_DIRECTIONAL_OUTPUT output;
    float4 _pos = float4(vertex.position, 1.0);
    output.position = mul(_pos, world_matrix);
    output.position = mul(output.position, view_projection_matrix);
    output.normal = mul(vertex.normals, world_matrix);
    output.normal = normalize(output.normal);
    output.uvw = float3(vertex.uv, vertex.atlas_slice);
    
    return output;
}

float4 ps_simple_directional_lighting(VS_DIRECTIONAL_OUTPUT input) : SV_TARGET
{
    float4 texture_color = inputTexture.Sample(BilinearSampler, input.uvw);
    float4 ambient = texture_color * ambient_color;  
    float n_l = dot(input.normal, light_direction.xyz);
    float4 diffuse =  texture_color * n_l * light_color;
    
    return n_l > 0 ? saturate(diffuse + ambient) : ambient;
}