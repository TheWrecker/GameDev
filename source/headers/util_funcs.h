
#pragma once

#include <cassert>

template<typename type>
void DXRelease(type& target)
{
	if (target)
		target->Release();
}

#define DXAssert(x) if ((x) < 0) {assert(false);} 