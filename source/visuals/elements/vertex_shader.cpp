
#include "util_funcs.h"

#include "vertex_shader.h"

VertexShader::VertexShader(Presenter* presenter)
    :IShader(presenter), shader()
{
}

VertexShader::~VertexShader()
{
    DXRelease(shader);
}

bool VertexShader::LoadShader(ShaderType type, const std::wstring& file)
{
    if (IShader::LoadShader(type, file))
    {
        DXAssert(device->CreateVertexShader(byte_code, byte_code_size, 0, &shader));
        return true;
    }
    return false;
}

void VertexShader::Apply()
{
    assert(shader);
    context->VSSetShader(shader, 0, 0);
}
