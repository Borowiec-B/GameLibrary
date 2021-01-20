#pragma once

#include <cmath>

#include <boost/numeric/conversion/cast.hpp>

#include "GameLibrary/Exceptions/Conversions.h"


namespace GameLibrary::Utilities::Conversions
{
	template<typename To, typename From>
	To safeArithmeticCast(From from) {
		if constexpr (std::is_floating_point_v<std::decay_t<From>>)
		{
			const auto category = std::fpclassify(from);
			if ((category != FP_NORMAL) && (category != FP_ZERO) && (category != FP_SUBNORMAL))
			{
				throw Exceptions::ConversionError::fromTypes<From, To>("safeArithmeticCast() failed: invalid floating-point argument.");
			}
		}

		try {
			return boost::numeric_cast<To>(from);
		} catch (...) {
			throw Exceptions::ConversionError::fromTypes<From, To>("safeArithmeticCast() failed.");
		}
	}
}

