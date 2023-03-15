
#ifndef UTIL_FUNCS_H
	#define UTIL_FUNCS_H

	#pragma warning (disable : 4996)

	#include <cassert>
	#include <codecvt>
	#include <locale>

	template<typename type>
	inline void DXRelease(type& target)
	{
		if (target)
		{
			target->Release();
			target = nullptr;
		}
	}

	//std::wstring String_To_WString(const std::string& str)
	//{
	//    using convert_typeX = std::codecvt_utf8<wchar_t>;
	//    std::wstring_convert<convert_typeX, wchar_t> converterX;
	//
	//    return converterX.from_bytes(str);
	//}
	//
	//std::string WString_To_String(const std::wstring& wstr)
	//{
	//    using convert_typeX = std::codecvt_utf8<wchar_t>;
	//    std::wstring_convert<convert_typeX, wchar_t> converterX;
	//
	//    return converterX.to_bytes(wstr);
	//}

	inline std::wstring String_To_WString(const std::string& target)
	{
		auto what = target.c_str();
		std::size_t size = target.size() + 1;
		std::wstring wc(size, L'#');
		mbstowcs(&wc[0], what, size);
		return wc;
	}

	inline std::string WString_To_String(const std::wstring& target)
	{
		auto what = target.c_str();
		std::size_t size = target.size() + 1;
		std::string mc(size, '#');
		wcstombs(&mc[0], what, size);
		return mc;
	}

	#if defined( DEBUG ) || defined( _DEBUG )
		#define DXAssert(x) if ((x) < 0) {assert(false);} 
		#define RetAssert(x) if (!(x)) {assert(false);} 
	#else
		#define DXAssert(x) x
		#define RetAssert(x) x 
	#endif // DEBUG

#endif // !UTIL_FUNCS_H