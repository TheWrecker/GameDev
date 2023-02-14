
#pragma once

#include <d3d11.h>
#include <memory>

class VertexShader;
class PixelShader;
class InputLayout;
class IndexBuffer;
class BasicCamera;
class Scene;
class Presenter;

class RenderBase
{
public:
	RenderBase(Scene* scene);
	~RenderBase();

	virtual void Render() = 0;
	virtual void Update();

protected:

	std::unique_ptr<InputLayout> input_layout;
	std::unique_ptr<IndexBuffer> index_buffer;
	std::unique_ptr<VertexShader> vertex_shader;
	std::unique_ptr<PixelShader> pixel_shader;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Scene* scene;
	BasicCamera* camera;
	Presenter* presenter;
};
