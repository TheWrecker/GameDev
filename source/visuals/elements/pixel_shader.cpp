
#include "util_funcs.h"

#include "pixel_shader.h"

PixelShader::PixelShader(ID3D11Device* device, ID3D11DeviceContext* context)
    :IShader(device, context), shader()
{
}

PixelShader::~PixelShader()
{
    DXRelease(shader);
}

bool PixelShader::LoadShader(ShaderType type, const std::wstring& file)
{
    if (IShader::LoadShader(type, file))
    {
        DXAssert(device->CreatePixelShader(byte_code, byte_code_size, 0, &shader));
        return true;
    }
    return false;
}

void PixelShader::Apply()
{
    assert(shader);
    context->PSSetShader(shader, 0, 0);
}
