#pragma once

#include <type_traits>
#include <string>


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

	template<typename T>
	struct IsArithmeticOrString : public std::disjunction<std::is_arithmetic<std::decay_t<T>>, IsString<std::decay_t<T>>> {};
	template<typename T>
	constexpr inline bool IsArithmeticOrStringV = IsArithmeticOrString<T>::value;
}
