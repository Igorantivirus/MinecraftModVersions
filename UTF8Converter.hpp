#pragma once

//#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <locale>
#include <codecvt>


namespace utf8C
{
	#if !defined(char8_t)
	using char8_t = char;
	#endif // !defined(char8_t)

	template<typename T>
	std::enable_if_t<std::is_same_v<T, wchar_t> ||
		std::is_same_v<T, char8_t> ||
		std::is_same_v<T, char16_t> ||
		std::is_same_v<T, char32_t>, 
		std::string> to_utf8(const T* const array)
	{
		if constexpr (std::is_same_v<char8_t, T>)
			return std::string(reinterpret_cast<const char*>(array));
		else
		{
			std::wstring_convert<std::codecvt_utf8<T>, T> converter;
			return converter.to_bytes(std::basic_string<T>(array));
		}
	}

}


std::string operator""_utf8(const wchar_t* str, const size_t size)
{
	return utf8C::to_utf8(str);
}

std::string operator""_utf8(const char16_t* str, const size_t size)
{
	return utf8C::to_utf8(str);
}

std::string operator""_utf8(const char32_t* str, const size_t size)
{
	return utf8C::to_utf8(str);
}

#if defined(char8_t)

std::string operator""_utf8(const char8_t* str, const size_t size)
{
	return utf8C::to_utf8(str);
}

#endif