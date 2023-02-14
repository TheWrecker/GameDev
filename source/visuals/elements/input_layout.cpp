
#include "util_funcs.h"
#include "shader_vertex.h"
#include "../presenter.h"

#include "input_layout.h"

InputLayout::InputLayout(Presenter* presenter, VertexShader* shader)
    :input_layout(), shader(shader)
{
    device = presenter->GetDevice();
    context = presenter->GetContext();
}

InputLayout::~InputLayout()
{
    DXRelease(input_layout);
}

InputLayout& InputLayout::AddElement(const std::string& semantic, DXGI_FORMAT format)
{
    if (container.size() == 0)
    {
        container.push_back({ semantic.c_str(), 0, format, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }
    else
    {
        container.push_back({ semantic.c_str(), 0, format, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }
    return *this;
}

InputLayout& InputLayout::AddElement(D3D11_INPUT_ELEMENT_DESC& target)
{
    container.push_back(target);
    return *this;
}

bool InputLayout::Build()
{
    auto _result = device->CreateInputLayout(&container.at(0), static_cast<unsigned int>(container.size()), shader->GetByteCode(),
        shader->GetSize(), &input_layout);
    if (_result < 0)
        return false;
    return true;
}

void InputLayout::Bind()
{
    context->IASetInputLayout(input_layout);
}

void InputLayout::Unbind()
{
    context->IASetInputLayout(nullptr);
}

void InputLayout::Clear()
{
    container.clear();
}
