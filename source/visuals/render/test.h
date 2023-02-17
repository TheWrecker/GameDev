
#pragma once

//TODO: change to renderer interface

#include <d3d11.h>
#include <memory>
#include <vector>

#include "interface_drawable.h"
#include "../entities/entity_basic.h"

class VertexShader;
class PixelShader;
class InputLayout;
class StateMaster;
class BufferMaster;
class TextureManager;
class Scene;
class Presenter;

class TestRender : public IDrawable
{
public:
	TestRender(Presenter* presenter, Scene* scene);
	~TestRender();

	void Draw() override;

private:

	struct TestVertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 uv;
	};

	struct PerObjectBuffer
	{
		DirectX::XMMATRIX world;
	};

	std::unique_ptr<BasicEntity> test_object;
	std::unique_ptr<InputLayout> input_layout;
	std::unique_ptr<ConstantBuffer<PerObjectBuffer>> per_object_buffer;
	std::unique_ptr<VertexShader> vertex_shader;
	std::unique_ptr<PixelShader> pixel_shader;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Scene* scene;
	Presenter* presenter;
	StateMaster* state_master;
	BufferMaster* buffer_master;
	TextureManager* texture_manager;
};
