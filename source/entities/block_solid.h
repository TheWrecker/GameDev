
#ifndef BLOCK_SOLID_H
	#define BLOCK_SOLID_H

	#include "defs_blocks.h"
	#include "entity_basic.h"

	class SolidBlock : public BasicEntity
	{
	public:
		SolidBlock(BlockType type = BlockType::TEST, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		~SolidBlock();

		void SetType(BlockType what);
		void SetPosition(float x, float y, float z) override;
		void SetPosition(DirectX::XMFLOAT3 pos) override;

		BlockType GetBlockType();
		unsigned int GetAtlasIndex();

	private:
		BlockType type;
	};

#endif // !BLOCK_SOLID_H
