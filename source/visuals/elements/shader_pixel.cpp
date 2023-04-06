
#include <d3dcompiler.h>

#include "util_funcs.h"
#include "../presenter.h"

#include "shader_pixel.h"

PixelShader::PixelShader(Presenter* presenter, const std::wstring& file, const std::string& entry)
	:shader()
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
	assert(device && context);

	UINT _shaderFlags = 0;
	ID3DBlob* _errorMessages = nullptr;

	#if defined( DEBUG ) || defined( _DEBUG )
		_shaderFlags |= D3DCOMPILE_DEBUG;
		_shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	DXAssert(D3DCompileFromFile(file.c_str(), nullptr, nullptr, entry.c_str(), "ps_5_0", _shaderFlags, 0, &blob, &_errorMessages));
	byte_code = blob->GetBufferPointer();
	byte_code_size = blob->GetBufferSize();
	DXAssert(device->CreatePixelShader(byte_code, byte_code_size, 0, &shader));
	DXRelease(_errorMessages);
}

PixelShader::~PixelShader()
{
	DXRelease(shader);
}

void PixelShader::Apply()
{
	context->PSSetShader(shader, 0, 0);
}
