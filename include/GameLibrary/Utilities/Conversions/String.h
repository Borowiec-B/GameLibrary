#pragma once

#include <limits>
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

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
	 *
	 *  Throws:
	 *    - ConversionError when Stringstream's operator<< throws, called with value or supplied flags.
	 */
	template<typename S = std::string, typename T, typename... SF>
	S stringstreamCast(const T& value, const SF&... stringstreamFlags) {
		auto conversionStream = stringToOstringstream<S>();
		try {
			(conversionStream << ... << stringstreamFlags) << value;
		}
		catch (const std::exception&) {
			throw Exceptions::ConversionError::fromTypes<T, S>("stringstreamCast() failed.");
		}

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

	/*
	 *  toString() family: Try to return String representation of an object.
	 *  				   Optionally takes precision to be used in floating-point conversions.
	 *
	 *  Throws:
	 *    - InvalidArgument if precision level is invalid (negative or containing an unknown FloatPrecisionPreset).
	 *    - ConversionError if casting failed for any other reason.
	 */
	template<typename S = std::string, typename F>
	std::enable_if_t<std::is_floating_point_v<F>, S>
	toString(const F& value, const FloatPrecision& floatPrecision = FloatPrecisionPreset::Normal) {
		switch (fpclassify(value))
		{
			case FP_NAN:
				if constexpr (std::is_same_v<typename S::value_type, wchar_t>)
					return L"nan";
				else
					return "nan";
			case FP_INFINITE:
				if constexpr (std::is_same_v<typename S::value_type, wchar_t>)
					return L"inf";
				else
					return "inf";
		}

		const int precision = floatPrecisionToInt<F>(floatPrecision);

		return stringstreamCast<S>(value, std::setprecision(precision));
	}

	template<typename S = std::string, typename T>
	std::enable_if_t<!std::is_floating_point_v<T>, S>
	toString(const T& value, const FloatPrecision& floatPrecision = FloatPrecisionPreset::Normal) {
		return stringstreamCast<S>(value);
	}

	/*
	 *  fromString() family: Try to return chosen type's representation of an object.
	 *						 Currently implemented conversions are:
	 *						   - String->Arithmetic
	 *						   - A few cases of String->String:
	 *						     - Conversions to char pointers aren't implemented
	 *						     - Literal to its equivalent String type (wchar_t* to wstring, char* to string)
	 *						     - Normal literal to wide string.
	 *
	 *  Throws:
	 *    - ConversionError if casting failed for any reason.
	 */

	template<typename F, typename S>
	std::enable_if_t<std::is_floating_point_v<F>, F>
	fromString(const S& str) {
		try {
			// std::sto* functions throw on extreme values, so boost::lexical_cast is preferable over <string>'s std::sto*.
			return boost::lexical_cast<F>(str);
		}
		catch (const boost::bad_lexical_cast&) {
			throw Exceptions::ConversionError::fromTypes<S, F>();
		}
	}

	template<typename I, typename S>
	std::enable_if_t<std::is_integral_v<I>, I>
	fromString(const S& str) {
		try
		{
			return boost::lexical_cast<I>(str);
		}
		catch (const boost::bad_lexical_cast&)
		{
			try
			{
				// Try to recover if bad_lexical_cast is caused by str being floating-point.
				const auto ld = fromString<long double>(str);
				return boost::numeric_cast<I>(ld);
			}
			// Plenty of exception types could have been thrown at this point; they're unrecoverable, just catch everything.
			catch (const std::exception&)
			{
				throw Exceptions::ConversionError::fromTypes<S, I>("fromString() failed");
			}
		}
	}

	// Last-ditch effort; assume T is a string.
	template<typename T, typename S>
	std::enable_if_t<!std::is_floating_point_v<T> && !std::is_integral_v<T>, T>
	fromString(const S& str) {
		return toString<T>(str);
	}
}

