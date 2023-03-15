
#include <vector>

#include "item_stack.h"

std::vector<std::string> itemNames = {
    "Undefined",
    "Test Block",
    "Grass Block",
    "Dirt Block"
};

ItemStack::ItemStack(ItemType type, unsigned int max, unsigned int current)
    :type(type), stack_max(max), stack_current(current), name("")
{
}

ItemStack::~ItemStack()
{
}

void ItemStack::SetName(const std::string& what)
{
    name = what;
}

void ItemStack::AddToStack(unsigned int count)
{
    stack_current += count;
    stack_current > stack_max ? stack_current = stack_max : stack_current;
}

void ItemStack::RemoveFromStack(unsigned int count)
{
    if (DoesContain(count))
        stack_current -= count;
    else
        stack_current = 0;
}

void ItemStack::Fill()
{
    stack_current = stack_max;
}

void ItemStack::Clear()
{
    stack_current = 0;
}

unsigned int ItemStack::GetCurrentCount()
{
    return stack_current;
}

unsigned int ItemStack::GetCapacity()
{
    return stack_max;
}

ItemType ItemStack::GetType()
{
    return type;
}

ItemContainer* ItemStack::GetParentContainer()
{
    return nullptr;
}

const std::string& ItemStack::GetName()
{
    if (name == "")
        return itemNames[static_cast<unsigned int>(type)];

    return name;
}

bool ItemStack::IsEmpty()
{
    return stack_current > 0;
}

bool ItemStack::IsFull()
{
    return stack_current == stack_max;
}

bool ItemStack::DoesContain(unsigned int count)
{
    return stack_current >= count;
}

bool ItemStack::HasCapacity(unsigned int count)
{
    return (stack_max - stack_current) >= count;
}
