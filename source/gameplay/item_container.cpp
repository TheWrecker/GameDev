
#include "item_container.h"

ItemContainer::ItemContainer(unsigned int capacity)
    :capacity(capacity)
{
}

ItemContainer::~ItemContainer()
{
    for (auto item : container)
        delete item;
}

bool ItemContainer::AddItem(ItemStack* target)
{
    if (target->parent_container)
        return false;

    if (container.empty())
    {
        container.push_back(target);
        target->parent_container = this;
        return true;
    }
    else
    {
        for (auto item : container)
        {
            if (item == target)
                return false;
        }

        for (auto item : container)
        {
            if (item->type == target->type)
            {
                if (item->HasCapacity(target->stack_current))
                {
                    item->AddToStack(target->stack_current);
                    delete target;
                    return true;
                }
            }
        }

        if (HasFreeSlots())
        {
            container.push_back(target);
            target->parent_container = this;
            return true;
        }
    }

    return false;
}

bool ItemContainer::AddItem(ItemType type, unsigned int count, unsigned int capacity)
{
    for (auto item : container)
    {
        if (item->type == type)
        {
            if ((item->HasCapacity(count)) && (item->GetCapacity() == capacity))
            {
                item->AddToStack(count);
                return true;
            }
        }
    }

    if (HasFreeSlots())
    {
        auto _item = new ItemStack(type, capacity, count);
        container.push_back(_item);
        _item->parent_container = this;
        return true;
    }

    return false;
}

bool ItemContainer::RemoveItem(ItemStack* target)
{
    if (container.empty())
    {
        return false;
    }
    else
    {
        auto _size1 = container.size();
        container.remove(target);
        auto _size2 = container.size();

        if (_size1 == _size2)
            return false;
    }

    return true;
}

bool ItemContainer::RemoveItem(ItemType type, unsigned int count)
{
    if (container.empty())
    {
        return false;
    }
    else
    {
        for (auto item : container)
        {
            if (item->type == type)
            {
                if (item->DoesContain(count))
                {
                    item->RemoveFromStack(count);

                    if (item->IsEmpty())
                        delete item;

                    return true;
                }
            }
        }
    }

    return false;
}

bool ItemContainer::MoveItem(ItemStack* item, ItemContainer* destination)
{
    if ((item->parent_container != this) || (!destination))
        return false;

    item->parent_container = nullptr;
    if (destination->AddItem(item))
    {
        container.remove(item);
        return true;
    }

    item->parent_container = this;
    return false;
}

bool ItemContainer::MoveItem(ItemType type, ItemContainer* destination)
{
    if (!destination)
        return false;

    for (auto item : container)
    {
        if (item->type == type)
        {
            item->parent_container = nullptr;
            if (destination->AddItem(item))
            {
                container.remove(item);
                return true;
            }
            item->parent_container = this;
        }
    }

    return false;
}

bool ItemContainer::MoveItem(std::list<ItemStack*>::iterator iter, ItemContainer* destination)
{
    auto _item = *iter;

    if ((_item->parent_container != this) || (!destination))
        return false;

    _item->parent_container = nullptr;
    if (destination->AddItem(_item))
    {
        container.remove(_item);
        return true;
    }

    _item->parent_container = this;
    return false;
}

unsigned int ItemContainer::GetCapacity()
{
    return capacity;
}

bool ItemContainer::HasFreeSlots()
{
    return container.size() < capacity;
}
