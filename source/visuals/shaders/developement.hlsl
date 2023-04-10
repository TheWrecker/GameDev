
SamplerState BilinearSampler : register(s14);
SamplerState ProjectionSampler : register(s12);

Texture2D projectionTexture : register(t118); //logo
Texture2D inputTexture : register(t123);

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

struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORDS;
    float3 normals : NORMALS;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORDS;
    float3 normals : NORMALS;
    float3 world_pos : WORLDPOS;
    float4 proj_coord : PROJCOORD;
    float attenuation : ATTENUATION;
};

VS_OUTPUT vs_main(VS_INPUT vertex)
{
    VS_OUTPUT output;
    float4 _pos = float4(vertex.position, 1.0);
    _pos = mul(_pos, world_matrix);
    output.world_pos = _pos.xyz;
    output.position = mul(_pos, view_projection_matrix);
    output.uv = vertex.uv;
    output.normals = normalize(mul(float4(vertex.normals, 0), world_matrix).xyz);
    
    float3 _light_direction = light_position - output.world_pos;
    
    output.attenuation = saturate(1.0f - (length(_light_direction) / light_radius));
    output.proj_coord = mul(_pos, projector_matrix);
    
    return output;
}

float4 ps_main(VS_OUTPUT input) : SV_TARGET
{
    float3 _light_direction = normalize(light_position - input.world_pos);
    float3 _view_direction = normalize(camera_position - input.world_pos);
    float3 _half_vector = normalize(_view_direction + _light_direction);
    float _n_dot_l = dot(input.normals, _light_direction);
    float _n_dot_h = dot(input.normals, _half_vector);

    float4 _color = inputTexture.Sample(BilinearSampler, input.uv);

    float3 _ambient = _color.rgb * ambient_color.rgb * ambient_color.a;
    float3 _diffuse = _color.rgb * saturate(_n_dot_l) * light_color.rgb * input.attenuation * light_color.a;
    float3 _specular = specular_color.rgb * saturate(_n_dot_h) * input.attenuation * specular_color.a * specular_power;

    float4 _out = float4(saturate(_ambient + _diffuse + _specular), _color.a);
    
    if (input.proj_coord.w > 0.0f)
    {
        input.proj_coord.xy /= input.proj_coord.w;
        float3 _proj_color = projectionTexture.Sample(ProjectionSampler, input.proj_coord.xy).rgb;
        
        _out.rgb *= _proj_color;
    }
    
    return _out;
}
