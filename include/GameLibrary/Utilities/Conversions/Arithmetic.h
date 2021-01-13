#pragma once

#include <boost/numeric/conversion/cast.hpp>

#include "GameLibrary/Exceptions/Conversions.h"


namespace GameLibrary::Utilities::Conversions
{
	template<typename To, typename From>
	To safeArithmeticCast(From from) {
		try
		{
			return boost::numeric_cast<To>(from);
		}
		catch (const std::exception&)
		{
			throw Exceptions::ConversionError::fromTypes<From, To>("safeArithmeticCast() failed.");
		}
	}
}

