
#pragma once

#include <cassert>
#include <codecvt>
#include <locale>

template<typename type>
void DXRelease(type& target)
{
	if (target)
		target->Release();
}

std::wstring String_To_WString(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string WString_To_String(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

#define DXAssert(x) if ((x) < 0) {assert(false);} 