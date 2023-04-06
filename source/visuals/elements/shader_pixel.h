
#ifndef SHADER_PIXEL_H
	#define SHADER_PIXEL_H

	#include <d3d11.h>
	#include <string>

	#include "shader_interface.h"

	class Presenter;

	class PixelShader : public IShader
	{
	public:
		PixelShader(Presenter* presenter, const std::wstring& file, const std::string& entry = "main");
		~PixelShader();

		void Apply() override;

	private:
		ID3D11PixelShader* shader;
	};

#endif // !SHADER_PIXEL_H
