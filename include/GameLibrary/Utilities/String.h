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
}
