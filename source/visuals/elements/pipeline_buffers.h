
#pragma once

#include <d3d11.h>

#include "buffer_constant.h"

class Presenter;

enum class DefaultConstants
{
	UNDEFINED = 0,
	VIEW_PROJECTION_MATRIX = 1
};

enum class DefaultObjects
{
	UNDEFINED = 0,
	BLOCK = 1,
	SPHERE = 2
};


class BufferMaster
{
public:
	BufferMaster(Presenter* presenter);
	~BufferMaster();

	void UpdateDefaultConstant(DefaultConstants target);
	void BindDefaultConstant(DefaultConstants target);
	void UnbindDefaultConstant(DefaultConstants target);

	void RebuildDefaultObjects();
	void BindDefaultObject(DefaultObjects target);
	void UnbindDefaultObject(DefaultObjects target);

	unsigned int GetCurrentSlot(DefaultConstants target);
	unsigned int GetCurrentSlot(DefaultObjects target);

private:
	Presenter* presenter;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
};
