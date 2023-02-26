
#pragma once

#include <d3d11.h>

class TEMP_BLOCK_HOLDER;

class SolidBlockProcessor
{
public:
	SolidBlockProcessor();
	~SolidBlockProcessor();

	void Rebuild(TEMP_BLOCK_HOLDER* target);

private:

};