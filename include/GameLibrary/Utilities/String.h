#pragma once

#include "GameLibrary/Utilities/Conversions/String.h"


namespace GameLibrary::Utilities
{
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

	std::string quote(const char* const str) {
		return surround<std::string>(str, '"');
	}

	std::wstring quote(const wchar_t* const str) {
		return surround<std::wstring>(str, L'"');
	}
}
