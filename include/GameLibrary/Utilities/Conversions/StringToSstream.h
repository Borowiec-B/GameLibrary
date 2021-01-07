#pragma once

#include <sstream>


namespace GameLibrary::Utilities::Conversions
{
	/*
	 *  stringToStringStream family: Construct a Stringstream based on String's member types.
	 */
	template<typename String>
	auto stringToStringstream() {
		return std::basic_stringstream<typename String::value_type, typename String::traits_type, typename String::allocator_type>();
	}

	template<typename String>
	auto stringToIstringstream() {
		return std::basic_istringstream<typename String::value_type, typename String::traits_type, typename String::allocator_type>();
	}

	template<typename String>
	auto stringToOstringstream() {
		return std::basic_ostringstream<typename String::value_type, typename String::traits_type, typename String::allocator_type>();
	}
}
