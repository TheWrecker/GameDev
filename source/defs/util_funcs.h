
#ifndef UTIL_FUNCS_H
	#define UTIL_FUNCS_H

	#pragma warning (disable : 4996)

	#include <cassert>
	#include <codecvt>
	#include <locale>

	template<typename type>
	static inline void DXRelease(type& target)
	{
		if (target)
		{
			target->Release();
			target = nullptr;
		}
	}

	static inline std::wstring String_To_WString(const std::string& target)
	{
		auto _what = target.c_str();
		std::size_t _size = target.size() + 1;
		std::wstring _wc(_size, L'#');
		mbstowcs(&_wc[0], _what, _size);
		return _wc;
	}

	static inline std::string WString_To_String(const std::wstring& target)
	{
		auto _what = target.c_str();
		std::size_t _size = target.size() + 1;
		std::string _mc(_size, '#');
		wcstombs(&_mc[0], _what, _size);
		return _mc;
	}

	static inline int int_floor(float value)
	{
		int _i = (int)value;
		return _i - (_i > value);
	}

	static inline float float_floor(float value)
	{
		int _i = (int)value;
		return (float)(_i - (_i > value));
	}

	#if defined( DEBUG ) || defined( _DEBUG )
		#define DXAssert(x) if ((x) < 0) {assert(false);} 
		#define RetAssert(x) if (!(x)) {assert(false);} 
	#else
		#define DXAssert(x) x
		#define RetAssert(x) x 
	#endif // DEBUG

#endif // !UTIL_FUNCS_H