
#ifndef SHADER_VERTEX_H
	#define SHADER_VERTEX_H

	#include <d3d11.h>
	#include <string>

	#include "shader_interface.h"

	class Presenter;

	class VertexShader : public IShader
	{
	public:
		VertexShader(Presenter* presenter, const std::wstring& file, const std::string& entry = "main");
		~VertexShader();

		void Apply() override;

	private:
		ID3D11VertexShader* shader;
	};

#endif // !SHADER_VERTEX_H
