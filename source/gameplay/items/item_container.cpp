
#include "item_container.h"

ItemContainer::ItemContainer(unsigned int capacity)
    :capacity(capacity)
{
    container.resize(capacity, ItemStack(ItemType::NO_ITEM));
}

ItemContainer::~ItemContainer()
{
}

bool ItemContainer::AddItem(ItemStack* target)
{
    if (!target)
        return false;

    for (auto& slot : container)
    {
        if (slot.type == target->type)
        {
            if (slot.HasCapacity(target->stack_current))
            {
                slot.AddToStack(target->stack_current);
                target->Clear();
                return true;
            }
        }
    }

    unsigned int _slot = 0;
    if (HasFreeSlot(_slot))
    {
        container[_slot] = *target;
        return true;
    }

    return false;
}

bool ItemContainer::AddItem(ItemType type, unsigned int count, unsigned int capacity)
{
    if (type == ItemType::NO_ITEM)
        return false;

    for (auto& item : container)
    {
        if (item.type == type)
        {
            if ((item.HasCapacity(count)) && (item.GetCapacity() == capacity))
            {
                item.AddToStack(count);
                return true;
            }
        }
    }

    unsigned int _slot = 0;
    if (HasFreeSlot(_slot))
    {
        container[_slot].type = type;
        container[_slot].stack_max = capacity;
        container[_slot].stack_current = count;
        return true;
    }

    return false;
}

bool ItemContainer::RemoveItem(ItemType type, unsigned int count)
{
    if (type == ItemType::NO_ITEM)
        return false;

    for (auto& item : container)
    {
        if (item.type == type)
        {
            if (item.DoesContain(count))
            {
                item.RemoveFromStack(count);

                if (item.IsEmpty())
                    item.Clear();

                return true;
            }
        }
    }

    return false;
}

bool ItemContainer::RemoveItem(unsigned int slot)
{
    if (slot > capacity)
        return false;

    if (container[slot].type == ItemType::NO_ITEM)
        return false;

    container[slot].Clear();
    return true;
}

bool ItemContainer::MoveItem(ItemType type, ItemContainer* destination)
{
    if (!destination)
        return false;

    if (type == ItemType::NO_ITEM)
        return false;

    for (auto& item : container)
    {
        if (item.type == type)
        {
            if (destination->AddItem(&item))
            {
                item.Clear();
                return true;
            }
        }
    }

    return false;
}

bool ItemContainer::MoveItem(unsigned int slot, ItemContainer* destination)
{
    if (!destination)
        return false;

    if (container[slot].type == ItemType::NO_ITEM)
        return false;

    if (destination->AddItem(&container[slot]))
    {
        container[slot].Clear();
        return true;
    }

    return false;
}

ItemStack* ItemContainer::GetItemAtSlot(unsigned int slot)
{
    if (slot > capacity)
        return nullptr;

    return &container[slot];
}

bool ItemContainer::HasFreeSlot(unsigned int& slot)
{
    unsigned int _free = 0;
    for (auto& slot : container)
    {
        if (slot.type == ItemType::NO_ITEM)
            break;

        _free++;
    }

    if (_free < capacity)
    {
        slot = _free;
        return true;
    }
    else
    {
        return false;
    }
}

unsigned int ItemContainer::GetCapacity()
{
    return capacity;
}
