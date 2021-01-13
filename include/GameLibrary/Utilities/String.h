#pragma once

#include "GameLibrary/Utilities/Conversions/StringToSstream.h"


namespace GameLibrary::Utilities
{
	template<typename T>
	struct IsString : public std::disjunction<
					   std::is_same<std::decay_t<T>, char*>,
					   std::is_same<std::decay_t<T>, const char*>,
					   std::is_same<std::decay_t<T>, wchar_t*>,
					   std::is_same<std::decay_t<T>, const wchar_t*>,
					   std::is_same<std::decay_t<T>, std::string>,
					   std::is_same<std::decay_t<T>, std::wstring>
	> {};
	template<typename T>
	inline constexpr bool IsStringV = IsString<T>::value;

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

	std::string quote(const char* const str);
	std::wstring quote(const wchar_t* const str);
}
