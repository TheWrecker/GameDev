
#pragma once

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

struct DefaultConstantStruct
{
	DirectX::XMMATRIX matrix;
};
