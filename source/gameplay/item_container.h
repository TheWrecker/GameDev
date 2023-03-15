
#ifndef ITEM_CONTAINER_H
	#define ITEM_CONTAINER_H

	#include <list>

	#include "item_stack.h"

	class ItemContainer
	{
	public:
		ItemContainer(unsigned int capacity);
		~ItemContainer();

		bool AddItem(ItemStack* target);
		bool AddItem(ItemType type, unsigned int count = 1, unsigned int capacity = 99);
		bool RemoveItem(ItemStack* target);
		bool RemoveItem(ItemType type, unsigned int count = 1);
		bool MoveItem(ItemStack* item, ItemContainer* destination);
		bool MoveItem(ItemType type, ItemContainer* destination);
		bool MoveItem(std::list<ItemStack*>::iterator iter, ItemContainer* destination);

		//TODO: add ability to swap item order/position

		unsigned int GetCapacity();
		bool HasFreeSlots();

	private:
		friend class Overlay;

		std::list<ItemStack*> container;
		unsigned int capacity;
	};

#endif // !ITEM_CONTAINER_H
