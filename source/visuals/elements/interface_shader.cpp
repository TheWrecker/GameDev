
#include <cassert>
#include <d3dcompiler.h>

#include "util_funcs.h"
#include "../presenter.h"

#include "interface_shader.h"

IShader::IShader(Presenter* presenter)
	:blob(), byte_code(nullptr), byte_code_size(0), type(ShaderType::UNDEFINED)
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
}

IShader::~IShader()
{
	DXRelease(blob);
}

bool IShader::LoadShader(ShaderType type, const std::wstring& file)
{
	assert(device && context);
	assert(!blob);

	UINT shaderFlags = 0;
	ID3DBlob* errorMessages = nullptr;

#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (type == ShaderType::VERTEX)
	{
		DXAssert(D3DCompileFromFile(file.c_str(), nullptr, nullptr, "main", "vs_5_0", shaderFlags, 0, &blob, &errorMessages));
		byte_code = blob->GetBufferPointer();
		byte_code_size = blob->GetBufferSize();
	}

	else if (type == ShaderType::PIXEL)
	{
		DXAssert(D3DCompileFromFile(file.c_str(), nullptr, nullptr, "main", "ps_5_0", shaderFlags, 0, &blob, &errorMessages));
		byte_code = blob->GetBufferPointer();
		byte_code_size = blob->GetBufferSize();
	}
	else if (type == ShaderType::UNDEFINED)
	{
		return false;
	}

	if (errorMessages)
		errorMessages->Release();

	return true;
}
