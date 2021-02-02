#pragma once

#include <cctype>
#include <cwctype>

#include "GameLibrary/Utilities/Conversions/String.h"
#include "GameLibrary/Utilities/Conversions/StringToSstream.h"


namespace GameLibrary::Utilities
{
	template<typename S, typename... Args>
	S compose(Args&&... args) {
		auto stream = Conversions::stringToOstringstream<S>();
		
		(stream << ... << Conversions::toString<S>(std::forward<Args>(args)));

		return stream.str();
	}

	template<typename S1, typename S2>
	S1 surround(const S1& str, const S2& surrounder) {
		auto stream = Conversions::stringToOstringstream<S1>();
		
		stream << surrounder << str << surrounder;

		return stream.str();
	}

	template<typename S2>
	std::string surround(const char* const str, const S2& surrounder) {
		return surround<std::string, S2>(str, surrounder);
	}

	template<typename S2>
	std::wstring surround(const wchar_t* const str, const S2& surrounder) {
		return surround<std::wstring, S2>(str, surrounder);
	}

	template<typename S>
	S quote(const S& str) {
		return surround(str, '"');
	}

	template<typename C>
	bool isWhitespace(const C c) {
		if constexpr (std::is_same_v<std::decay_t<C>, wchar_t>)
			return std::iswspace(c);
		else
			return std::isspace(c);
	}

	std::string quote(const char* const str);
	std::wstring quote(const wchar_t* const str);
}
