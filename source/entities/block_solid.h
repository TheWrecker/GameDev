
#ifndef BLOCK_SOLID_H
	#define BLOCK_SOLID_H

	#include <string>
	#include <DirectXCollision.h>

	#include "entity_basic.h"

	enum class FaceName
	{
		UNDEFINED,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		FRONT,
		BACK
	};

	enum class SolidBlockType
	{
		TEST = 1,
		DIRT = 2,
		GRASS = 3
	};

	class SolidBlock : public BasicEntity
	{
	public:
		SolidBlock(SolidBlockType type = SolidBlockType::TEST, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		~SolidBlock();

		void SetType(SolidBlockType what);
		void SetPosition(float x, float y, float z) override;
		void SetPosition(DirectX::XMFLOAT3 pos) override;

		SolidBlockType GetBlockType();
		DirectX::BoundingBox& GetBoundingBox();
		unsigned int GetAtlasIndex();

	private:
		SolidBlockType type;
		DirectX::BoundingBox collision_box;
	};

#endif // !BLOCK_SOLID_H
