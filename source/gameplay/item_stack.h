
#ifndef ITEM_STACK_H
	#define ITEM_STACK_H

	#include <string>

	enum class ItemType
	{
		UNDEFINED = 0,
		TEST_BLOCK,
		GRASS_BLOCK,
		DIRT_BLOCK
	};

	class ItemContainer;

	class ItemStack
	{
	public:
		ItemStack(ItemType type, unsigned int max = 99, unsigned int current = 0);
		~ItemStack();
		void SetName(const std::string& what);

		void AddToStack(unsigned int count = 1);
		void RemoveFromStack(unsigned int count = 1);
		void Fill();
		void Clear();

		unsigned int GetCurrentCount();
		unsigned int GetCapacity();
		bool DoesContain(unsigned int count);
		bool HasCapacity(unsigned int count);
		bool IsEmpty();
		bool IsFull();

		ItemType GetType();
		ItemContainer* GetParentContainer();
		const std::string& GetName();

	private:
		friend class ItemContainer;

		ItemContainer* parent_container;
		ItemType type;
		unsigned int stack_current, stack_max;
		std::string name;
	};

#endif // !ITEM_STACK_H
