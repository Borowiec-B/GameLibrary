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
	 *  FloatPrecision: Utility for storing a custom int precision, or max precision (float->string->float guarantee), likely to be used in streams.
	 *                  Also used for storing presets like normal().
	 */
	struct FloatPrecision {
		explicit constexpr FloatPrecision(const int precision) : precision(precision) {
			if (precision < 0)
				throw Exceptions::InvalidArgument("FloatPrecision::FloatPrecision() failed: precision can't be negative.");
		}
		
		constexpr static FloatPrecision normal() noexcept {
			return FloatPrecision(4);
		}

		constexpr static FloatPrecision max() noexcept {
			return FloatPrecision();
		}

		constexpr bool isMax() const noexcept {
			return !precision.has_value();
		}

		template<typename F>
		constexpr auto get() const {
			if (isMax())
				return std::numeric_limits<F>::max_digits10;
			else
				return precision.value();
		}

	private:
		explicit constexpr FloatPrecision() noexcept : precision(std::nullopt) {}

		std::optional<int> precision;
	};

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
	 *  toString() family: Try to return String representation of an object.
	 *  				   Optionally takes precision to be used in floating-point conversions.
	 *
	 *  Throws:
	 *    - InvalidArgument if precision level is invalid (negative or containing an unknown FloatPrecisionPreset).
	 *    - ConversionError if casting failed for any other reason.
	 */
	template<typename S = std::string, typename F>
	std::enable_if_t<std::is_floating_point_v<F>, S>
	toString(const F& value, const FloatPrecision& floatPrecision = FloatPrecision::normal()) {
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

		return stringstreamCast<S>(value, std::fixed, std::setprecision(floatPrecision.get<F>()));
	}

	template<typename S = std::string, typename T>
	std::enable_if_t<!std::is_floating_point_v<T>, S>
	toString(const T& value, const FloatPrecision& floatPrecision = FloatPrecision::normal()) {
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

