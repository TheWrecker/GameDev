
#include "util_funcs.h"

#include "shader_interface.h"

IShader::IShader()
	:blob(), byte_code(nullptr), byte_code_size(0), device(), context()
{
}

IShader::~IShader()
{
	DXRelease(blob);
}

std::size_t IShader::GetSize()
{
	return byte_code_size;
}

void* IShader::GetByteCode()
{
	return byte_code;
}
