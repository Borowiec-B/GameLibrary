#pragma once

#include <sstream>


namespace GameLibrary::Utilities::Conversions
{
	/*
	 *  stringToStringStream family: Construct a Stringstream based on String's member types.
	 */
	template<typename S>
	auto stringToStringstream() {
		return std::basic_stringstream<typename S::value_type, typename S::traits_type, typename S::allocator_type>();
	}

	template<typename S>
	auto stringToIstringstream() {
		return std::basic_istringstream<typename S::value_type, typename S::traits_type, typename S::allocator_type>();
	}

	template<typename S>
	auto stringToOstringstream() {
		return std::basic_ostringstream<typename S::value_type, typename S::traits_type, typename S::allocator_type>();
	}
}
