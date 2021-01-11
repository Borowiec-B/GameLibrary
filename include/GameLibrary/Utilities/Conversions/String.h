#pragma once

#include <limits>
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

#include "boost/lexical_cast.hpp"

#include "GameLibrary/Exceptions/Conversions.h"
#include "GameLibrary/Exceptions/Standard.h"
#include "GameLibrary/Utilities/Conversions/StringToSstream.h"

namespace GameLibrary::Utilities::Conversions
{
	/*
	 *  FloatPrecision: Used to specify desired precision when converting floats to strings.
	 *  				Results will be rounded.
	 *  
	 *  Normal uses std::numeric_limits<T>::digits10 and std::noshowpoint.
	 *  Max uses std::numeric_limits<T>::max_digits10.
	 */
	enum class FloatPrecisionPreset {
		Normal,
		Max
	};
	using FloatPrecision = std::variant<int, FloatPrecisionPreset>;


	/*
	 *  stringstreamCast: Pass all stringstreamFlags... to a Stringstream, then value, then return resulting .str().
	 */
	template<typename String = std::string, typename T, typename... StringstreamFlags>
	String stringstreamCast(const T& value, const StringstreamFlags&... stringstreamFlags) {
		auto conversionStream = stringToOstringstream<String>();

		(conversionStream << ... << stringstreamFlags) << value;

		return conversionStream.str();
	}

	/*
	 *  floatPrecisionToInt: FloatPrecision can contain either FloatPrecisionPreset or int. Convert it to int.
	 *
	 *  Throws:
	 *    - InvalidArgument when floatPrecision contains a negative number or unhandled FloatPrecisionPreset.
	 */
	template<typename F>
	std::enable_if_t<std::is_floating_point_v<F>, int> floatPrecisionToInt(const FloatPrecision& floatPrecision) {
		int ret;

		if (std::holds_alternative<FloatPrecisionPreset>(floatPrecision))
		{
			const auto precisionPreset = std::get<FloatPrecisionPreset>(floatPrecision);
			switch (precisionPreset)
			{
				case FloatPrecisionPreset::Normal:
					ret = std::numeric_limits<F>::digits10;
					break;
				case FloatPrecisionPreset::Max:
					ret = std::numeric_limits<F>::max_digits10;
					break;
				default:
					throw Exceptions::InvalidArgument("Used an unimplemented FloatPrecisionPreset.");
					break;
			}
		}
		else if (std::holds_alternative<int>(floatPrecision))
		{
			const int storedPrecision = std::get<int>(floatPrecision);
			if (storedPrecision < 0)
				throw Exceptions::InvalidArgument("Attempted to set negative float precision.");

			ret = storedPrecision;
		}

		return ret;
	}

	template<typename String = std::string, typename F>
	std::enable_if_t<std::is_floating_point_v<F>, String>
	floatToString(F value, const FloatPrecision& floatPrecision = FloatPrecisionPreset::Normal) {
		const int precision = floatPrecisionToInt<F>(floatPrecision);

		return stringstreamCast<String>(value, std::setprecision(precision));
	}

	template<typename String = std::string, typename T>
	String toString(T&& value, const FloatPrecision& floatPrecision = FloatPrecisionPreset::Normal) {
		if constexpr(std::is_floating_point_v<std::decay_t<T>>)
			return floatToString<String>(value, floatPrecision);
		else
			return stringstreamCast<String>(std::forward<T>(value));
	}

	template<typename F, typename S>
	F floatFromString(S&& str) {
		try
		{
			// std::sto* functions throw on extreme values, so boost::lexical_cast is preferable for Float to Float.
			return boost::lexical_cast<F>(std::forward<S>(str));
		}
		catch (const boost::bad_lexical_cast&)
		{
			throw Exceptions::ConversionError::fromTypes<S&&, F>();
		}
	}

	template<typename T, typename S>
	T fromString(S&& str) {
		if constexpr (std::is_floating_point_v<T>)
			return floatFromString<T>(std::forward<S>(str));
	}
}

