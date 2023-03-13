
#ifndef DEFS_PIPELINE_H
	#define DEFS_PIPELINE_H

	#include <DirectXMath.h>

	enum class BindStage
	{
		UNDEFINED = 0,
		VERTEX,
		PIXEL
	};

	struct DefaultVertexStruct
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
	};

	struct NormalVertexStruct
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
	};

	struct SolidBlockVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		float atlas_slice;
	};

	struct DefaultConstantStruct
	{
		DirectX::XMMATRIX matrix;
	};

	//we don't use first and last slots
	enum class DefaultConstants : unsigned int
	{
		//14 slots total
		UNDEFINED = 0,
		HUD_MATRIX = 10,
		SUN_LIGHT_DATA = 11,
		VIEW_PROJECTION_MATRIX = 12,
		END_PADDING = 13 //D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT (14)
	};

	enum class DefaultObjects : unsigned int
	{
		//32 slots total
		UNDEFINED = 0,
		QUAD_FULLSCREEN = 24,
		QUAD_NORMAL = 25,
		BLOCK_NORMAL = 26,
		SPHERE_NORMAL = 27,
		QUAD = 28,
		BLOCK = 29,
		SPHERE = 30,
		END_PADDING = 31 //D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT (32)
	};

	enum class DefaultTextures : unsigned int
	{
		UNDEFINED = 0,
		HIGHLIGHT = 119,
		FIRST_PASS = 120,
		CROSSHAIR_1 = 121,
		SKY_1 = 122,
		TEST_CHECKERS = 123,
		SUN = 124,
		EARTH = 125,
		TEXTURE_ATLAS = 126,
		END_PADDING = 127 //D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT (128)
	};

	enum class DefaultSampler
	{
		UNDEFINED = 0,
		POINT = 13,
		BILINEAR = 14,
		END_PADDING = 15 //D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT (16)
	};

#endif // !DEFS_PIPELINE_H
