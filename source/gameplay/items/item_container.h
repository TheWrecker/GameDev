
#ifndef ITEM_CONTAINER_H
	#define ITEM_CONTAINER_H

	#include <vector>

	#include "item_stack.h"

	class ItemContainer
	{
	public:
		ItemContainer(unsigned int capacity);
		~ItemContainer();

		bool AddItem(ItemStack* target);
		bool AddItem(ItemType type, unsigned int count = 1, unsigned int capacity = 99);
		bool RemoveItem(ItemType type, unsigned int count = 1);
		bool RemoveItem(unsigned int slot);
		bool MoveItem(ItemType type, ItemContainer* destination);
		bool MoveItem(unsigned int slot, ItemContainer* destination);

		ItemStack* GetItemAtSlot(unsigned int slot);

		//TODO: add ability to swap item order/position

		bool HasFreeSlot(unsigned int& slot );
		unsigned int GetCapacity();


	private:
		friend class Overlay;

		std::vector<ItemStack> container;
		unsigned int capacity;
	};

#endif // !ITEM_CONTAINER_H
