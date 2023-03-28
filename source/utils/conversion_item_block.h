
#ifndef CONVERSION_ITEM_BLOCK_H
	#define CONVERSION_ITEM_BLOCK_H

	#include "defs_items.h"
	#include "defs_blocks.h"

	//TODO: bench against std::unordered_map for performance, when types reach higher numbers

	inline BlockType ItemTypeToBlockType(ItemType type)
	{
		switch (type)
		{
		case ItemType::NO_ITEM: return BlockType::EMPTY;
			break;
		case ItemType::TEST_BLOCK: return BlockType::TEST;
			break;
		case ItemType::DIRT_BLOCK: return BlockType::DIRT;
			break;
		case ItemType::GRASS_BLOCK: return BlockType::GRASS_ON_DIRT;
			break;
		case ItemType::STONE_BLOCK: return BlockType::STONE;
			break;
		case ItemType::SAND_BLOCK: return BlockType::SAND;
			break;
		default: return BlockType::EMPTY;
			break;
		}
	}

	inline ItemType BlockTypeToItemType(BlockType type)
	{
		switch (type)
		{
		case BlockType::EMPTY: return ItemType::NO_ITEM;
			break;
		case BlockType::TEST: return ItemType::TEST_BLOCK;
			break;
		case BlockType::DIRT: return ItemType::DIRT_BLOCK;
			break;
		case BlockType::GRASS: return ItemType::GRASS_BLOCK;
			break;
		case BlockType::GRASS_ON_DIRT: return ItemType::GRASS_BLOCK;
			break;
		case BlockType::STONE: return ItemType::STONE_BLOCK;
			break;
		case BlockType::SAND: return ItemType::SAND_BLOCK;
			break;
		default: return ItemType::NO_ITEM;
			break;
		}
	}

#endif // !CONVERSION_ITEM_BLOCK_H
