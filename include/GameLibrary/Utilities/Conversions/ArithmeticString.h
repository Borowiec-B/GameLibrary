#pragma once

#include "GameLibrary/Exceptions/Conversions.h"
#include "GameLibrary/Utilities/Conversions/Arithmetic.h"
#include "GameLibrary/Utilities/Conversions/String.h"


namespace GameLibrary::Utilities::Conversions
{
	/*
	 *  arithmeticOrStringCast(): Perform conversion Arithmetic/String -> Arithmetic/String.
	 *
	 *  Guidelines (each position is an if-else):
	 *    - To == From (both decayed):	return from.
	 *    - Both types are arithmetic:	perform safeArithmeticCast()
	 *    - To is String:				perform toString() on from.
	 *    - From is String:				perform fromString() on from.
	 *
	 *  Throws:
	 *    - ConversionError if any underlying conversion functions throw.
	 */
	template<typename To, typename From>
	std::enable_if_t<(std::is_arithmetic_v<To> || IsStringV<To>) && (std::is_arithmetic_v<From> || IsStringV<From>), 
	To>
	arithmeticOrStringCast(From&& from) {
		try
		{
			if constexpr (std::is_same_v<std::decay_t<To>, std::decay_t<From>>)
				return std::forward<From>(from);
			else if constexpr (std::is_arithmetic_v<To> && std::is_arithmetic_v<From>)
				return safeArithmeticCast<To>(std::forward<From>(from));
			else if constexpr (IsStringV<To>)
				return toString<To>(std::forward<From>(from));
			else
				return fromString<To>(std::forward<From>(from));
		}
		catch (const std::exception&)
		{
			throw Exceptions::ConversionError::fromTypes<From, To>();
		}
	}
}
