
#ifndef MASTER_STATE_H
	#define MASTER_STATE_H

	#include <d3d11.h>
	#include <vector>

	#include "defs_pipeline.h"

	class TextureSampler;
	class Presenter;

	class StateMaster
	{
	public:
		StateMaster(Presenter* parent);
		~StateMaster();

		void BindDefaultTextureSampler(DefaultSampler what);
		void UnbindDefaultTextureSampler(DefaultSampler what);
		unsigned int GetSamplerSlot(DefaultSampler target);

	private:
		Presenter* presenter;
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		std::vector<TextureSampler*> texture_samplers;
	};

#endif // !MASTER_STATE_H
