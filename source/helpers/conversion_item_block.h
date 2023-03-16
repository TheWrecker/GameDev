
#ifndef CONVERSION_ITEM_BLOCK_H
	#define CONVERSION_ITEM_BLOCK_H

	#include "defs_items.h"
	#include "defs_blocks.h"

	inline SolidBlockType ItemTypeToBlockType(ItemType type)
	{
		switch (type)
		{
		case ItemType::NO_ITEM: return SolidBlockType::UNDEFINED;
			break;
		case ItemType::TEST_BLOCK: return SolidBlockType::TEST;
			break;
		case ItemType::DIRT_BLOCK: return SolidBlockType::DIRT;
			break;
		case ItemType::GRASS_BLOCK: return SolidBlockType::GRASS_ON_DIRT;
			break;
		case ItemType::STONE_BLOCK: return SolidBlockType::STONE;
			break;
		case ItemType::SAND_BLOCK: return SolidBlockType::SAND;
			break;
		default: return SolidBlockType::UNDEFINED;
			break;
		}
	}

	inline ItemType BlockTypeToItemType(SolidBlockType type)
	{
		switch (type)
		{
		case SolidBlockType::UNDEFINED: return ItemType::NO_ITEM;
			break;
		case SolidBlockType::TEST: return ItemType::TEST_BLOCK;
			break;
		case SolidBlockType::DIRT: return ItemType::DIRT_BLOCK;
			break;
		case SolidBlockType::GRASS: return ItemType::GRASS_BLOCK;
			break;
		case SolidBlockType::GRASS_ON_DIRT: return ItemType::GRASS_BLOCK;
			break;
		case SolidBlockType::STONE: return ItemType::STONE_BLOCK;
			break;
		case SolidBlockType::SAND: return ItemType::SAND_BLOCK;
			break;
		default: return ItemType::NO_ITEM;
			break;
		}
	}

#endif // !CONVERSION_ITEM_BLOCK_H
